#pragma once
#include <mutex>
#include <memory>

class TaskQueue
{
public:
    struct Task
    {
        int id, priority = 1;
        std::wstring command, log;
        HANDLE process = nullptr, job = nullptr;
        std::string state = "waiting";
        DWORD result = 0;
        bool paused = false;
        ~Task() { if (process) CloseHandle(process); if (job) CloseHandle(job); }
    };
    std::mutex mutex;
    std::vector<std::shared_ptr<Task>> tasks;
    bool enabled = false;
    int parallel = 2;

    TaskQueue() : worker([this] { Work(); }) {}
    ~TaskQueue()
    {
        { std::lock_guard<std::mutex> lock(mutex); stopping = true; }
        worker.join();
        for (auto& task : tasks)
            if (task->job && task->state == "running") TerminateJobObject(task->job, ERROR_CANCELLED);
    }

    int Add(const std::wstring& command)
    {
        std::lock_guard<std::mutex> lock(mutex);
        auto task = std::make_shared<Task>();
        task->id = static_cast<int>(tasks.size()) + 1;
        task->command = command;
        wchar_t temp[MAX_PATH];
        if (!GetTempPathW(MAX_PATH, temp)) return 0;
        task->log = std::wstring(temp) + L"toolbox-task-" + std::to_wstring(GetCurrentProcessId()) + L"-" + std::to_wstring(task->id) + L".log";
        tasks.push_back(task);
        return task->id;
    }

    static DWORD Priority(int value)
    {
        return value == 0 ? BELOW_NORMAL_PRIORITY_CLASS : value == 2 ? ABOVE_NORMAL_PRIORITY_CLASS : NORMAL_PRIORITY_CLASS;
    }

    bool Control(int id, int action, int priority = 1)
    {
        std::lock_guard<std::mutex> lock(mutex);
        if (id <= 0 || id > static_cast<int>(tasks.size())) return false;
        auto& task = *tasks[id - 1];
        if (task.state != "waiting" && task.state != "running") return false;
        if (action == 3)
        {
            if (priority < 0 || priority > 2) return false;
            task.priority = priority;
        }
        if (task.state == "waiting")
        {
            if (action == 1) task.paused = true;
            if (action == 2) task.paused = false;
            return true;
        }
        // Job membership confines process controls to this task and its children.
        std::vector<BYTE> data(sizeof(JOBOBJECT_BASIC_PROCESS_ID_LIST) + 4096 * sizeof(ULONG_PTR));
        auto* ids = reinterpret_cast<JOBOBJECT_BASIC_PROCESS_ID_LIST*>(data.data());
        if (!QueryInformationJobObject(task.job, JobObjectBasicProcessIdList, ids, static_cast<DWORD>(data.size()), nullptr)) return false;
        using Suspend = LONG (NTAPI*)(HANDLE);
        auto address = GetProcAddress(GetModuleHandleW(L"ntdll.dll"), action == 1 ? "NtSuspendProcess" : "NtResumeProcess");
        Suspend suspend = nullptr;
        static_assert(sizeof(suspend) == sizeof(address), "Function pointer size");
        memcpy(&suspend, &address, sizeof(suspend));
        if (action != 3 && (!suspend || (action == 1) == task.paused)) return false;
        bool ok = true;
        for (DWORD i = 0; i < ids->NumberOfProcessIdsInList; ++i)
        {
            HANDLE process = OpenProcess(PROCESS_SET_INFORMATION | 0x0800, FALSE, static_cast<DWORD>(ids->ProcessIdList[i]));
            if (!process) { ok = false; continue; }
            if (action == 3) ok = SetPriorityClass(process, Priority(priority)) && ok;
            else ok = (suspend(process) >= 0) && ok;
            CloseHandle(process);
        }
        if (action == 1) task.paused = true;
        if (action == 2) task.paused = false;
        return ok;
    }

private:
    bool stopping = false;
    std::thread worker;
    void Work()
    {
        while (true)
        {
            {
                std::lock_guard<std::mutex> lock(mutex);
                if (stopping) return;
                int running = 0;
                for (auto& task : tasks)
                {
                    if (task->state != "running") continue;
                    JOBOBJECT_BASIC_ACCOUNTING_INFORMATION info{};
                    if (!QueryInformationJobObject(task->job, JobObjectBasicAccountingInformation, &info, sizeof(info), nullptr) || info.ActiveProcesses != 0)
                    { if (!task->paused) ++running; continue; }
                    GetExitCodeProcess(task->process, &task->result);
                    task->state = task->result == 0 ? "done" : "failed";
                    if (task->result == 0 && task->command.find(L"winget install ") != std::wstring::npos)
                    {
                        std::string command(task->command.begin(), task->command.end());
                        RegisterInstalledToolPaths(command);
                        RefreshEnvironmentPath();
                    }
                }
                while (running < parallel)
                {
                    std::shared_ptr<Task> next;
                    for (auto& task : tasks)
                        if (task->state == "waiting" && !task->paused && (!next || task->priority > next->priority)) next = task;
                    if (!next) break;
                    Start(*next);
                    if (next->state == "running") ++running;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }

    void Start(Task& task)
    {
        SECURITY_ATTRIBUTES security{sizeof(security), nullptr, TRUE};
        HANDLE log = CreateFileW(task.log.c_str(), GENERIC_WRITE, FILE_SHARE_READ, &security, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
        HANDLE input = CreateFileW(L"NUL", GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, &security, OPEN_EXISTING, 0, nullptr);
        task.job = CreateJobObjectW(nullptr, nullptr);
        JOBOBJECT_EXTENDED_LIMIT_INFORMATION limits{};
        limits.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
        bool ready = log != INVALID_HANDLE_VALUE && input != INVALID_HANDLE_VALUE && task.job &&
            SetInformationJobObject(task.job, JobObjectExtendedLimitInformation, &limits, sizeof(limits));
        STARTUPINFOW startup{};
        startup.cb = sizeof(startup);
        startup.dwFlags = STARTF_USESTDHANDLES;
        startup.hStdInput = input;
        startup.hStdOutput = startup.hStdError = log;
        PROCESS_INFORMATION process{};
        std::wstring command = task.command;
        bool created = ready && CreateProcessW(nullptr, command.data(), nullptr, nullptr, TRUE,
            CREATE_NO_WINDOW | CREATE_SUSPENDED | Priority(task.priority), nullptr, nullptr, &startup, &process);
        if (created && AssignProcessToJobObject(task.job, process.hProcess))
        {
            task.process = process.hProcess;
            task.state = "running";
            ResumeThread(process.hThread);
        }
        else
        {
            task.result = GetLastError();
            task.state = "failed";
            if (created) { TerminateProcess(process.hProcess, ERROR_CANCELLED); CloseHandle(process.hProcess); }
        }
        if (process.hThread) CloseHandle(process.hThread);
        if (log != INVALID_HANDLE_VALUE) CloseHandle(log);
        if (input != INVALID_HANDLE_VALUE) CloseHandle(input);
    }
};

TaskQueue& GetTaskQueue() { static TaskQueue queue; return queue; }

void TaskQueueMenu()
{
    auto& queue = GetTaskQueue();
    while (true)
    {
        {
            std::lock_guard<std::mutex> lock(queue.mutex);
            std::cout << "\n=== 后台任务队列 ===\n退出工具箱将结束未完成的队列任务。\n后台提交: " << (queue.enabled ? "开" : "关") << "  并发: " << queue.parallel << "\n";
            for (const auto& task : queue.tasks)
                std::cout << task->id << ". " << task->state << (task->paused ? " [暂停]" : "")
                          << " 优先级=" << task->priority << " 返回码=" << task->result << "\n";
        }
        std::cout << "1. 切换后台提交  2. 暂停  3. 继续  4. 优先级  5. 日志  6. 并发数  0. 返回\n";
        std::string line;
        if (!std::getline(std::cin, line) || line == "0") return;
        if (line == "1") { queue.enabled = !queue.enabled; continue; }
        if (line == "6")
        {
            std::cout << "并发数（1-8）: ";
            std::getline(std::cin, line);
            if (line.size() == 1 && line[0] >= '1' && line[0] <= '8')
            { std::lock_guard<std::mutex> lock(queue.mutex); queue.parallel = line[0] - '0'; }
            continue;
        }
        const std::string action = line;
        std::cout << "任务 ID: ";
        if (!std::getline(std::cin, line)) return;
        int id = 0;
        try { size_t end; id = std::stoi(line, &end); if (end != line.size()) continue; } catch (...) { continue; }
        if (action == "5")
        {
            std::lock_guard<std::mutex> lock(queue.mutex);
            if (id > 0 && id <= static_cast<int>(queue.tasks.size()))
                ShellExecuteW(nullptr, L"open", L"notepad.exe", (L"\"" + queue.tasks[id-1]->log + L"\"").c_str(), nullptr, SW_SHOW);
        }
        else if (action == "2" || action == "3" || action == "4")
        {
            int priority = 1;
            if (action == "4")
            {
                std::cout << "优先级 0=低 1=普通 2=高: ";
                std::getline(std::cin, line);
                if (line.size() != 1 || line[0] < '0' || line[0] > '2') continue;
                priority = line[0] - '0';
            }
            if (!queue.Control(id, action == "2" ? 1 : action == "3" ? 2 : 3, priority))
                std::cout << "操作失败或任务已结束。\n";
        }
    }
}
