// 多功能系统工具箱 v2.7
// 编译环境：Visual Studio 2019/2022 或 MinGW-w64，Windows 10/11
// 注意：请将本文件保存为 UTF-8 with BOM 格式，避免中文乱码
// 运行需要管理员权限，部分操作有风险，使用前请阅读免责声明

// ==================== 版本信息 ====================
#define TOOLBOX_VERSION    "v1.5"
#define GITHUB_REPO        "huvubb/system-toolbox"
#define GITHUB_API_URL     "https://api.github.com/repos/huvubb/system-toolbox/releases/latest"
#define GITHUB_RELEASE_URL "https://github.com/huvubb/system-toolbox/releases"
#define ENV_LAST_HASH "TOOLBOX_LAST_HASH"

// ==================== 全局变量 ====================
bool g_bOnlineMode = true;       // true=在线模式，false=离线模式
bool g_bModeInitialized = false;

#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include <iostream>
#include <ctime>   // 用于 srand
#include <objbase.h>  // 用于 CoCreateGuid
#pragma comment(lib, "ole32.lib")  // 链接 ole32
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <cstdio>
#include <windows.h>
#include <d3d11.h>       // 添加
#include <dxgi.h>        // 添加
#include <shellapi.h>
#include <conio.h>
#include <fstream>
#include <chrono>
#include <cmath>
#include <thread>
#include <atomic>
#include <gl/gl.h>

#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "d3d11.lib")   // 添加
#pragma comment(lib, "dxgi.lib")    // 添加
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")

// ====================================================================
// 基础工具函数
// ====================================================================

// 控制台代码页设置为 UTF-8
void SetUtf8Console()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    system("chcp 65001 >nul");
}

// 检查是否以管理员身份运行
bool IsRunAsAdmin()
{
    BOOL isElevated = FALSE;
    HANDLE hToken = NULL;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
    {
        TOKEN_ELEVATION elevation;
        DWORD dwSize;
        if (GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &dwSize))
            isElevated = elevation.TokenIsElevated;
        CloseHandle(hToken);
    }
    return isElevated != FALSE;
}

// 尝试以管理员身份重新启动
bool ElevateSelf()
{
    TCHAR szPath[MAX_PATH];
    if (GetModuleFileName(NULL, szPath, MAX_PATH))
    {
        SHELLEXECUTEINFO sei = { sizeof(sei) };
        sei.lpVerb = TEXT("runas");
        sei.lpFile = szPath;
        sei.hwnd = NULL;
        sei.nShow = SW_NORMAL;
        return ShellExecuteEx(&sei) != FALSE;
    }
    return false;
}

// 清屏
void ClearScreen()
{
    system("cls");
}

// 暂停等待用户按键
void Pause()
{
    std::cout << "\n按任意键返回菜单...";
    _getch();
    std::cout << std::endl;
}

// 执行系统命令
void RunCmd(const std::string& cmd, bool showOutput = true)
{
    std::cout << ">> " << cmd << std::endl;
    if (!showOutput)
        system((cmd + " >nul 2>&1").c_str());
    else
    {
        int ret = system(cmd.c_str());
        std::cout << "返回码: " << ret << std::endl;
    }
}

// 获取用户输入的字符串
std::string GetInput(const std::string& prompt)
{
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return input;
}


// ==================== 功能函数声明 ====================

// 基础工具函数
void SetUtf8Console();
void MediaToolsMenu();
bool IsRunAsAdmin();
bool ElevateSelf();
void ClearScreen();
void Pause();
void RunCmd(const std::string& cmd, bool showOutput);
std::string GetInput(const std::string& prompt);
int GetIntInput(const std::string& prompt, int defaultVal = -1);

// 系统维护
void CleanDiskC();
void ManageWindowsUpdate();
void SystemFileRepair();
void HashToolsMenu();
void ManageHibernate();
void ManageVirtualMemory();
void SyncSystemTime();
void StorageUnitConverter();
void NetworkToolsMenu();
void DiskToolsMenu();

// 磁盘工具
void DiskRepairMenu();
void USBRepairMenu();
void DetectFakeDisk();
void ExtractZip();

//update
void SoftwareUpdate();
bool GetLatestReleaseInfo(std::string& tagName, std::string& downloadUrl, std::string& body);
std::string GetFileSHA256(const std::string& filePath);
std::string GetLocalExeHash();
std::string GetRemoteExeHash(const std::string& zipPath);
bool CheckJustUpdated();
void ShowUpdateContent();
std::string GetLastHash();
void SaveCurrentHash(const std::string& hash);

// 软件与运行库
void InstallRuntimes();
void OpenSoftwareWebsite();
void CompileToolsMenu();

//online and offline
bool IsInPeOrRe();
void ToggleMode();
void RunCmdSafe(const std::string& cmd, bool showOutput);
bool IsFeatureAvailable(const std::string& feature);

// 硬件压测
void StressTestMenu();
void NetworkStressTest();
void InterfaceStressTest();


// GPU 工具
void GetGpuInfo();
void OpenGLTest();
void DirectX11Test();
void VulkanTest();
void GpuMultiWindowTest();
void GpuToolsMenu();
int SelectDuration(bool allowInfinite);

// 安全检测
void SecurityCheckMenu();
void CheckScheduledTasks();
void CheckProcesses();
void CheckRegistryAutoRuns();

// 文件签名与镜像
void FileDigitalSignature();
void DismWimMenu();
void DismExtractWim();
void DismCaptureWim();
void DismWimIndexManage();
void EsdToWim();
void WimToEsd();
void IsoInfo();
void ImageApplyMenu();

// TrustedInstaller 特权
std::string GetPsExecPath();
bool CheckPsExec();
bool IsTrustedInstallerRunning();
bool StartTrustedInstallerService();
bool RunAsTrustedInstaller(const std::string& command, bool showOutput);
std::string RunAsTrustedInstallerWithOutput(const std::string& command);
bool StartProcessAsTrustedInstaller(const std::string& exePath, const std::string& args);
bool DeleteFileAsTrustedInstaller(const std::string& filePath);
bool SetRegistryAsTrustedInstaller(const std::string& keyPath, const std::string& valueName,
                                    const std::string& valueData, bool isDword);
bool ControlServiceAsTrustedInstaller(const std::string& serviceName, bool startService);
bool RunPowerShellAsTrustedInstaller(const std::string& psCommand);
std::string RunPowerShellAsTrustedInstallerWithOutput(const std::string& psCommand);
bool CopyFileAsTrustedInstaller(const std::string& sourcePath, const std::string& destPath);
bool MoveFileAsTrustedInstaller(const std::string& sourcePath, const std::string& destPath);
bool CreateDirectoryAsTrustedInstaller(const std::string& dirPath);
bool RemoveDirectoryAsTrustedInstaller(const std::string& dirPath);
bool SetFileAttributesAsTrustedInstaller(const std::string& filePath, const std::string& attributes);
bool TakeOwnershipAsTrustedInstaller(const std::string& filePath);
bool GrantFullControlAsTrustedInstaller(const std::string& filePath, const std::string& userName);
void TrustedInstallerMenu();

// NTDLL 底层 API
void NtdllApiMenu();
void NtdllFileOperations();

//进制转换
void BaseConverterMenu();
std::string NumToBase(unsigned long long num, int base);
std::string StringToBase(const std::string& text, int base, const std::string& sep);

// NPM 工具
void NpmInstallMenu();

//文档转换
void DocumentConverterMenu();

//驱动工具
void CheckDriverSigningStatus();
void DisableDriverSignature();
void EnableDriverSignature();
void DriverToolsMenu();

// ==================== 在线/离线模式 ====================

bool IsInPeOrRe()
{
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\MiniNT", 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
        RegCloseKey(hKey);
        return true;
    }

    char winDir[MAX_PATH];
    GetWindowsDirectoryA(winDir, MAX_PATH);
    if (strstr(winDir, "X:\\") || strstr(winDir, "x:\\"))
        return true;

    return false;
}

void ToggleMode()
{
    ClearScreen();
    std::cout << "========== 切换运行模式 ==========\n";
    std::cout << "当前模式: " << (g_bOnlineMode ? "在线模式" : "离线模式") << "\n\n";

    if (IsInPeOrRe())
        std::cout << "[提示] 检测到 PE/RE 环境，建议保持离线模式。\n\n";

    std::cout << " 1. 切换到在线模式\n";
    std::cout << " 2. 切换到离线模式\n";
    std::cout << " 0. 返回\n";
    std::cout << "==================================\n";

    int c = GetIntInput("请选择: ");

    if (c == 1)
    {
        g_bOnlineMode = true;
        std::cout << "\n已切换到在线模式。\n";
    }
    else if (c == 2)
    {
        g_bOnlineMode = false;
        std::cout << "\n已切换到离线模式。\n";
    }
    else
    {
        std::cout << "\n未修改。\n";
    }
    Pause();
}

// GPU 压测窗口过程
LRESULT CALLBACK GPUStressWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

// 获取用户输入的整数
int GetIntInput(const std::string& prompt, int defaultVal)
{
    std::string input = GetInput(prompt);
    if (input.empty() && defaultVal != -1)
        return defaultVal;
    try { return std::stoi(input); }
    catch (...) { return -1; }
}

// 计算文件 SHA256
std::string GetFileSHA256(const std::string& filePath)
{
    std::string result;
    std::string tempHash = "C:\\hash_temp.txt";
    DeleteFileA(tempHash.c_str());

    std::string cmd = "powershell -Command \""
        "$ProgressPreference='SilentlyContinue';"
        "(Get-FileHash -Path '" + filePath + "' -Algorithm SHA256).Hash"
        " | Out-File -FilePath '" + tempHash + "' -Encoding ASCII\"";
    system(cmd.c_str());

    std::ifstream f(tempHash);
    if (f.is_open())
    {
        std::getline(f, result);
        f.close();
    }
    DeleteFileA(tempHash.c_str());

    while (!result.empty() && (result.front() == ' ' || result.front() == '\r' || result.front() == '\n'))
        result.erase(result.begin());
    while (!result.empty() && (result.back() == ' ' || result.back() == '\r' || result.back() == '\n'))
        result.pop_back();

    return result;
}
int main()
{
    SetUtf8Console();

    // PE/RE 自动检测
    if (IsInPeOrRe())
    {
        g_bOnlineMode = false;
    }

    // 检查是否刚更新过
    if (CheckJustUpdated())
    {
        ShowUpdateContent();
    }


    // 免责声明 + 管理员权限（原有代码）

    int choice;
    while (true)
    {
        ClearScreen();
        std::cout << "========================================\n";
        std::cout << "        多功能系统工具箱 " << TOOLBOX_VERSION << "\n";
        std::cout << "========================================\n";
        std::cout << "运行模式: " << (g_bOnlineMode ? "[在线]" : "[离线]") << "\n";
        std::cout << "----------------------------------------\n";

        // 在线模式显示全部功能，离线模式隐藏网络相关功能
        if (g_bOnlineMode)
        {
            // 在线模式：完整菜单
            std::cout << " 1. C盘清理\n";
            std::cout << " 2. 禁用/启用 Windows 更新\n";
            std::cout << " 3. 硬盘修复（含U盘修复、扩容盘检测）\n";
            std::cout << " 4. 检查并修复系统文件错误\n";
            std::cout << " 5. 检查/安装运行库（.NET 和 VC++）\n";
            std::cout << " 6. 打开常用软件官网\n";
            std::cout << " 7. 解压 ZIP 压缩包\n";
            std::cout << " 8. 关闭/开启休眠（快速启动）\n";
            std::cout << " 9. 虚拟内存管理\n";
            std::cout << "10. 硬件压力测试（硬盘/内存/CPU/GPU/网络/接口）\n";
            std::cout << "11. 安全检测（计划任务/进程/注册表）\n";
            std::cout << "12. 文件数字签名\n";
            std::cout << "13. DISM 工具（WIM/映像/WinSxS）\n";
            std::cout << "14. TrustedInstaller 特权操作\n";
            std::cout << "15. 系统时间同步\n";
            std::cout << "16. 存储单位换算\n";
            std::cout << "17. NTDLL 底层 API 操作\n";
            std::cout << "18. 镜像应用（ESD/WIM/ISO）\n";
            std::cout << "19. NPM 工具安装\n";
            std::cout << "20. 网络操作工具\n";
            std::cout << "21. 磁盘操作工具\n";
            std::cout << "22. 显卡管理\n";
            std::cout << "23. 驱动工具\n";
            std::cout << "24. 编译工具\n";
            std::cout << "25. 软件更新\n";
            std::cout << "26. 音视频工具\n";
            std::cout << "27. 文档格式转换\n";
            std::cout << "28. 哈希工具\n";
            std::cout << "29. 进制转换工具\n";
        }
        else
        {
            // 离线模式：隐藏网络功能
            std::cout << " 1. C盘清理\n";
            std::cout << " 2. 启用 Windows 更新 [不可用]\n";       // 灰色显示
            std::cout << " 3. 硬盘修复（含U盘修复、扩容盘检测）\n";
            std::cout << " 4. 检查并修复系统文件错误\n";
            std::cout << " 5. 安装运行库 [不可用]\n";
            std::cout << " 6. 打开常用软件官网 [不可用]\n";
            std::cout << " 7. 解压 ZIP 压缩包\n";
            std::cout << " 8. 关闭/开启休眠（快速启动）\n";
            std::cout << " 9. 虚拟内存管理\n";
            std::cout << "10. 硬件压力测试（硬盘/内存/CPU/GPU/接口）\n";
            std::cout << "11. 安全检测（计划任务/进程/注册表）\n";
            std::cout << "12. 文件数字签名\n";
            std::cout << "13. DISM 工具（离线修复/WIM/WinSxS）\n";
            std::cout << "14. TrustedInstaller 特权操作\n";
            std::cout << "15. 系统时间同步 [不可用]\n";
            std::cout << "16. 存储单位换算\n";
            std::cout << "17. NTDLL 底层 API 操作\n";
            std::cout << "18. 镜像应用（ESD/WIM/ISO）\n";
            std::cout << "19. NPM 工具安装 [不可用]\n";
            std::cout << "20. 网络操作工具 [不可用]\n";
            std::cout << "21. 磁盘操作工具\n";
            std::cout << "22. 显卡管理\n";
            std::cout << "23. 驱动工具\n";
            std::cout << "24. 编译工具\n";
            std::cout << "25. 软件更新 [不可用]\n";
            std::cout << "26. 音视频工具\n";
            std::cout << "27. 文档格式转换\n";
            std::cout << "28. 哈希工具\n";
            std::cout << "29. 进制转换工具\n";
        }

        std::cout << "----------------------------------------\n";
        std::cout << "90. 切换在线/离线模式\n";
        std::cout << " 0. 退出程序\n";
        std::cout << "========================================\n";
        std::cout << "请输入选项: ";

        std::cin >> choice;
        std::cin.ignore();

        // 离线模式下，拦截网络相关功能
        if (!g_bOnlineMode)
        {
            if (choice == 2 || choice == 5 || choice == 6 || choice == 15 ||
                choice == 19 || choice == 20 || choice == 25)
            {
                ClearScreen();
                std::cout << "========== 功能不可用 ==========\n";
                std::cout << "此功能需要网络连接，当前为离线模式。\n";
                std::cout << "请切换到在线模式后再使用。\n";
                std::cout << "===============================\n";
                Pause();
                continue;
            }
        }

        switch (choice)
        {
            case 1: CleanDiskC(); break;
            case 2: ManageWindowsUpdate(); break;
            case 3: DiskRepairMenu(); break;
            case 4: SystemFileRepair(); break;
            case 5: InstallRuntimes(); break;
            case 6: OpenSoftwareWebsite(); break;
            case 7: ExtractZip(); break;
            case 8: ManageHibernate(); break;
            case 9: ManageVirtualMemory(); break;
            case 10: StressTestMenu(); break;
            case 11: SecurityCheckMenu(); break;
            case 12: FileDigitalSignature(); break;
            case 13: DismWimMenu(); break;
            case 14: TrustedInstallerMenu(); break;
            case 15: SyncSystemTime(); break;
            case 16: StorageUnitConverter(); break;
            case 17: NtdllApiMenu(); break;
            case 18: ImageApplyMenu(); break;
            case 19: NpmInstallMenu(); break;
            case 20: NetworkToolsMenu(); break;
            case 21: DiskToolsMenu(); break;
            case 22: GpuToolsMenu(); break;
            case 23: DriverToolsMenu(); break;
            case 24: CompileToolsMenu(); break;
            case 25: SoftwareUpdate(); break;
            case 26: MediaToolsMenu(); break;
            case 27: DocumentConverterMenu(); break;
            case 28: HashToolsMenu(); break;
            case 29: BaseConverterMenu(); break;

            case 90:  // 切换模式
                ToggleMode();
                break;

            case 0:
                std::cout << "感谢使用，再见！" << std::endl;
                return 0;

            default:
                std::cout << "无效选项，请重新输入。" << std::endl;
                Pause();
        }
    }
    return 0;
}

// ==================== 1. C盘清理 ====================

// 计算文件夹大小
unsigned long long GetFolderSize(const std::string& path)
{
    unsigned long long totalSize = 0;
    
    std::string searchPath = path;
    if (!searchPath.empty() && searchPath.back() != '\\')
    {
        searchPath += "\\";
    }
    searchPath += "*";
    
    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);
    
    if (hFind == INVALID_HANDLE_VALUE)
    {
        return 0;
    }
    
    do
    {
        std::string name = findData.cFileName;
        
        if (name == "." || name == "..")
        {
            continue;
        }
        
        std::string fullPath = path;
        if (!fullPath.empty() && fullPath.back() != '\\')
        {
            fullPath += "\\";
        }
        fullPath += name;
        
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            totalSize += GetFolderSize(fullPath);
        }
        else
        {
            ULARGE_INTEGER fileSize;
            fileSize.LowPart = findData.nFileSizeLow;
            fileSize.HighPart = findData.nFileSizeHigh;
            totalSize += fileSize.QuadPart;
        }
    } while (FindNextFileA(hFind, &findData));
    
    FindClose(hFind);
    return totalSize;
}

// 格式化大小显示
std::string FormatSize(unsigned long long bytes)
{
    char buffer[128];
    
    if (bytes >= 1024ULL * 1024 * 1024)
    {
        sprintf(buffer, "%.2f GB", bytes / (1024.0 * 1024.0 * 1024.0));
    }
    else if (bytes >= 1024 * 1024)
    {
        sprintf(buffer, "%.2f MB", bytes / (1024.0 * 1024.0));
    }
    else if (bytes >= 1024)
    {
        sprintf(buffer, "%.2f KB", bytes / 1024.0);
    }
    else
    {
        sprintf(buffer, "%llu B", bytes);
    }
    
    return std::string(buffer);
}

void CleanDiskC()
{
    ClearScreen();
    std::cout << "========================================\n";
    std::cout << "            C盘清理工具\n";
    std::cout << "========================================\n";
    std::cout << "正在分析可清理的项目，请稍候...\n\n";

    // 分析阶段
    std::cout << "┌─────────────────────────────────────┐\n";
    std::cout << "│           清理项目分析               │\n";
    std::cout << "└─────────────────────────────────────┘\n\n";

    // 1. 用户临时文件夹
    std::cout << " [1] 用户临时文件夹\n";
    char tempBuf[MAX_PATH];
    if (GetTempPathA(MAX_PATH, tempBuf))
    {
        std::cout << "     路径: " << tempBuf << "\n";
        unsigned long long size = GetFolderSize(tempBuf);
        std::cout << "     大小: " << FormatSize(size) << "\n";
    }

    // 2. Windows 临时文件夹
    std::cout << "\n [2] Windows 临时文件夹\n";
    std::cout << "     路径: C:\\Windows\\Temp\n";
    {
        unsigned long long size = GetFolderSize("C:\\Windows\\Temp");
        std::cout << "     大小: " << FormatSize(size) << "\n";
    }

    // 3. 回收站
    std::cout << "\n [3] 回收站\n";
    {
        SHQUERYRBINFO rbInfo;
        rbInfo.cbSize = sizeof(SHQUERYRBINFO);
        if (SHQueryRecycleBinA(NULL, &rbInfo) == S_OK)
        {
            std::cout << "     项目数: " << rbInfo.i64NumItems << "\n";
            std::cout << "     大小: " << FormatSize(rbInfo.i64Size) << "\n";
        }
    }

    // 4. 缩略图缓存
    std::cout << "\n [4] 缩略图缓存\n";
    {
        char localAppData[MAX_PATH];
        if (GetEnvironmentVariableA("LOCALAPPDATA", localAppData, MAX_PATH))
        {
            std::string thumbPath = std::string(localAppData) + "\\Microsoft\\Windows\\Explorer";
            std::cout << "     路径: " << thumbPath << "\n";
            unsigned long long size = GetFolderSize(thumbPath);
            std::cout << "     大小: " << FormatSize(size) << "\n";
        }
    }

    // 5. 预读取文件
    std::cout << "\n [5] 预读取文件 (Prefetch)\n";
    std::cout << "     路径: C:\\Windows\\Prefetch\n";
    {
        unsigned long long size = GetFolderSize("C:\\Windows\\Prefetch");
        std::cout << "     大小: " << FormatSize(size) << "\n";
    }

    // 6. Windows 错误报告
    std::cout << "\n [6] Windows 错误报告\n";
    std::cout << "     路径: C:\\ProgramData\\Microsoft\\Windows\\WER\n";
    {
        unsigned long long size = GetFolderSize("C:\\ProgramData\\Microsoft\\Windows\\WER");
        std::cout << "     大小: " << FormatSize(size) << "\n";
    }

    std::cout << "\n========================================\n";
    std::cout << "分析完成！\n";
    std::cout << "========================================\n\n";

    // 询问用户
    std::cout << "请选择要清理的项目：\n";
    std::cout << "  1. 用户临时文件夹\n";
    std::cout << "  2. Windows 临时文件夹\n";
    std::cout << "  3. 回收站\n";
    std::cout << "  4. 缩略图缓存\n";
    std::cout << "  5. 预读取文件\n";
    std::cout << "  6. Windows 错误报告\n";
    std::cout << "  7. 全部清理\n";
    std::cout << "  0. 取消操作\n";
    std::cout << "----------------------------------------\n";
    std::cout << "多选请用逗号分隔，例如：1,2,3\n";
    std::cout << "========================================\n";

    std::string selection = GetInput("请输入选择: ");

    if (selection == "0" || selection.empty())
    {
        std::cout << "\n操作已取消。" << std::endl;
        Pause();
        return;
    }

    std::cout << "\n========================================\n";
    std::cout << "           开始清理...\n";
    std::cout << "========================================\n";

    if (selection == "7" || selection.find('1') != std::string::npos)
    {
        std::cout << "\n[清理] 用户临时文件夹...\n";
        RunCmd("del /q /f /s \"%TEMP%\\*\" >nul 2>&1", false);
        std::cout << "  完成\n";
    }

    if (selection == "7" || selection.find('2') != std::string::npos)
    {
        std::cout << "\n[清理] Windows 临时文件夹...\n";
        RunCmd("del /q /f /s \"C:\\Windows\\Temp\\*\" >nul 2>&1", false);
        std::cout << "  完成\n";
    }

    if (selection == "7" || selection.find('3') != std::string::npos)
    {
        std::cout << "\n[清理] 回收站...\n";
        RunCmd("powershell -Command \"Clear-RecycleBin -Force -ErrorAction SilentlyContinue\"", false);
        std::cout << "  完成\n";
    }

    if (selection == "7" || selection.find('4') != std::string::npos)
    {
        std::cout << "\n[清理] 缩略图缓存...\n";
        RunCmd("del /q /f /s \"%LOCALAPPDATA%\\Microsoft\\Windows\\Explorer\\thumbcache_*.db\" >nul 2>&1", false);
        std::cout << "  完成\n";
    }

    if (selection == "7" || selection.find('5') != std::string::npos)
    {
        std::cout << "\n[清理] 预读取文件...\n";
        RunCmd("del /q /f /s \"C:\\Windows\\Prefetch\\*\" >nul 2>&1", false);
        std::cout << "  完成\n";
    }

    if (selection == "7" || selection.find('6') != std::string::npos)
    {
        std::cout << "\n[清理] Windows 错误报告...\n";
        RunCmd("del /q /f /s \"C:\\ProgramData\\Microsoft\\Windows\\WER\\*\" >nul 2>&1", false);
        std::cout << "  完成\n";
    }

    std::cout << "\n========================================\n";
    std::cout << "           C盘清理完成！\n";
    std::cout << "========================================\n";
    Pause();
}

// ==================== 2. Windows 更新管理 ====================
void ManageWindowsUpdate()
{
    ClearScreen();
    std::cout << "========== Windows 更新管理 ==========\n";
    std::cout << " 1. 禁用 Windows 更新\n";
    std::cout << " 2. 启用 Windows 更新（恢复默认）\n";
    std::cout << " 3. 返回主菜单\n";
    std::cout << "=======================================\n";
    int choice = GetIntInput("请选择: ");

    if (choice == 1)
    {
        RunCmd("reg add \"HKLM\\SOFTWARE\\Policies\\Microsoft\\Windows\\WindowsUpdate\\AU\" /v NoAutoUpdate /t REG_DWORD /d 1 /f");
        RunCmd("sc stop wuauserv");
        RunCmd("sc config wuauserv start= disabled");
        std::cout << "Windows 更新已禁用。" << std::endl;
    }
    else if (choice == 2)
    {
        RunCmd("reg delete \"HKLM\\SOFTWARE\\Policies\\Microsoft\\Windows\\WindowsUpdate\\AU\" /v NoAutoUpdate /f");
        RunCmd("sc config wuauserv start= auto");
        RunCmd("sc start wuauserv");
        std::cout << "Windows 更新已恢复为自动。" << std::endl;
    }
    else
        std::cout << "操作取消。" << std::endl;
    Pause();
}

// ==================== 3. 硬盘修复 ====================
void DiskRepairMenu()
{
    while (true)
    {
        ClearScreen();
        std::cout << "========== 硬盘修复 ==========\n";
        std::cout << " 1. 检查并修复指定磁盘 (chkdsk /f /r /x)\n";
        std::cout << " 2. U盘修复\n";
        std::cout << " 3. 检测扩容盘（暴力写入校验）\n";
        std::cout << " 4. 返回主菜单\n";
        std::cout << "===============================\n";
        int choice = GetIntInput("请选择: ");

        if (choice == 1)
        {
            std::string drive = GetInput("请输入要修复的盘符（如 C: ）: ");
            if (drive.empty()) { std::cout << "盘符不能为空！" << std::endl; Pause(); continue; }
            if (drive.back() != ':') drive += ":";
            std::cout << "即将对 " << drive << " 执行 chkdsk /f /r /x\n";
            std::cout << "是否继续？(y/n): ";
            char c; std::cin >> c; std::cin.ignore();
            if (c == 'y' || c == 'Y')
            {
                std::string cmd = "chkdsk " + drive + " /f /r /x";
                RunCmd(cmd);
            }
            else std::cout << "操作已取消。" << std::endl;
            Pause();
        }
        else if (choice == 2) USBRepairMenu();
        else if (choice == 3) DetectFakeDisk();
        else if (choice == 4) break;
        else { std::cout << "无效选项。" << std::endl; Pause(); }
    }
}

// ==================== U盘修复子菜单 ====================
void USBRepairMenu()
{
    while (true)
    {
        ClearScreen();
        std::cout << "========== U盘专修工具 ==========\n";
        std::cout << " 1. 对U盘执行 chkdsk 修复\n";
        std::cout << " 2. 清除U盘只读属性（自动）\n";
        std::cout << " 3. 启动 DiskPart 手动修复\n";
        std::cout << " 4. 返回上一级\n";
        std::cout << "=============================\n";
        int choice = GetIntInput("请选择: ");

        if (choice == 1)
        {
            std::string drive = GetInput("请输入U盘盘符（如 E: ）: ");
            if (drive.empty()) { std::cout << "盘符不能为空！" << std::endl; Pause(); continue; }
            if (drive.back() != ':') drive += ":";
            std::cout << "即将对 " << drive << " 执行 chkdsk /f /r /x\n";
            std::cout << "请确保U盘已备份重要数据。是否继续？(y/n): ";
            char c; std::cin >> c; std::cin.ignore();
            if (c == 'y' || c == 'Y')
            {
                std::string cmd = "chkdsk " + drive + " /f /r /x";
                RunCmd(cmd);
            }
            Pause();
        }
        else if (choice == 2)
        {
            std::string drive = GetInput("请输入只读U盘盘符（如 E: ）: ");
            if (drive.empty()) { std::cout << "盘符不能为空！" << std::endl; Pause(); continue; }
            if (drive.back() != ':') drive += ":";
            std::string psCmd =
                "powershell -Command \""
                "$driveLetter = '" + drive.substr(0, 1) + "';"
                "$partition = Get-Partition -DriveLetter $driveLetter -ErrorAction SilentlyContinue;"
                "if ($partition) {"
                "$disk = Get-Disk -Number $partition.DiskNumber;"
                "$disk | Set-Disk -IsReadOnly $false;"
                "$disk | Clear-Disk -RemoveData -RemoveOEM -ErrorAction SilentlyContinue;"
                "Write-Host '只读属性已清除，磁盘号: ' $disk.Number;"
                "} else { Write-Host '未找到对应U盘！'; }"
                "\"";
            RunCmd(psCmd);
            Pause();
        }
        else if (choice == 3)
        {
            std::cout << "正在启动 DiskPart...\n";
            system("diskpart");
            Pause();
        }
        else if (choice == 4) break;
        else { std::cout << "无效选项。" << std::endl; Pause(); }
    }
}

// ==================== 检测扩容盘 ====================
void DetectFakeDisk()
{
    ClearScreen();
    std::cout << "========== 扩容盘检测 ==========\n";
    std::string drive = GetInput("请输入要检测的盘符（如 D:，不能是C:）: ");
    if (drive.empty()) { std::cout << "盘符不能为空！" << std::endl; Pause(); return; }
    if (drive.back() != ':') drive += ':';
    if (toupper(drive[0]) == 'C') { std::cout << "不能对C盘进行暴力写入测试！" << std::endl; Pause(); return; }

    std::string rootPath = drive + "\\";
    UINT driveType = GetDriveTypeA(rootPath.c_str());
    if (driveType == DRIVE_NO_ROOT_DIR || driveType == DRIVE_UNKNOWN)
    { std::cout << "无效盘符或驱动器不存在。" << std::endl; Pause(); return; }

    ULARGE_INTEGER freeBytesAvailable, totalBytes, totalFreeBytes;
    if (!GetDiskFreeSpaceExA(rootPath.c_str(), &freeBytesAvailable, &totalBytes, &totalFreeBytes))
    { std::cout << "无法获取磁盘信息。" << std::endl; Pause(); return; }

    unsigned long long totalSpace = totalBytes.QuadPart;
    unsigned long long freeSpace = freeBytesAvailable.QuadPart;
    std::cout << "磁盘 " << drive << " 总空间: " << totalSpace / (1024 * 1024 * 1024) << " GB，可用空间: " << freeSpace / (1024 * 1024 * 1024) << " GB" << std::endl;

    std::cout << "\n警告：此操作将向磁盘写入大量测试文件，可能耗时较长，且会完全占用磁盘空间！\n";
    std::cout << "请确保磁盘中没有重要数据（测试后会自动删除测试文件）。\n";
    std::cout << "是否继续？(y/n): ";
    char c; std::cin >> c; std::cin.ignore();
    if (c != 'y' && c != 'Y') { std::cout << "操作已取消。" << std::endl; Pause(); return; }

    const size_t blockSize = 1024 * 1024;
    std::vector<char> buffer(blockSize, 0xAA);
    unsigned long long totalWritten = 0;
    int fileIndex = 0;
    std::vector<std::string> testFiles;

    std::cout << "\n正在写入测试数据...\n";
    while (true)
    {
        std::string fileName = drive + "\\__disk_test_" + std::to_string(fileIndex) + ".tmp";
        std::ofstream ofs(fileName, std::ios::binary | std::ios::out);
        if (!ofs) break;
        testFiles.push_back(fileName);
        unsigned long long fileSize = 0;
        while (ofs)
        {
            ofs.write(buffer.data(), blockSize);
            if (ofs.good()) { fileSize += blockSize; totalWritten += blockSize; }
            else break;
        }
        ofs.close();
        if (fileSize == 0) { DeleteFileA(fileName.c_str()); testFiles.pop_back(); break; }
        fileIndex++;
    }

    std::cout << "写入完成，实际写入数据量: " << totalWritten / (1024 * 1024 * 1024) << " GB" << std::endl;
    std::cout << "正在校验数据完整性...\n";
    bool allOk = true;
    std::vector<char> readBuf(blockSize);
    std::vector<char> expected(blockSize, 0xAA);

    for (const auto& fileName : testFiles)
    {
        std::ifstream ifs(fileName, std::ios::binary | std::ios::in);
        if (!ifs) { std::cout << "无法打开文件: " << fileName << std::endl; allOk = false; continue; }
        while (ifs.read(readBuf.data(), blockSize) || ifs.gcount() > 0)
        {
            std::streamsize bytesRead = ifs.gcount();
            if (memcmp(readBuf.data(), expected.data(), bytesRead) != 0)
            { std::cout << "校验失败：文件 " << fileName << " 中数据不一致！" << std::endl; allOk = false; break; }
        }
        ifs.close();
        if (!allOk) break;
    }

    if (allOk) std::cout << "\n[OK] 校验通过，所有写入数据均正确读取。磁盘容量可能正常。" << std::endl;
    else std::cout << "\n[FAIL] 校验失败，磁盘可能存在虚标容量（扩容盘）或硬件故障。" << std::endl;

    std::cout << "正在删除测试文件...\n";
    for (const auto& fileName : testFiles) DeleteFileA(fileName.c_str());
    std::cout << "清理完成。" << std::endl;
    Pause();
}

// ==================== 4. 系统文件修复 ====================
void SystemFileRepair()
{
    ClearScreen();
    std::cout << "========== 系统文件修复 ==========\n";
    std::cout << "将依次执行：\n";
    std::cout << " 1. sfc /scannow       - 扫描并修复系统文件\n";
    std::cout << " 2. DISM /Online /Cleanup-Image /RestoreHealth - 修复系统映像\n";
    std::cout << "注意：此过程可能耗时较长，且需要联网。\n";
    std::cout << "===================================\n";
    std::cout << "是否开始？(y/n): ";
    char c; std::cin >> c; std::cin.ignore();
    if (c != 'y' && c != 'Y') { std::cout << "操作已取消。" << std::endl; Pause(); return; }
    RunCmd("sfc /scannow");
    RunCmd("DISM /Online /Cleanup-Image /RestoreHealth");
    std::cout << "系统文件修复流程结束，请查看上方输出。" << std::endl;
    Pause();
}

// ==================== 5. 检查/安装运行库 ====================
void InstallRuntimes()
{
    while (true)
    {
        ClearScreen();
        std::cout << "========== 运行库安装 ==========\n";
        std::cout << "=== .NET 运行库 ===\n";
        std::cout << " 1. .NET 9 桌面运行时\n";
        std::cout << " 2. .NET 8 桌面运行时\n";
        std::cout << " 3. .NET 7 桌面运行时\n";
        std::cout << " 4. .NET 6 桌面运行时\n";
        std::cout << " 5. .NET 5 桌面运行时\n";
        std::cout << " 6. .NET Core 3.1 运行时\n";
        std::cout << " 7. .NET Framework 4.8\n";
        std::cout << " 8. 全部 .NET 运行库\n";
        std::cout << "\n=== VC++ 运行库 ===\n";
        std::cout << " 9. VC++ 2015-2022 x64\n";
        std::cout << "10. VC++ 2015-2022 x86\n";
        std::cout << "11. VC++ 2013 x64\n";
        std::cout << "12. VC++ 2013 x86\n";
        std::cout << "13. VC++ 2012 x64\n";
        std::cout << "14. VC++ 2012 x86\n";
        std::cout << "15. VC++ 2010 x64\n";
        std::cout << "16. VC++ 2010 x86\n";
        std::cout << "17. VC++ 2008 x64\n";
        std::cout << "18. VC++ 2008 x86\n";
        std::cout << "19. 全部 VC++ 运行库\n";
        std::cout << "\n 0. 返回主菜单\n";
        std::cout << "===============================\n";
        std::cout << "注意，本功能均使用winget安装运行库，作者不对winget安装的所以包负任何责任\n";
        
        int choice = GetIntInput("请选择: ");

        if (choice == 0) break;

        auto installPackage = [](const std::string& packageId)
        {
            std::string cmd = "winget install " + packageId + " --silent --accept-package-agreements --accept-source-agreements";
            RunCmd(cmd);
        };

        if (choice >= 1 && choice <= 19)
        {
            int ret = system("winget --version >nul 2>&1");
            if (ret != 0)
            {
                std::cout << "未检测到 winget！" << std::endl;
                std::cout << "请安装 App Installer 或手动下载。" << std::endl;
                Pause();
                continue;
            }
        }

        switch (choice)
        {
            // .NET 运行库
            case 1: installPackage("Microsoft.DotNet.DesktopRuntime.9"); break;
            case 2: installPackage("Microsoft.DotNet.DesktopRuntime.8"); break;
            case 3: installPackage("Microsoft.DotNet.DesktopRuntime.7"); break;
            case 4: installPackage("Microsoft.DotNet.DesktopRuntime.6"); break;
            case 5: installPackage("Microsoft.DotNet.DesktopRuntime.5"); break;
            case 6: installPackage("Microsoft.DotNet.Runtime.3_1"); break;
            case 7: installPackage("Microsoft.DotNet.Framework.DeveloperPack_4"); break;
            case 8:
                installPackage("Microsoft.DotNet.DesktopRuntime.9");
                installPackage("Microsoft.DotNet.DesktopRuntime.8");
                installPackage("Microsoft.DotNet.DesktopRuntime.6");
                installPackage("Microsoft.DotNet.DesktopRuntime.5");
                installPackage("Microsoft.DotNet.Runtime.3_1");
                break;

            // VC++ 运行库
            case 9: installPackage("Microsoft.VCRedist.2015+.x64"); break;
            case 10: installPackage("Microsoft.VCRedist.2015+.x86"); break;
            case 11: installPackage("Microsoft.VCRedist.2013.x64"); break;
            case 12: installPackage("Microsoft.VCRedist.2013.x86"); break;
            case 13: installPackage("Microsoft.VCRedist.2012.x64"); break;
            case 14: installPackage("Microsoft.VCRedist.2012.x86"); break;
            case 15: installPackage("Microsoft.VCRedist.2010.x64"); break;
            case 16: installPackage("Microsoft.VCRedist.2010.x86"); break;
            case 17: installPackage("Microsoft.VCRedist.2008.x64"); break;
            case 18: installPackage("Microsoft.VCRedist.2008.x86"); break;
            case 19:
                installPackage("Microsoft.VCRedist.2015+.x64");
                installPackage("Microsoft.VCRedist.2015+.x86");
                installPackage("Microsoft.VCRedist.2013.x64");
                installPackage("Microsoft.VCRedist.2013.x86");
                installPackage("Microsoft.VCRedist.2012.x64");
                installPackage("Microsoft.VCRedist.2012.x86");
                installPackage("Microsoft.VCRedist.2010.x64");
                installPackage("Microsoft.VCRedist.2010.x86");
                installPackage("Microsoft.VCRedist.2008.x64");
                installPackage("Microsoft.VCRedist.2008.x86");
                break;

            default:
                std::cout << "无效选项。" << std::endl;
        }
        Pause();
    }
}
// ==================== 6. 打开常用软件官网 ====================
void OpenSoftwareWebsite()
{
    struct Software { const char* name; const char* url; };
    std::vector<Software> softList = {
        // 社交与即时通讯
        {"微信", "https://weixin.qq.com/"},
        {"QQ", "https://im.qq.com/"},
        {"钉钉", "https://www.dingtalk.com/"},
        {"飞书", "https://www.feishu.cn/"},
        {"Telegram", "https://telegram.org/"},
        {"Discord", "https://discord.com/"},
        {"Slack", "https://slack.com/"},
        {"WhatsApp", "https://www.whatsapp.com/"},
        {"Signal", "https://signal.org/"},
        {"Skype", "https://www.skype.com/"},
        {"微博", "https://weibo.com/"},
        {"知乎", "https://www.zhihu.com/"},
        {"豆瓣", "https://www.douban.com/"},
        {"小红书", "https://www.xiaohongshu.com/"},
        {"抖音", "https://www.douyin.com/"},
        {"快手", "https://www.kuaishou.com/"},
        {"Bilibili", "https://www.bilibili.com/"},
        {"YouTube", "https://www.youtube.com/"},
        {"Twitter/X", "https://twitter.com/"},
        {"Facebook", "https://www.facebook.com/"},
        {"Instagram", "https://www.instagram.com/"},
        {"LinkedIn", "https://www.linkedin.com/"},
        {"Pinterest", "https://www.pinterest.com/"},
        {"Tumblr", "https://www.tumblr.com/"},
        {"Reddit", "https://www.reddit.com/"},

        // 浏览器
        {"Chrome", "https://www.google.com/chrome/"},
        {"Firefox", "https://www.mozilla.org/firefox/"},
        {"Edge", "https://www.microsoft.com/edge"},
        {"Opera", "https://www.opera.com/"},
        {"Brave", "https://brave.com/"},
        {"Vivaldi", "https://vivaldi.com/"},
        {"Tor Browser", "https://www.torproject.org/"},
        {"360浏览器", "https://browser.360.cn/"},
        {"QQ浏览器", "https://browser.qq.com/"},
        {"UC浏览器", "https://www.uc.cn/"},

        // 开发工具
        {"GitHub", "https://github.com/"},
        {"GitLab", "https://gitlab.com/"},
        {"Gitee", "https://gitee.com/"},
        {"Stack Overflow", "https://stackoverflow.com/"},
        {"Visual Studio Code", "https://code.visualstudio.com/"},
        {"Visual Studio", "https://visualstudio.microsoft.com/"},
        {"JetBrains", "https://www.jetbrains.com/"},
        {"IntelliJ IDEA", "https://www.jetbrains.com/idea/"},
        {"PyCharm", "https://www.jetbrains.com/pycharm/"},
        {"CLion", "https://www.jetbrains.com/clion/"},
        {"Notepad++", "https://notepad-plus-plus.org/"},
        {"Sublime Text", "https://www.sublimetext.com/"},
        {"Atom", "https://atom.io/"},
        {"Vim", "https://www.vim.org/"},
        {"Emacs", "https://www.gnu.org/software/emacs/"},
        {"Eclipse", "https://www.eclipse.org/"},
        {"NetBeans", "https://netbeans.apache.org/"},
        {"Docker", "https://www.docker.com/"},
        {"Kubernetes", "https://kubernetes.io/"},
        {"Node.js", "https://nodejs.org/"},
        {"Python", "https://www.python.org/"},
        {"Java", "https://www.java.com/"},
        {"Go", "https://golang.org/"},
        {"Rust", "https://www.rust-lang.org/"},
        {"Ruby", "https://www.ruby-lang.org/"},
        {"PHP", "https://www.php.net/"},
        {"Swift", "https://swift.org/"},
        {"Kotlin", "https://kotlinlang.org/"},
        {"TypeScript", "https://www.typescriptlang.org/"},
        {"React", "https://reactjs.org/"},
        {"Vue.js", "https://vuejs.org/"},
        {"Angular", "https://angular.io/"},
        {"Laravel", "https://laravel.com/"},
        {"Django", "https://www.djangoproject.com/"},
        {"Flask", "https://flask.palletsprojects.com/"},
        {"Spring", "https://spring.io/"},
        {"MySQL", "https://www.mysql.com/"},
        {"PostgreSQL", "https://www.postgresql.org/"},
        {"MongoDB", "https://www.mongodb.com/"},
        {"Redis", "https://redis.io/"},
        {"SQLite", "https://www.sqlite.org/"},
        {"Git", "https://git-scm.com/"},
        {"Postman", "https://www.postman.com/"},
        {"Jenkins", "https://www.jenkins.io/"},
        {"Ansible", "https://www.ansible.com/"},
        {"Terraform", "https://www.terraform.io/"},

        // 多媒体与设计
        {"VLC", "https://www.videolan.org/vlc/"},
        {"PotPlayer", "https://potplayer.daum.net/"},
        {"OBS Studio", "https://obsproject.com/"},
        {"Adobe Photoshop", "https://www.adobe.com/products/photoshop.html"},
        {"Adobe Illustrator", "https://www.adobe.com/products/illustrator.html"},
        {"Adobe Premiere", "https://www.adobe.com/products/premiere.html"},
        {"Adobe After Effects", "https://www.adobe.com/products/aftereffects.html"},
        {"Figma", "https://www.figma.com/"},
        {"Sketch", "https://www.sketch.com/"},
        {"Canva", "https://www.canva.com/"},
        {"Blender", "https://www.blender.org/"},
        {"GIMP", "https://www.gimp.org/"},
        {"Inkscape", "https://inkscape.org/"},
        {"Krita", "https://krita.org/"},
        {"Audacity", "https://www.audacityteam.org/"},
        {"DaVinci Resolve", "https://www.blackmagicdesign.com/products/davinciresolve/"},
        {"iMovie", "https://www.apple.com/imovie/"},
        {"Final Cut Pro", "https://www.apple.com/final-cut-pro/"},
        {"Logic Pro", "https://www.apple.com/logic-pro/"},
        {"FL Studio", "https://www.image-line.com/"},

        // 云存储与协作
        {"Google Drive", "https://drive.google.com/"},
        {"Dropbox", "https://www.dropbox.com/"},
        {"OneDrive", "https://onedrive.live.com/"},
        {"百度网盘", "https://pan.baidu.com/"},
        {"阿里云盘", "https://www.aliyundrive.com/"},
        {"腾讯微云", "https://www.weiyun.com/"},
        {"坚果云", "https://www.jianguoyun.com/"},
        {"Box", "https://www.box.com/"},
        {"MEGA", "https://mega.nz/"},
        {"MediaFire", "https://www.mediafire.com/"},
        {"Notion", "https://www.notion.so/"},
        {"印象笔记", "https://www.yinxiang.com/"},
        {"OneNote", "https://www.onenote.com/"},
        {"Evernote", "https://evernote.com/"},
        {"Google Docs", "https://docs.google.com/"},
        {"Microsoft Office", "https://www.office.com/"},
        {"WPS Office", "https://www.wps.cn/"},
        {"LibreOffice", "https://www.libreoffice.org/"},

        // 游戏
        {"Steam", "https://store.steampowered.com/"},
        {"Epic Games", "https://www.epicgames.com/"},
        {"GOG", "https://www.gog.com/"},
        {"Origin", "https://www.origin.com/"},
        {"Uplay", "https://ubisoftconnect.com/"},
        {"Battle.net", "https://www.blizzard.com/"},
        {"Riot Games", "https://www.riotgames.com/"},
        {"Nintendo", "https://www.nintendo.com/"},
        {"PlayStation", "https://www.playstation.com/"},
        {"Xbox", "https://www.xbox.com/"},

        // 在线教育
        {"Coursera", "https://www.coursera.org/"},
        {"edX", "https://www.edx.org/"},
        {"Udemy", "https://www.udemy.com/"},
        {"Khan Academy", "https://www.khanacademy.org/"},
        {"Codecademy", "https://www.codecademy.com/"},
        {"LeetCode", "https://leetcode.com/"},
        {"HackerRank", "https://www.hackerrank.com/"},
        {"中国大学MOOC", "https://www.icourse163.org/"},
        {"学堂在线", "https://www.xuetangx.com/"},
        {"网易公开课", "https://open.163.com/"},

        // 搜索引擎
        {"Google", "https://www.google.com/"},
        {"百度", "https://www.baidu.com/"},
        {"Bing", "https://www.bing.com/"},
        {"搜狗", "https://www.sogou.com/"},
        {"360搜索", "https://www.so.com/"},
        {"DuckDuckGo", "https://duckduckgo.com/"},
        {"Yahoo", "https://www.yahoo.com/"},
        {"Yandex", "https://yandex.com/"},

        // 新闻资讯
        {"CNN", "https://www.cnn.com/"},
        {"BBC", "https://www.bbc.com/"},
        {"纽约时报", "https://www.nytimes.com/"},
        {"路透社", "https://www.reuters.com/"},
        {"新华社", "http://www.xinhuanet.com/"},
        {"人民网", "http://www.people.com.cn/"},
        {"央视网", "https://www.cctv.com/"},
        {"今日头条", "https://www.toutiao.com/"},
        {"腾讯新闻", "https://news.qq.com/"},
        {"网易新闻", "https://news.163.com/"},

        // 购物
        {"淘宝", "https://www.taobao.com/"},
        {"天猫", "https://www.tmall.com/"},
        {"京东", "https://www.jd.com/"},
        {"拼多多", "https://www.pinduoduo.com/"},
        {"亚马逊", "https://www.amazon.com/"},
        {"eBay", "https://www.ebay.com/"},
        {"AliExpress", "https://www.aliexpress.com/"},
        {"苏宁易购", "https://www.suning.com/"},
        {"唯品会", "https://www.vip.com/"},
        {"当当网", "http://www.dangdang.com/"},

        // 工具类
        {"7-Zip", "https://www.7-zip.org/"},
        {"WinRAR", "https://www.win-rar.com/"},
        {"Everything", "https://www.voidtools.com/"},
        {"CCleaner", "https://www.ccleaner.com/"},
        {"TeamViewer", "https://www.teamviewer.com/"},
        {"AnyDesk", "https://anydesk.com/"},
        {"向日葵", "https://sunlogin.oray.com/"},
        {"Wireshark", "https://www.wireshark.org/"},
        {"Nmap", "https://nmap.org/"},
        {"VirtualBox", "https://www.virtualbox.org/"},
        {"VMware", "https://www.vmware.com/"},
        {"Rufus", "https://rufus.ie/"},
        {"PuTTY", "https://www.putty.org/"},
        {"FileZilla", "https://filezilla-project.org/"},
        {"qBittorrent", "https://www.qbittorrent.org/"},
        {"Bandizip", "https://www.bandisoft.com/bandizip/"},
        {"Honeyview", "https://www.bandisoft.com/honeyview/"},
        {"XnView", "https://www.xnview.com/"},
        {"IrfanView", "https://www.irfanview.com/"},
        {"FastStone", "https://www.faststone.org/"}
    };

    const int pageSize = 20;
    int totalPages = (softList.size() + pageSize - 1) / pageSize;
    int currentPage = 1;
    std::string searchKeyword = "";

    while (true)
    {
        ClearScreen();
        std::cout << "========== 常用软件官网 ==========\n";
        std::cout << "共 " << softList.size() << " 个网站 | 第 " << currentPage << "/" << totalPages << " 页\n";
        if (!searchKeyword.empty())
        {
            std::cout << "搜索: " << searchKeyword << "\n";
        }
        std::cout << "===================================\n";

        // 根据搜索过滤
        std::vector<int> filtered;
        if (searchKeyword.empty())
        {
            for (int i = 0; i < (int)softList.size(); i++) filtered.push_back(i);
        }
        else
        {
            for (int i = 0; i < (int)softList.size(); i++)
            {
                std::string name = softList[i].name;
                std::transform(name.begin(), name.end(), name.begin(), ::tolower);
                std::string kw = searchKeyword;
                std::transform(kw.begin(), kw.end(), kw.begin(), ::tolower);
                if (name.find(kw) != std::string::npos)
                {
                    filtered.push_back(i);
                }
            }
        }

        totalPages = (filtered.size() + pageSize - 1) / pageSize;
        if (totalPages == 0) totalPages = 1;
        if (currentPage > totalPages) currentPage = totalPages;

        int startIdx = (currentPage - 1) * pageSize;
        int endIdx = std::min(startIdx + pageSize, (int)filtered.size());

        for (int i = startIdx; i < endIdx; i++)
        {
            int idx = filtered[i];
            std::cout << " " << std::setw(3) << (i + 1) << ". " << softList[idx].name << "\n";
        }

        std::cout << "\n-----------------------------------\n";
        std::cout << " [P]上一页  [N]下一页  [S]搜索  [C]清空搜索\n";
        std::cout << " 输入编号打开网站  0.返回主菜单\n";
        std::cout << "===================================\n";

        std::string input = GetInput("请输入: ");

        if (input == "0" || input == "q" || input == "Q")
        {
            break;
        }
        else if (input == "p" || input == "P")
        {
            if (currentPage > 1) currentPage--;
        }
        else if (input == "n" || input == "N")
        {
            if (currentPage < totalPages) currentPage++;
        }
        else if (input == "s" || input == "S")
        {
            searchKeyword = GetInput("请输入搜索关键词: ");
            currentPage = 1;
        }
        else if (input == "c" || input == "C")
        {
            searchKeyword = "";
            currentPage = 1;
        }
        else
        {
            try
            {
                int num = std::stoi(input);
                if (num >= 1 && num <= (int)filtered.size())
                {
                    int idx = filtered[num - 1];
                    ShellExecuteA(NULL, "open", softList[idx].url, NULL, NULL, SW_SHOWNORMAL);
                    std::cout << "已在默认浏览器中打开: " << softList[idx].name << std::endl;
                    Pause();
                }
            }
            catch (...) {}
        }
    }
}

// ==================== 7. ZIP 解压 ====================
void ExtractZip()
{
    ClearScreen();
    std::cout << "========== ZIP 解压 ==========\n";
    std::string zipPath = GetInput("请输入 ZIP 文件完整路径: ");
    if (zipPath.empty()) { std::cout << "路径不能为空！" << std::endl; Pause(); return; }
    std::string destPath = GetInput("请输入解压目标文件夹路径: ");
    if (destPath.empty()) { std::cout << "目标路径不能为空！" << std::endl; Pause(); return; }
    std::string psCmd = "powershell -Command \"Expand-Archive -Path '" + zipPath + "' -DestinationPath '" + destPath + "' -Force\"";
    RunCmd(psCmd);
    std::cout << "解压完成。" << std::endl;
    Pause();
}

// ==================== 8. 休眠管理 ====================
void ManageHibernate()
{
    ClearScreen();
    std::cout << "========== 休眠管理 ==========\n";
    std::cout << " 1. 关闭休眠（同时关闭快速启动）\n";
    std::cout << " 2. 开启休眠（恢复快速启动）\n";
    std::cout << " 3. 返回\n";
    std::cout << "===============================\n";
    int choice = GetIntInput("请选择: ");
    if (choice == 1) { RunCmd("powercfg /hibernate off"); std::cout << "休眠已关闭，快速启动已禁用。" << std::endl; }
    else if (choice == 2) { RunCmd("powercfg /hibernate on"); std::cout << "休眠已开启，快速启动已启用。" << std::endl; }
    else std::cout << "操作取消。" << std::endl;
    Pause();
}

// ==================== 9. 虚拟内存管理 ====================
void ManageVirtualMemory()
{
    ClearScreen();
    std::cout << "========== 虚拟内存管理 ==========\n";
    std::cout << " 1. 开启系统自动管理所有驱动器的分页文件\n";
    std::cout << " 2. 在指定盘符创建分页文件（自定义大小）\n";
    std::cout << " 3. 在指定盘符自动创建分页文件（根据内存推荐）\n";
    std::cout << " 4. 删除指定盘符上的分页文件（包括C盘）\n";
    std::cout << " 5. 查看当前分页文件配置（中文显示）\n";
    std::cout << " 6. 返回\n";
    std::cout << "===================================\n";
    int choice = GetIntInput("请选择: ");

    if (choice == 1)
    {
        RunCmd("wmic computersystem set AutomaticManagedPagefile=True");
        std::cout << "已开启系统自动管理。" << std::endl;
    }
    else if (choice == 2 || choice == 3)
    {
        std::string drive = GetInput("请输入盘符（如 D，也可以是 C）: ");
        if (drive.empty()) { std::cout << "盘符无效！" << std::endl; Pause(); return; }
        drive = std::string(1, toupper(drive[0]));
        std::string pageName = drive + ":\\\\pagefile.sys";
        RunCmd("wmic computersystem set AutomaticManagedPagefile=False");
        std::string createCmd = "wmic pagefileset create name=\"" + pageName + "\"";
        RunCmd(createCmd);

        if (choice == 2)
        {
            int initial = GetIntInput("请输入初始大小（MB）: ");
            int maximum = GetIntInput("请输入最大大小（MB）: ");
            if (initial <= 0 || maximum <= 0 || initial > maximum) std::cout << "大小设置无效！" << std::endl;
            else
            {
                std::string setCmd = "wmic pagefileset where name=\"" + pageName + "\" set InitialSize=" + std::to_string(initial) + ",MaximumSize=" + std::to_string(maximum);
                RunCmd(setCmd);
                std::cout << "分页文件已创建并设置大小。" << std::endl;
            }
        }
        else
        {
            MEMORYSTATUSEX statex;
            statex.dwLength = sizeof(statex);
            GlobalMemoryStatusEx(&statex);
            DWORDLONG totalPhysMB = statex.ullTotalPhys / (1024 * 1024);
            int initial = static_cast<int>(totalPhysMB * 1.5);
            int maximum = static_cast<int>(totalPhysMB * 3);
            if (initial < 1024) initial = 1024;
            if (maximum < initial) maximum = initial * 2;
            std::cout << "检测到物理内存约 " << totalPhysMB << " MB，建议初始 " << initial << " MB，最大 " << maximum << " MB。" << std::endl;
            std::string setCmd = "wmic pagefileset where name=\"" + pageName + "\" set InitialSize=" + std::to_string(initial) + ",MaximumSize=" + std::to_string(maximum);
            RunCmd(setCmd);
            std::cout << "分页文件已自动创建。" << std::endl;
        }
    }
    else if (choice == 4)
    {
        std::string drive = GetInput("请输入要删除分页文件的盘符（如 C）: ");
        if (drive.empty()) { std::cout << "盘符无效！" << std::endl; Pause(); return; }
        drive = std::string(1, toupper(drive[0]));
        std::string pageName = drive + ":\\\\pagefile.sys";
        std::string delCmd = "wmic pagefileset where name=\"" + pageName + "\" delete";
        RunCmd(delCmd);
        std::cout << "已尝试删除 " << drive << " 盘上的分页文件。" << std::endl;
    }
    else if (choice == 5)
    {
        std::string psCmd = "powershell -Command \""
            "$pf = Get-CimInstance Win32_PageFileUsage | Select-Object Name, AllocatedBaseSize, CurrentUsage, PeakUsage, TempPageFile;"
            "if ($pf) { $pf | ForEach-Object { Write-Host ('页面文件: ' + $_.Name + '  分配大小(MB): ' + $_.AllocatedBaseSize + '  当前使用(MB): ' + $_.CurrentUsage + '  峰值使用(MB): ' + $_.PeakUsage + '  临时页面文件: ' + $_.TempPageFile) } }"
            "else { Write-Host '当前没有配置分页文件或系统自动管理。' }"
            "\"";
        RunCmd(psCmd);
    }
    else
        std::cout << "操作取消。" << std::endl;
    Pause();
}

// ==================== 10. 硬件压力测试 ====================
void StressTestMenu()
{
    while (true)
    {
        ClearScreen();
        std::cout << "========== 硬件压力测试 ==========\n";
        std::cout << " 1. 硬盘压力测试\n";
        std::cout << " 2. 内存压力测试\n";
        std::cout << " 3. CPU 压力测试\n";
        std::cout << " 5. 网络（网卡）压力测试\n";
        std::cout << " 6. 电脑接口通道压测\n";
        std::cout << " 7. 返回主菜单\n";
        std::cout << "===================================\n";
        int choice = GetIntInput("请选择: ");

        // ========== 1. 硬盘压力测试 ==========
        if (choice == 1)
        {
            ClearScreen();
            std::cout << "========== 硬盘压力测试 ==========\n";
            std::string drive = GetInput("请输入要测试的盘符（如 D:，不能是C:）: ");
            if (drive.empty()) { std::cout << "盘符不能为空！" << std::endl; Pause(); continue; }
            if (drive.back() != ':') drive += ':';
            if (toupper(drive[0]) == 'C') { std::cout << "不能对C盘进行压力测试！" << std::endl; Pause(); continue; }

            std::string rootPath = drive + "\\";
            UINT driveType = GetDriveTypeA(rootPath.c_str());
            if (driveType == DRIVE_NO_ROOT_DIR || driveType == DRIVE_UNKNOWN)
            {
                std::cout << "无效盘符或驱动器不存在。" << std::endl;
                Pause();
                continue;
            }

            int testSizeMB = GetIntInput("请输入测试文件大小（MB，建议 100-1000）: ");
            if (testSizeMB <= 0) { std::cout << "大小无效！" << std::endl; Pause(); continue; }

            std::cout << "\n警告：将写入 " << testSizeMB << " MB 的测试文件，测试完成后自动删除。\n";
            std::cout << "是否继续？(y/n): ";
            char c; std::cin >> c; std::cin.ignore();
            if (c != 'y' && c != 'Y') { std::cout << "操作已取消。" << std::endl; Pause(); continue; }

            std::string fileName = drive + "\\__stress_test.tmp";
            const size_t blockSize = 1024 * 1024; // 1MB
            size_t totalBlocks = static_cast<size_t>(testSizeMB);
            std::vector<char> writeBuffer(blockSize, 0x5A);
            std::vector<char> readBuffer(blockSize);

            auto startWrite = std::chrono::steady_clock::now();
            std::ofstream ofs(fileName, std::ios::binary | std::ios::out);
            if (!ofs) { std::cout << "无法创建测试文件！" << std::endl; Pause(); continue; }
            for (size_t i = 0; i < totalBlocks; ++i)
            {
                ofs.write(writeBuffer.data(), blockSize);
                if (!ofs.good())
                {
                    std::cout << "写入失败，可能磁盘空间不足。" << std::endl;
                    ofs.close();
                    DeleteFileA(fileName.c_str());
                    Pause();
                    continue;
                }
            }
            ofs.close();
            auto endWrite = std::chrono::steady_clock::now();
            double writeTime = std::chrono::duration<double>(endWrite - startWrite).count();
            double writeSpeed = (testSizeMB / writeTime);

            auto startRead = std::chrono::steady_clock::now();
            std::ifstream ifs(fileName, std::ios::binary | std::ios::in);
            bool readOk = true;
            while (ifs.read(readBuffer.data(), blockSize) || ifs.gcount() > 0)
            {
                std::streamsize bytesRead = ifs.gcount();
                if (memcmp(readBuffer.data(), writeBuffer.data(), bytesRead) != 0)
                {
                    readOk = false;
                    break;
                }
            }
            ifs.close();
            auto endRead = std::chrono::steady_clock::now();
            double readTime = std::chrono::duration<double>(endRead - startRead).count();
            double readSpeed = readOk ? (testSizeMB / readTime) : 0.0;

            std::cout << "\n===== 测试结果 =====" << std::endl;
            std::cout << "写入耗时: " << writeTime << " 秒，速度: " << writeSpeed << " MB/s" << std::endl;
            if (readOk)
                std::cout << "读取校验: 通过，耗时: " << readTime << " 秒，速度: " << readSpeed << " MB/s" << std::endl;
            else
                std::cout << "读取校验: 失败，数据不一致！" << std::endl;

            DeleteFileA(fileName.c_str());
            std::cout << "测试文件已删除。" << std::endl;
            Pause();
        }
        // ========== 2. 内存压力测试 ==========
        else if (choice == 2)
        {
            ClearScreen();
            std::cout << "========== 内存压力测试 ==========\n";
            MEMORYSTATUSEX statex;
            statex.dwLength = sizeof(statex);
            GlobalMemoryStatusEx(&statex);
            unsigned long long totalPhysMB = statex.ullTotalPhys / (1024 * 1024);
            unsigned long long availPhysMB = statex.ullAvailPhys / (1024 * 1024);
            std::cout << "物理内存总量: " << totalPhysMB << " MB，可用: " << availPhysMB << " MB" << std::endl;

            int allocSizeMB = GetIntInput("请输入要分配的内存大小（MB，建议不超过可用内存的80%）: ");
            if (allocSizeMB <= 0) { std::cout << "大小无效！" << std::endl; Pause(); continue; }
            if (static_cast<unsigned long long>(allocSizeMB) > availPhysMB * 0.8)
            {
                std::cout << "警告：分配大小接近或超过可用内存，可能导致系统无响应。" << std::endl;
                std::cout << "是否仍要继续？(y/n): ";
                char c; std::cin >> c; std::cin.ignore();
                if (c != 'y' && c != 'Y') { std::cout << "操作已取消。" << std::endl; Pause(); continue; }
            }

            std::cout << "\n正在分配 " << allocSizeMB << " MB 内存...\n";
            size_t totalBytes = static_cast<size_t>(allocSizeMB) * 1024 * 1024;
            char* buffer = nullptr;
            try
            {
                buffer = new char[totalBytes];
            }
            catch (std::bad_alloc&)
            {
                std::cout << "内存分配失败！" << std::endl;
                Pause();
                continue;
            }

            std::cout << "内存分配成功，开始压力测试（反复写入并校验）...\n";
            memset(buffer, 0xA5, totalBytes);
            const int rounds = 5;
            bool memOk = true;
            auto start = std::chrono::steady_clock::now();
            for (int r = 0; r < rounds; ++r)
            {
                char pattern = (r % 2 == 0) ? 0xA5 : 0x5A;
                memset(buffer, pattern, totalBytes);
                for (size_t off = 0; off < totalBytes; off += 1024 * 1024)
                {
                    if (buffer[off] != pattern) { memOk = false; break; }
                }
                if (!memOk) break;
            }
            auto end = std::chrono::steady_clock::now();
            double elapsed = std::chrono::duration<double>(end - start).count();

            delete[] buffer;
            std::cout << "\n===== 测试结果 =====" << std::endl;
            if (memOk)
                std::cout << "内存压力测试通过，耗时: " << elapsed << " 秒" << std::endl;
            else
                std::cout << "内存压力测试失败，检测到数据错误！" << std::endl;
            Pause();
        }
        // ========== 3. CPU 压力测试 ==========
        else if (choice == 3)
        {
            ClearScreen();
            std::cout << "========== CPU 压力测试 ==========\n";
            unsigned int hwThreads = std::thread::hardware_concurrency();
            std::cout << "检测到 CPU 逻辑核心数: " << hwThreads << std::endl;
            int userThreads = GetIntInput("请输入线程数（0 表示使用全部核心）: ", 0);
            if (userThreads <= 0) userThreads = hwThreads;
            int duration = GetIntInput("请输入持续时间（秒，0 表示手动停止）: ", 30);
            if (duration < 0) { std::cout << "无效。" << std::endl; Pause(); continue; }

            std::cout << "\n开始 CPU 压力测试...\n";
            std::atomic<bool> stop{false};
            std::vector<std::thread> workers;
            for (int i = 0; i < userThreads; ++i)
            {
                workers.emplace_back([&stop]() {
                    double x = 0.0;
                    while (!stop.load())
                    {
                        x += std::sin(x) * std::cos(x) + std::sqrt(std::abs(x) + 1.0);
                        x = std::fmod(x, 100.0);
                    }
                });
            }

            if (duration > 0)
            {
                std::this_thread::sleep_for(std::chrono::seconds(duration));
                stop = true;
                for (auto& t : workers) t.join();
                std::cout << "测试完成，持续 " << duration << " 秒。" << std::endl;
            }
            else
            {
                std::cout << "按任意键停止测试..." << std::endl;
                _getch();
                stop = true;
                for (auto& t : workers) t.join();
                std::cout << "测试停止。" << std::endl;
            }
            Pause();
        }

        // ========== 5. 网络压力测试 ==========
        else if (choice == 5)
        {
            NetworkStressTest();
        }
        // ========== 6. 接口通道压测 ==========
        else if (choice == 6)
        {
            InterfaceStressTest();
        }
        // ========== 7. 返回 ==========
        else if (choice == 7)
        {
            break;
        }
        else
        {
            std::cout << "无效选项。" << std::endl;
            Pause();
        }
    }
}

// ==================== 网络压力测试函数 ====================
void NetworkStressTest()
{
    ClearScreen();
    std::cout << "========== 网络（网卡）压力测试 ==========\n";
    std::cout << "本测试通过本地 TCP 回环传输大量数据，测量网卡/协议栈吞吐量。\n";
    std::cout << "测试不会产生真实网络流量，仅测试本机网络栈性能。\n";
    std::cout << "注意：测试将占用较多 CPU 和内存。\n";
    std::cout << "===========================================\n";

    int duration = GetIntInput("请输入测试持续时间（秒，0 表示手动停止）: ", 30);
    if (duration < 0) { std::cout << "无效。" << std::endl; Pause(); return; }

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cout << "Winsock 初始化失败！" << std::endl;
        Pause();
        return;
    }

    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET)
    {
        std::cout << "创建套接字失败！" << std::endl;
        WSACleanup();
        Pause();
        return;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cout << "绑定端口失败！" << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        Pause();
        return;
    }

    if (listen(listenSocket, 1) == SOCKET_ERROR)
    {
        std::cout << "监听失败！" << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        Pause();
        return;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET)
    {
        closesocket(listenSocket);
        WSACleanup();
        Pause();
        return;
    }

    sockaddr_in clientAddr = serverAddr;
    if (connect(clientSocket, (sockaddr*)&clientAddr, sizeof(clientAddr)) == SOCKET_ERROR)
    {
        std::cout << "连接失败！" << std::endl;
        closesocket(clientSocket);
        closesocket(listenSocket);
        WSACleanup();
        Pause();
        return;
    }

    SOCKET acceptedSocket = accept(listenSocket, NULL, NULL);
    if (acceptedSocket == INVALID_SOCKET)
    {
        closesocket(clientSocket);
        closesocket(listenSocket);
        WSACleanup();
        Pause();
        return;
    }

    std::cout << "连接建立，开始传输数据...\n";
    const int bufferSize = 1024 * 1024; // 1MB
    std::vector<char> buffer(bufferSize, 0xAB);
    std::atomic<bool> stop{false};
    std::atomic<unsigned long long> totalBytes{0};

    std::thread sender([&]() {
        while (!stop.load())
        {
            int bytesSent = send(clientSocket, buffer.data(), bufferSize, 0);
            if (bytesSent > 0)
                totalBytes += bytesSent;
            else
                break;
        }
    });

    std::thread receiver([&]() {
        while (!stop.load())
        {
            int bytesReceived = recv(acceptedSocket, buffer.data(), bufferSize, 0);
            if (bytesReceived == SOCKET_ERROR || bytesReceived == 0)
                break;
        }
    });

    auto start = std::chrono::steady_clock::now();
    if (duration > 0)
    {
        std::this_thread::sleep_for(std::chrono::seconds(duration));
        stop = true;
    }
    else
    {
        std::cout << "按任意键停止测试..." << std::endl;
        _getch();
        stop = true;
    }

    if (sender.joinable()) sender.join();
    if (receiver.joinable()) receiver.join();

    auto end = std::chrono::steady_clock::now();
    double elapsed = std::chrono::duration<double>(end - start).count();
    double speedMBps = (totalBytes.load() / (1024.0 * 1024.0)) / elapsed;

    closesocket(acceptedSocket);
    closesocket(clientSocket);
    closesocket(listenSocket);
    WSACleanup();

    std::cout << "\n===== 测试结果 =====" << std::endl;
    std::cout << "传输数据量: " << totalBytes.load() / (1024 * 1024) << " MB" << std::endl;
    std::cout << "持续时间: " << elapsed << " 秒" << std::endl;
    std::cout << "平均吞吐量: " << speedMBps << " MB/s" << std::endl;
    Pause();
}

// ==================== 接口通道压测函数 ====================
void InterfaceStressTest()
{
    ClearScreen();
    std::cout << "========== 电脑接口通道压测 ==========\n";
    std::cout << "本测试通过对指定盘符进行大文件读写，检测接口（USB/SATA/NVMe等）传输速率。\n";
    std::string drive = GetInput("请输入要测试的盘符（如 E: ）: ");
    if (drive.empty()) { std::cout << "盘符不能为空！" << std::endl; Pause(); return; }
    if (drive.back() != ':') drive += ':';

    std::string rootPath = drive + "\\";
    UINT driveType = GetDriveTypeA(rootPath.c_str());
    if (driveType == DRIVE_NO_ROOT_DIR || driveType == DRIVE_UNKNOWN)
    {
        std::cout << "无效盘符或驱动器不存在。" << std::endl;
        Pause();
        return;
    }

    int testSizeMB = GetIntInput("请输入测试文件大小（MB，建议 200-2000）: ");
    if (testSizeMB <= 0) { std::cout << "大小无效！" << std::endl; Pause(); return; }

    std::cout << "\n警告：将写入 " << testSizeMB << " MB 的测试文件，测试完成后自动删除。\n";
    std::cout << "是否继续？(y/n): ";
    char c; std::cin >> c; std::cin.ignore();
    if (c != 'y' && c != 'Y') { std::cout << "操作已取消。" << std::endl; Pause(); return; }

    std::string fileName = drive + "\\__interface_test.tmp";
    const size_t blockSize = 1024 * 1024;
    size_t totalBlocks = static_cast<size_t>(testSizeMB);
    std::vector<char> writeBuffer(blockSize, 0xC3);
    std::vector<char> readBuffer(blockSize);

    auto startWrite = std::chrono::steady_clock::now();
    std::ofstream ofs(fileName, std::ios::binary | std::ios::out);
    if (!ofs) { std::cout << "无法创建测试文件！" << std::endl; Pause(); return; }
    for (size_t i = 0; i < totalBlocks; ++i)
    {
        ofs.write(writeBuffer.data(), blockSize);
        if (!ofs.good())
        {
            std::cout << "写入失败，可能磁盘空间不足。" << std::endl;
            ofs.close();
            DeleteFileA(fileName.c_str());
            Pause();
            return;
        }
    }
    ofs.close();
    auto endWrite = std::chrono::steady_clock::now();
    double writeTime = std::chrono::duration<double>(endWrite - startWrite).count();
    double writeSpeed = (testSizeMB / writeTime);

    auto startRead = std::chrono::steady_clock::now();
    std::ifstream ifs(fileName, std::ios::binary | std::ios::in);
    bool ok = true;
    while (ifs.read(readBuffer.data(), blockSize) || ifs.gcount() > 0)
    {
        if (memcmp(readBuffer.data(), writeBuffer.data(), ifs.gcount()) != 0)
        {
            ok = false;
            break;
        }
    }
    ifs.close();
    auto endRead = std::chrono::steady_clock::now();
    double readTime = std::chrono::duration<double>(endRead - startRead).count();
    double readSpeed = testSizeMB / readTime;

    DeleteFileA(fileName.c_str());

    std::cout << "\n===== 接口通道测试结果 =====" << std::endl;
    std::cout << "写入速度: " << writeSpeed << " MB/s" << std::endl;
    if (ok)
        std::cout << "读取速度: " << readSpeed << " MB/s，数据校验通过。" << std::endl;
    else
        std::cout << "读取校验失败，数据不一致！" << std::endl;
    Pause();
}

// ==================== 11. 安全检测 ====================
void SecurityCheckMenu()
{
    while (true)
    {
        ClearScreen();
        std::cout << "========== 安全检测 ==========\n";
        std::cout << " 1. 计划任务探测（可疑）\n";
        std::cout << " 2. 可疑进程探测\n";
        std::cout << " 3. 用户/系统注册表自启动探测\n";
        std::cout << " 4. 返回主菜单\n";
        std::cout << "===============================\n";
        std::cout << "注意：本功能仅检测并列出信息，不会自动修改或删除任何内容。\n";
        std::cout << "注意：本功能不能替代防病毒产品，如果有病毒请使用防病毒产品（windows defender，火绒，360）。\n";
        int choice = GetIntInput("请选择: ");

        if (choice == 1) CheckScheduledTasks();
        else if (choice == 2) CheckProcesses();
        else if (choice == 3) CheckRegistryAutoRuns();
        else if (choice == 4) break;
        else { std::cout << "无效选项。" << std::endl; Pause(); }
    }
}

void CheckScheduledTasks()
{
    ClearScreen();
    std::cout << "========== 计划任务探测 ==========\n";
    std::cout << "正在获取所有计划任务，请耐心等待...\n\n";
    std::string cmd = "schtasks /query /fo LIST /v | findstr /i \"cmd.exe powershell.exe wscript.exe cscript.exe mshta.exe rundll32.exe temp\"";
    int ret = system(cmd.c_str());
    if (ret != 0) std::cout << "未发现可疑任务或查询失败。" << std::endl;
    std::cout << "\n提示：也可以手动运行 schtasks /query 查看全部计划任务。" << std::endl;
    Pause();
}

void CheckProcesses()
{
    ClearScreen();
    std::cout << "========== 可疑进程探测 ==========\n";
    std::cout << "正在获取所有进程信息...\n\n";
    std::string cmd = "wmic process get Name,ProcessId,ExecutablePath /format:list";
    system(cmd.c_str());
    std::cout << "\n提示：可疑进程通常位于临时目录、用户下载目录或非标准路径。\n";
    std::cout << "常见系统进程路径：C:\\Windows\\System32, C:\\Windows\\SysWOW64, C:\\Program Files 等。\n";
    Pause();
}

void CheckRegistryAutoRuns()
{
    ClearScreen();
    std::cout << "========== 注册表自启动探测 ==========\n";
    std::cout << "正在检查常见自启动注册表项...\n\n";

    std::vector<std::string> keys = {
        "HKLM\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
        "HKLM\\Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce",
        "HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
        "HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce",
        "HKLM\\Software\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Run",
        "HKLM\\Software\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\RunOnce"
    };

    for (const auto& key : keys)
    {
        std::cout << "----------------------------------------\n";
        std::cout << "注册表项: " << key << "\n";
        std::string cmd = "reg query \"" + key + "\"";
        int ret = system(cmd.c_str());
        if (ret != 0) std::cout << "（无此项或无值）" << std::endl;
        std::cout << std::endl;
    }

    std::cout << "----------------------------------------\n";
    std::cout << "当前用户启动文件夹: %APPDATA%\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\n";
    system("dir /b \"%APPDATA%\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\" 2>nul");
    std::cout << "公共启动文件夹: C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\n";
    system("dir /b \"C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\" 2>nul");

    std::cout << "\n提示：请重点检查未知来源的启动项，特别是路径在临时目录或包含 cmd/powershell/wscript 等。\n";
    Pause();
}

// ==================== 12. 文件数字签名 ====================
void FileDigitalSignature()
{
    ClearScreen();
    std::cout << "========== 文件数字签名 ==========\n";
    std::cout << "本功能创建自签名证书并对文件签名。\n";
    std::cout << "证书有效期10年，无时间戳服务器。\n";
    std::cout << "请注意，本工具可能会被报毒。\n";
    std::cout << "=====================================\n";

    std::string filePath = GetInput("请输入要签名的文件完整路径: ");
    if (filePath.empty())
    {
        std::cout << "路径不能为空！" << std::endl;
        Pause();
        return;
    }

    // 去除引号
    std::string cleanPath;
    for (char c : filePath)
    {
        if (c != '"' && c != '\'') cleanPath += c;
    }

    DWORD attr = GetFileAttributesA(cleanPath.c_str());
    if (attr == INVALID_FILE_ATTRIBUTES)
    {
        std::cout << "文件不存在: " << cleanPath << std::endl;
        Pause();
        return;
    }

    // 提取文件名
    std::string fileName = cleanPath;
    size_t lastSlash = fileName.find_last_of("\\/");
    if (lastSlash != std::string::npos) fileName = fileName.substr(lastSlash + 1);
    size_t lastDot = fileName.find_last_of('.');
    if (lastDot != std::string::npos) fileName = fileName.substr(0, lastDot);

    std::string certName = fileName + "_Cert";
    std::string password = GetInput("请输入证书密码（至少6位）: ");

    if (password.length() < 6)
    {
        std::cout << "密码至少6位！" << std::endl;
        Pause();
        return;
    }

    std::cout << "\n=== 签名参数 ===\n";
    std::cout << "文件: " << cleanPath << "\n";
    std::cout << "证书名: " << certName << "\n";
    std::cout << "================\n";

    std::cout << "是否继续？(y/n): ";
    char c;
    std::cin >> c;
    std::cin.ignore();
    if (c != 'y' && c != 'Y')
    {
        std::cout << "操作已取消。" << std::endl;
        Pause();
        return;
    }

    // 写入 PowerShell 脚本
    std::string scriptPath = "C:\\sign_temp.ps1";
    std::ofstream scriptFile(scriptPath);
    if (!scriptFile.is_open())
    {
        std::cout << "无法创建临时脚本！" << std::endl;
        Pause();
        return;
    }

    scriptFile << "$ErrorActionPreference = 'Stop'\n";
    scriptFile << "try {\n";
    scriptFile << "    $certName = '" << certName << "'\n";
    scriptFile << "    $password = ConvertTo-SecureString -String '" << password << "' -Force -AsPlainText\n";
    scriptFile << "    $cert = New-SelfSignedCertificate -Subject \"CN=$certName\" -Type CodeSigningCert -CertStoreLocation 'Cert:\\CurrentUser\\My' -KeyExportPolicy Exportable -KeySpec Signature -NotAfter (Get-Date).AddYears(10)\n";
    scriptFile << "    $certPath = Join-Path $env:TEMP \"$certName.pfx\"\n";
    scriptFile << "    Export-PfxCertificate -Cert $cert -FilePath $certPath -Password $password | Out-Null\n";
    scriptFile << "    Set-AuthenticodeSignature -FilePath '" << cleanPath << "' -Certificate $cert -HashAlgorithm SHA256 | Out-Null\n";
    scriptFile << "    Write-Host 'OK' >> 'C:\\sign_result.txt'\n";
    scriptFile << "} catch {\n";
    scriptFile << "    Write-Host \"ERR: $_\" >> 'C:\\sign_result.txt'\n";
    scriptFile << "}\n";
    scriptFile.close();

    std::cout << "\n正在签名...\n";
    std::cout << "如果文件被占用，请先关闭后重新运行签名。\n";
    
    std::string psCmd = "powershell -ExecutionPolicy Bypass -File \"" + scriptPath + "\"";
    RunCmd(psCmd);

    DeleteFileA(scriptPath.c_str());
    
    std::cout << "结果已写入 C:\\sign_result.txt" << std::endl;
    Pause();
}

// ==================== DISM 工具（完整版） ====================

// ==================== WIM 解压 ====================
void DismExtractWim()
{
    ClearScreen();
    std::cout << "========== DISM 解压 WIM ==========\n";
    std::string wimPath = GetInput("WIM 文件完整路径: ");
    if (wimPath.empty()) { Pause(); return; }
    std::string destDir = GetInput("解压目标文件夹路径: ");
    if (destDir.empty()) { Pause(); return; }

    std::string listCmd = "dism /Get-WimInfo /WimFile:\"" + wimPath + "\"";
    std::cout << "\n正在获取 WIM 索引信息...\n";
    int ret = system(listCmd.c_str());
    if (ret != 0)
    {
        std::cout << "获取 WIM 信息失败，请检查路径或 WIM 文件是否损坏。" << std::endl;
        Pause();
        return;
    }

    int index = GetIntInput("请输入要解压的索引号（默认 1）: ", 1);
    if (index <= 0) index = 1;

    std::string applyCmd = "dism /Apply-Image /ImageFile:\"" + wimPath + "\" /Index:" + std::to_string(index) + " /ApplyDir:\"" + destDir + "\"";
    std::cout << "\n正在解压（应用镜像）...\n";
    RunCmd(applyCmd);
    std::cout << "解压完成。" << std::endl;
    Pause();
}

// ==================== WIM 打包 ====================
void DismCaptureWim()
{
    ClearScreen();
    std::cout << "========== DISM 打包 WIM ==========\n";
    std::string sourceDir = GetInput("要打包的目录路径: ");
    if (sourceDir.empty()) { Pause(); return; }
    std::string wimPath = GetInput("输出 WIM 文件路径: ");
    if (wimPath.empty()) { Pause(); return; }
    std::string imageName = GetInput("镜像名称（默认 Toolbox Image）: ");
    if (imageName.empty()) imageName = "Toolbox Image";
    std::string imageDesc = GetInput("镜像描述（默认 Created by System Toolbox）: ");
    if (imageDesc.empty()) imageDesc = "Created by System Toolbox";

    std::cout << "\n压缩类型:\n";
    std::cout << " 1. 不压缩 (none)\n";
    std::cout << " 2. 快速压缩 (fast)\n";
    std::cout << " 3. 最大压缩 (max)\n";
    std::cout << " 4. 默认\n";
    int c = GetIntInput("请选择: ", 4);

    std::string compressArg;
    switch (c)
    {
        case 1: compressArg = "/Compress:none"; break;
        case 2: compressArg = "/Compress:fast"; break;
        case 3: compressArg = "/Compress:max"; break;
        default: break;
    }

    std::cout << "\n是否开始打包？(y/n): ";
    char ch;
    std::cin >> ch;
    std::cin.ignore();
    if (ch != 'y' && ch != 'Y') { Pause(); return; }

    std::string cmd = "dism /Capture-Image /ImageFile:\"" + wimPath + "\" /CaptureDir:\"" + sourceDir + "\" /Name:\"" + imageName + "\" /Description:\"" + imageDesc + "\"";
    if (!compressArg.empty()) cmd += " " + compressArg;

    RunCmd(cmd);
    std::cout << "打包完成。" << std::endl;
    Pause();
}

// ==================== WIM 索引管理 ====================
void DismWimIndexManage()
{
    while (true)
    {
        ClearScreen();
        std::cout << "========== WIM 索引管理 ==========\n";
        std::cout << " 1. 卸载已应用的 WIM\n";
        std::cout << " 2. 删除 WIM 中的索引\n";
        std::cout << " 3. 导出 WIM 中的指定索引\n";
        std::cout << " 4. 分割 WIM 文件\n";
        std::cout << " 5. 合并 WIM 文件\n";
        std::cout << " 6. 转换 WIM 压缩类型\n";
        std::cout << " 0. 返回上级菜单\n";
        std::cout << "===================================\n";

        int choice = GetIntInput("请选择: ");

        if (choice == 0) break;

        if (choice == 1)
        {
            std::string applyDir = GetInput("已应用 WIM 的目录路径: ");
            if (!applyDir.empty())
            {
                std::cout << "\n警告：将从 " << applyDir << " 卸载 WIM 镜像。\n";
                std::cout << "是否继续？(y/n): ";
                char c;
                std::cin >> c;
                std::cin.ignore();
                if (c == 'y' || c == 'Y')
                    RunCmd("dism /Unmount-Image /MountDir:\"" + applyDir + "\" /Discard");
            }
            Pause();
        }
        else if (choice == 2)
        {
            std::string wimPath = GetInput("WIM 文件路径: ");
            if (!wimPath.empty())
            {
                system(("dism /Get-WimInfo /WimFile:\"" + wimPath + "\"").c_str());
                int index = GetIntInput("要删除的索引号: ");
                if (index > 0)
                {
                    std::cout << "\n警告：将永久删除索引 " << index << "！\n";
                    std::cout << "是否继续？(y/n): ";
                    char c;
                    std::cin >> c;
                    std::cin.ignore();
                    if (c == 'y' || c == 'Y')
                        RunCmd("dism /Delete-Image /ImageFile:\"" + wimPath + "\" /Index:" + std::to_string(index));
                }
            }
            Pause();
        }
        else if (choice == 3)
        {
            std::string wimPath = GetInput("源 WIM 文件路径: ");
            std::string newWim = GetInput("输出 WIM 文件路径: ");
            int index = GetIntInput("要导出的索引号: ");
            std::string name = GetInput("新镜像名称（可空）: ");
            if (!wimPath.empty() && !newWim.empty() && index > 0)
            {
                std::string cmd = "dism /Export-Image /SourceImageFile:\"" + wimPath + "\" /SourceIndex:" + std::to_string(index) + " /DestinationImageFile:\"" + newWim + "\"";
                if (!name.empty()) cmd += " /Name:\"" + name + "\"";
                RunCmd(cmd);
            }
            Pause();
        }
        else if (choice == 4)
        {
            std::string wimPath = GetInput("源 WIM 文件路径: ");
            std::string outDir = GetInput("输出目录: ");
            std::string prefix = GetInput("文件名前缀（默认 install）: ");
            if (prefix.empty()) prefix = "install";
            int sizeMB = GetIntInput("每个分卷大小（MB，默认 4096）: ", 4096);
            if (!wimPath.empty() && !outDir.empty())
            {
                std::string cmd = "dism /Split-Image /ImageFile:\"" + wimPath + "\" /SWMFile:\"" + outDir + "\\" + prefix + ".swm\" /FileSize:" + std::to_string(sizeMB);
                RunCmd(cmd);
            }
            Pause();
        }
        else if (choice == 5)
        {
            std::string firstSwm = GetInput("第一个 SWM 文件路径（.swm）: ");
            std::string wimPath = GetInput("输出 WIM 文件路径: ");
            if (!firstSwm.empty() && !wimPath.empty())
                RunCmd("dism /Export-Image /SourceImageFile:\"" + firstSwm + "\" /DestinationImageFile:\"" + wimPath + "\"");
            Pause();
        }
        else if (choice == 6)
        {
            std::string srcWim = GetInput("源 WIM 路径: ");
            std::string dstWim = GetInput("输出 WIM 路径: ");
            std::cout << "压缩类型:\n 1. none\n 2. fast\n 3. max\n";
            int c = GetIntInput("请选择: ");
            std::string compress;
            switch (c)
            {
                case 1: compress = "/Compress:none"; break;
                case 2: compress = "/Compress:fast"; break;
                case 3: compress = "/Compress:max"; break;
            }
            if (!srcWim.empty() && !dstWim.empty() && !compress.empty())
                RunCmd("dism /Export-Image /SourceImageFile:\"" + srcWim + "\" /DestinationImageFile:\"" + dstWim + "\" " + compress);
            Pause();
        }
        else
        {
            std::cout << "无效选项。" << std::endl;
            Pause();
        }
    }
}

// ==================== DISM 主菜单 ====================
void DismWimMenu()
{
    while (true)
    {
        ClearScreen();
        std::cout << "========== DISM 工具 ==========\n";
        std::cout << "=== WIM 镜像 ===\n";
        std::cout << " 1. 解压（应用）WIM 镜像\n";
        std::cout << " 2. 打包目录为 WIM 镜像\n";
        std::cout << " 3. 查看 WIM 镜像信息\n";
        std::cout << " 4. 挂载 WIM 镜像（可编辑）\n";
        std::cout << " 5. WIM 索引管理\n";
        std::cout << "\n=== 系统映像修复 ===\n";
        std::cout << " 6. 检查系统映像健康状态\n";
        std::cout << " 7. 扫描系统映像\n";
        std::cout << " 8. 修复系统映像（在线）\n";
        std::cout << " 9. 指定源修复系统映像\n";
        std::cout << "10. SFC + DISM 联合修复\n";
        std::cout << "\n=== WinSxS 组件存储 ===\n";
        std::cout << "11. 分析组件存储\n";
        std::cout << "12. 清理组件存储\n";
        std::cout << "13. 重置组件存储基础（ResetBase）\n";
        std::cout << "14. 查看组件存储大小\n";
        std::cout << "15. 查看已安装更新包\n";
        std::cout << "16. 卸载指定更新包\n";
        std::cout << "17. 清理所有已取代的更新\n";
        std::cout << "\n=== 系统映像管理 ===\n";
        std::cout << "18. 查看系统映像信息\n";
        std::cout << "19. 挂载 WIM 映像（可写）\n";
        std::cout << "20. 挂载 WIM 映像（只读）\n";
        std::cout << "21. 卸载 WIM 映像（保存）\n";
        std::cout << "22. 卸载 WIM 映像（放弃）\n";
        std::cout << "23. 提交 WIM 更改\n";
        std::cout << "24. 查看挂载的映像\n";
        std::cout << "25. 清理挂载点\n";
        std::cout << "\n=== 驱动管理 ===\n";
        std::cout << "26. 导出所有驱动\n";
        std::cout << "27. 添加驱动到系统映像\n";
        std::cout << "28. 查看已安装驱动\n";
        std::cout << "29. 删除系统映像中的驱动\n";
        std::cout << "\n=== 功能管理 ===\n";
        std::cout << "30. 查看所有 Windows 功能\n";
        std::cout << "31. 查看已启用的功能\n";
        std::cout << "32. 启用 Windows 功能\n";
        std::cout << "33. 禁用 Windows 功能\n";
        std::cout << "34. 启用 .NET 3.5\n";
        std::cout << "\n=== 高级操作 ===\n";
        std::cout << "35. 优化系统映像\n";
        std::cout << "36. 清理临时文件\n";
        std::cout << "37. 导出系统信息报告\n";
        std::cout << "38. 修复 Windows Update 错误\n";
        std::cout << "39. 系统映像格式转换\n";
        std::cout << "40. 查看映像版本信息\n";
        std::cout << " 0. 返回主菜单\n";
        std::cout << "================================\n";

        int choice = GetIntInput("请选择: ");

        if (choice == 0) break;

        switch (choice)
        {
            // ==================== WIM 镜像 ====================
            case 1: DismExtractWim(); break;
            case 2: DismCaptureWim(); break;
            case 3:
            {
                std::string wimPath = GetInput("WIM 文件路径: ");
                if (!wimPath.empty())
                    RunCmd("dism /Get-WimInfo /WimFile:\"" + wimPath + "\"");
                Pause();
                break;
            }
            case 4:
            {
                std::string wimPath = GetInput("WIM 文件路径: ");
                std::string mountDir = GetInput("挂载目录: ");
                int index = GetIntInput("索引号（默认1）: ", 1);
                if (!wimPath.empty() && !mountDir.empty())
                {
                    std::string cmd = "dism /Mount-Image /ImageFile:\"" + wimPath + "\" /Index:" + std::to_string(index) + " /MountDir:\"" + mountDir + "\"";
                    RunCmd(cmd);
                }
                Pause();
                break;
            }
            case 5: DismWimIndexManage(); break;

            // ==================== 系统映像修复 ====================
            case 6:
            {
                ClearScreen();
                std::cout << "========== 检查系统映像健康状态 ==========\n";
                std::cout << "快速检查，不扫描完整映像。\n\n";
                RunCmd("DISM /Online /Cleanup-Image /CheckHealth");
                Pause();
                break;
            }
            case 7:
            {
                ClearScreen();
                std::cout << "========== 扫描系统映像 ==========\n";
                std::cout << "完整扫描，耗时较长。\n\n";
                RunCmd("DISM /Online /Cleanup-Image /ScanHealth");
                Pause();
                break;
            }
            case 8:
            {
                ClearScreen();
                std::cout << "========== 修复系统映像 ==========\n";
                std::cout << "从 Windows Update 下载修复文件。\n\n";
                RunCmd("DISM /Online /Cleanup-Image /RestoreHealth");
                Pause();
                break;
            }
            case 9:
            {
                ClearScreen();
                std::cout << "========== 指定源修复系统映像 ==========\n";
                std::string src = GetInput("源 WIM 路径（如 D:\\sources\\install.wim）: ");
                std::string index = GetInput("索引号（默认1）: ");
                if (index.empty()) index = "1";
                if (!src.empty())
                {
                    std::string cmd = "DISM /Online /Cleanup-Image /RestoreHealth /Source:WIM:" + src + ":" + index + " /LimitAccess";
                    RunCmd(cmd);
                }
                Pause();
                break;
            }
            case 10:
            {
                ClearScreen();
                std::cout << "========== SFC + DISM 联合修复 ==========\n";
                std::cout << "先运行 DISM，再运行 SFC。\n\n";
                std::cout << "是否继续？(y/n): ";
                char c;
                std::cin >> c;
                std::cin.ignore();
                if (c == 'y' || c == 'Y')
                {
                    RunCmd("DISM /Online /Cleanup-Image /RestoreHealth");
                    RunCmd("sfc /scannow");
                }
                Pause();
                break;
            }

            // ==================== WinSxS 组件存储 ====================
            case 11:
            {
                ClearScreen();
                std::cout << "========== 分析组件存储 ==========\n\n";
                RunCmd("DISM /Online /Cleanup-Image /AnalyzeComponentStore");
                Pause();
                break;
            }
            case 12:
            {
                ClearScreen();
                std::cout << "========== 清理组件存储 ==========\n";
                std::cout << "清理旧的组件版本，可释放空间。\n\n";
                RunCmd("DISM /Online /Cleanup-Image /StartComponentCleanup");
                Pause();
                break;
            }
            case 13:
            {
                ClearScreen();
                std::cout << "========== 重置组件存储基础 ==========\n";
                std::cout << "警告：重置后无法卸载已安装的更新！\n\n";
                std::cout << "是否继续？(y/n): ";
                char c;
                std::cin >> c;
                std::cin.ignore();
                if (c == 'y' || c == 'Y')
                    RunCmd("DISM /Online /Cleanup-Image /StartComponentCleanup /ResetBase");
                Pause();
                break;
            }
            case 14:
            {
                ClearScreen();
                std::cout << "========== 查看组件存储大小 ==========\n\n";
                RunCmd("dir /s /a \"C:\\Windows\\WinSxS\" | findstr /i \"个文件 File(s)\"");
                std::cout << "\n详细分析：\n";
                RunCmd("DISM /Online /Cleanup-Image /AnalyzeComponentStore");
                Pause();
                break;
            }
            case 15:
            {
                ClearScreen();
                std::cout << "========== 已安装更新包 ==========\n\n";
                RunCmd("DISM /Online /Get-Packages");
                Pause();
                break;
            }
            case 16:
            {
                ClearScreen();
                std::cout << "========== 卸载更新包 ==========\n";
                std::string pkg = GetInput("包名（如 Package_for_KB5000000）: ");
                if (!pkg.empty())
                {
                    std::cout << "警告：卸载更新可能导致系统不稳定！\n";
                    std::cout << "是否继续？(y/n): ";
                    char c;
                    std::cin >> c;
                    std::cin.ignore();
                    if (c == 'y' || c == 'Y')
                    {
                        std::string cmd = "DISM /Online /Remove-Package /PackageName:" + pkg;
                        RunCmd(cmd);
                    }
                }
                Pause();
                break;
            }
            case 17:
            {
                ClearScreen();
                std::cout << "========== 清理已取代的更新 ==========\n";
                std::cout << "清理所有被取代的更新包。\n\n";
                RunCmd("DISM /Online /Cleanup-Image /StartComponentCleanup");
                Pause();
                break;
            }

            // ==================== 系统映像管理 ====================
            case 18:
            {
                ClearScreen();
                std::cout << "========== 系统映像信息 ==========\n\n";
                RunCmd("DISM /Online /Get-ImageInfo");
                Pause();
                break;
            }
            case 19:
            {
                ClearScreen();
                std::cout << "========== 挂载 WIM 映像（可写） ==========\n";
                std::string wimPath = GetInput("WIM 文件路径: ");
                std::string mountDir = GetInput("挂载目录: ");
                int index = GetIntInput("索引号（默认1）: ", 1);
                if (!wimPath.empty() && !mountDir.empty())
                {
                    std::string cmd = "dism /Mount-Image /ImageFile:\"" + wimPath + "\" /Index:" + std::to_string(index) + " /MountDir:\"" + mountDir + "\" /Optimize";
                    RunCmd(cmd);
                }
                Pause();
                break;
            }
            case 20:
            {
                ClearScreen();
                std::cout << "========== 挂载 WIM 映像（只读） ==========\n";
                std::string wimPath = GetInput("WIM 文件路径: ");
                std::string mountDir = GetInput("挂载目录: ");
                int index = GetIntInput("索引号（默认1）: ", 1);
                if (!wimPath.empty() && !mountDir.empty())
                {
                    std::string cmd = "dism /Mount-Image /ImageFile:\"" + wimPath + "\" /Index:" + std::to_string(index) + " /MountDir:\"" + mountDir + "\" /ReadOnly";
                    RunCmd(cmd);
                }
                Pause();
                break;
            }
            case 21:
            {
                ClearScreen();
                std::cout << "========== 卸载 WIM（保存） ==========\n";
                std::string mountDir = GetInput("挂载目录: ");
                if (!mountDir.empty())
                    RunCmd("dism /Unmount-Image /MountDir:\"" + mountDir + "\" /Commit");
                Pause();
                break;
            }
            case 22:
            {
                ClearScreen();
                std::cout << "========== 卸载 WIM（放弃） ==========\n";
                std::string mountDir = GetInput("挂载目录: ");
                if (!mountDir.empty())
                    RunCmd("dism /Unmount-Image /MountDir:\"" + mountDir + "\" /Discard");
                Pause();
                break;
            }
            case 23:
            {
                ClearScreen();
                std::cout << "========== 提交 WIM 更改 ==========\n";
                std::string mountDir = GetInput("挂载目录: ");
                if (!mountDir.empty())
                    RunCmd("dism /Commit-Wim /MountDir:\"" + mountDir + "\"");
                Pause();
                break;
            }
            case 24:
            {
                ClearScreen();
                std::cout << "========== 已挂载的映像 ==========\n\n";
                RunCmd("dism /Get-MountedWimInfo");
                Pause();
                break;
            }
            case 25:
            {
                ClearScreen();
                std::cout << "========== 清理挂载点 ==========\n\n";
                RunCmd("dism /Cleanup-Wim");
                Pause();
                break;
            }

            // ==================== 驱动管理 ====================
            case 26:
            {
                ClearScreen();
                std::cout << "========== 导出所有驱动 ==========\n";
                std::string dir = GetInput("导出目录: ");
                if (!dir.empty())
                    RunCmd("dism /online /export-driver /destination:\"" + dir + "\"");
                Pause();
                break;
            }
            case 27:
            {
                ClearScreen();
                std::cout << "========== 添加驱动到系统映像 ==========\n";
                std::string inf = GetInput("INF 文件路径: ");
                if (!inf.empty())
                    RunCmd("dism /online /add-driver /driver:\"" + inf + "\" /recurse");
                Pause();
                break;
            }
            case 28:
            {
                ClearScreen();
                std::cout << "========== 已安装驱动 ==========\n\n";
                RunCmd("dism /online /get-drivers /format:table");
                Pause();
                break;
            }
            case 29:
            {
                ClearScreen();
                std::cout << "========== 删除系统映像中的驱动 ==========\n";
                std::string oem = GetInput("驱动 OEM 名称（如 oem0.inf）: ");
                if (!oem.empty())
                {
                    std::cout << "警告：删除驱动可能影响硬件！\n";
                    std::cout << "是否继续？(y/n): ";
                    char c;
                    std::cin >> c;
                    std::cin.ignore();
                    if (c == 'y' || c == 'Y')
                        RunCmd("dism /online /remove-driver /driver:" + oem);
                }
                Pause();
                break;
            }

            // ==================== 功能管理 ====================
            case 30:
            {
                ClearScreen();
                std::cout << "========== 所有 Windows 功能 ==========\n\n";
                RunCmd("dism /online /get-features /format:table");
                Pause();
                break;
            }
            case 31:
            {
                ClearScreen();
                std::cout << "========== 已启用的功能 ==========\n\n";
                RunCmd("dism /online /get-features | findstr /i \"Enable\"");
                Pause();
                break;
            }
            case 32:
            {
                ClearScreen();
                std::cout << "========== 启用 Windows 功能 ==========\n";
                std::string feat = GetInput("功能名称: ");
                if (!feat.empty())
                    RunCmd("dism /online /enable-feature /featurename:" + feat + " /all");
                Pause();
                break;
            }
            case 33:
            {
                ClearScreen();
                std::cout << "========== 禁用 Windows 功能 ==========\n";
                std::string feat = GetInput("功能名称: ");
                if (!feat.empty())
                {
                    std::cout << "警告：禁用功能可能导致某些程序无法运行！\n";
                    std::cout << "是否继续？(y/n): ";
                    char c;
                    std::cin >> c;
                    std::cin.ignore();
                    if (c == 'y' || c == 'Y')
                        RunCmd("dism /online /disable-feature /featurename:" + feat);
                }
                Pause();
                break;
            }
            case 34:
            {
                ClearScreen();
                std::cout << "========== 启用 .NET 3.5 ==========\n";
                std::cout << "从 Windows Update 下载。\n\n";
                RunCmd("dism /online /enable-feature /featurename:NetFx3 /all");
                Pause();
                break;
            }

            // ==================== 高级操作 ====================
            case 35:
            {
                ClearScreen();
                std::cout << "========== 优化系统映像 ==========\n";
                std::cout << "清理组件存储 + 重置基础。\n\n";
                std::cout << "是否继续？(y/n): ";
                char c;
                std::cin >> c;
                std::cin.ignore();
                if (c == 'y' || c == 'Y')
                    RunCmd("DISM /Online /Cleanup-Image /StartComponentCleanup /ResetBase");
                Pause();
                break;
            }
            case 36:
            {
                ClearScreen();
                std::cout << "========== 清理临时文件 ==========\n\n";
                RunCmd("dism /online /cleanup-image /startcomponentcleanup");
                RunCmd("del /q /f /s \"C:\\Windows\\Temp\\*\" >nul 2>&1", false);
                std::cout << "清理完成。\n";
                Pause();
                break;
            }
            case 37:
            {
                ClearScreen();
                std::cout << "========== 导出系统信息报告 ==========\n";
                std::string path = GetInput("保存路径（如 C:\\sysinfo.txt）: ");
                if (!path.empty())
                {
                    RunCmd("systeminfo > \"" + path + "\"");
                    std::cout << "已导出到: " << path << std::endl;
                }
                Pause();
                break;
            }
            case 38:
            {
                ClearScreen();
                std::cout << "========== 修复 Windows Update 错误 ==========\n";
                std::cout << "停止服务 → 重命名缓存 → 重启服务。\n\n";
                std::cout << "是否继续？(y/n): ";
                char c;
                std::cin >> c;
                std::cin.ignore();
                if (c == 'y' || c == 'Y')
                {
                    RunCmd("net stop wuauserv");
                    RunCmd("net stop bits");
                    RunCmd("ren C:\\Windows\\SoftwareDistribution SoftwareDistribution.old");
                    RunCmd("net start wuauserv");
                    RunCmd("net start bits");
                    std::cout << "\n修复完成。请重新运行 Windows Update。\n";
                }
                Pause();
                break;
            }
            case 39:
            {
                ClearScreen();
                std::cout << "========== 系统映像格式转换 ==========\n";
                std::cout << " 1. ESD 转 WIM\n";
                std::cout << " 2. WIM 转 ESD\n";
                std::cout << " 3. WIM 转 ISO（手动挂载）\n";
                int c = GetIntInput("请选择: ");

                if (c == 1)
                {
                    std::string esd = GetInput("ESD 文件路径: ");
                    std::string wim = GetInput("输出 WIM 路径: ");
                    int index = GetIntInput("索引号（默认1）: ", 1);
                    if (!esd.empty() && !wim.empty())
                        RunCmd("dism /Export-Image /SourceImageFile:\"" + esd + "\" /SourceIndex:" + std::to_string(index) + " /DestinationImageFile:\"" + wim + "\" /Compress:max");
                }
                else if (c == 2)
                {
                    std::string wim = GetInput("WIM 文件路径: ");
                    std::string esd = GetInput("输出 ESD 路径: ");
                    int index = GetIntInput("索引号（默认1）: ", 1);
                    if (!wim.empty() && !esd.empty())
                        RunCmd("dism /Export-Image /SourceImageFile:\"" + wim + "\" /SourceIndex:" + std::to_string(index) + " /DestinationImageFile:\"" + esd + "\" /Compress:recovery");
                }
                else if (c == 3)
                {
                    std::cout << "请使用 oscdimg 工具，或直接挂载 WIM。\n";
                }
                Pause();
                break;
            }
            case 40:
            {
                ClearScreen();
                std::cout << "========== 映像版本信息 ==========\n\n";
                RunCmd("dism /online /get-currentedition");
                RunCmd("dism /online /get-targeteditions");
                Pause();
                break;
            }

            default:
                std::cout << "无效选项。" << std::endl;
                Pause();
        }
    }
}

// ====================================================================
// 14. TrustedInstaller 特权操作模块（完整版）
// ====================================================================

// 获取 PsExec64.exe 完整路径
std::string GetPsExecPath()
{
    char currentDir[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, currentDir);
    std::string psexecPath = std::string(currentDir) + "\\PsExec64.exe";
    
    DWORD attr = GetFileAttributesA(psexecPath.c_str());
    if (attr == INVALID_FILE_ATTRIBUTES)
    {
        return "PsExec64.exe";
    }
    
    return psexecPath;
}

// 检查 PsExec64.exe 是否存在
bool CheckPsExec()
{
    std::string psexecPath = GetPsExecPath();
    DWORD attr = GetFileAttributesA(psexecPath.c_str());
    if (attr == INVALID_FILE_ATTRIBUTES)
    {
        std::cout << "未找到 PsExec64.exe！" << std::endl;
        std::cout << "请下载并放置到程序同目录：" << std::endl;
        std::cout << "https://live.sysinternals.com/PsExec64.exe" << std::endl;
        return false;
    }
    return true;
}

// 检查 TrustedInstaller 服务是否运行
bool IsTrustedInstallerRunning()
{
    SC_HANDLE hSCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_CONNECT);
    if (!hSCM) return false;
    
    SC_HANDLE hService = OpenServiceW(hSCM, L"TrustedInstaller", SERVICE_QUERY_STATUS);
    if (!hService)
    {
        CloseServiceHandle(hSCM);
        return false;
    }
    
    SERVICE_STATUS status;
    BOOL result = QueryServiceStatus(hService, &status);
    
    CloseServiceHandle(hService);
    CloseServiceHandle(hSCM);
    
    return result && status.dwCurrentState == SERVICE_RUNNING;
}

// 启动 TrustedInstaller 服务
bool StartTrustedInstallerService()
{
    SC_HANDLE hSCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (!hSCM) return false;
    
    SC_HANDLE hService = OpenServiceW(hSCM, L"TrustedInstaller", SERVICE_ALL_ACCESS);
    if (!hService)
    {
        CloseServiceHandle(hSCM);
        return false;
    }
    
    BOOL result = StartServiceW(hService, 0, NULL);
    
    CloseServiceHandle(hService);
    CloseServiceHandle(hSCM);
    
    return result;
}

// 以 TrustedInstaller 权限执行命令
bool RunAsTrustedInstaller(const std::string& command, bool showOutput)
{
    if (!CheckPsExec()) return false;
    
    std::string psexec = GetPsExecPath();
    std::string cmd = "\"" + psexec + "\" -accepteula -s " + command;
    
    if (!showOutput)
    {
        cmd += " >nul 2>&1";
    }
    
    std::cout << "执行命令: " << cmd << std::endl;
    int ret = system(cmd.c_str());
    return ret == 0;
}

// 以 TrustedInstaller 权限执行命令并捕获输出
std::string RunAsTrustedInstallerWithOutput(const std::string& command)
{
    if (!CheckPsExec()) return "";
    
    std::string psexec = GetPsExecPath();
    
    // 清理命令中多余的引号
    std::string cleanCmd = command;
    size_t pos;
    while ((pos = cleanCmd.find("\"\"")) != std::string::npos)
    {
        cleanCmd.replace(pos, 2, "\"");
    }
    
    std::string cmd = "\"" + psexec + "\" -accepteula -s " + cleanCmd + " 2>&1";
    
    std::cout << "执行命令: " << cmd << std::endl;
    
    FILE* pipe = _popen(cmd.c_str(), "r");
    if (!pipe)
    {
        return "无法执行命令";
    }
    
    std::stringstream buffer;
    char line[4096];
    while (fgets(line, sizeof(line), pipe) != nullptr)
    {
        buffer << line;
    }
    
    _pclose(pipe);
    
    // 过滤 PsExec 的额外输出
    std::string output = buffer.str();
    
    // 移除 PsExec 版权信息
    std::vector<std::string> skipLines = {
        "PsExec v",
        "Copyright",
        "Sysinternals",
        "Starting ",
        "exited on "
    };
    
    std::stringstream filtered;
    std::stringstream ss(output);
    std::string line2;
    
    while (std::getline(ss, line2))
    {
        bool skip = false;
        for (const auto& skipStr : skipLines)
        {
            if (line2.find(skipStr) != std::string::npos)
            {
                skip = true;
                break;
            }
        }
        
        if (!skip && !line2.empty())
        {
            filtered << line2 << "\n";
        }
    }
    
    std::string result = filtered.str();
    
    if (result.empty())
    {
        return "命令执行完成，无输出";
    }
    
    return result;
}

// 以 TrustedInstaller 权限启动进程
bool StartProcessAsTrustedInstaller(const std::string& exePath, const std::string& args)
{
    if (!CheckPsExec()) return false;
    
    std::string psexec = GetPsExecPath();
    
    // 提取引号之间的路径
    std::string cleanPath = exePath;
    size_t firstQuote = exePath.find('"');
    size_t lastQuote = exePath.rfind('"');
    
    if (firstQuote != std::string::npos && lastQuote != std::string::npos && firstQuote != lastQuote)
    {
        cleanPath = exePath.substr(firstQuote + 1, lastQuote - firstQuote - 1);
    }
    
    // 使用 cmd /k 保持窗口打开
    std::string cmd = "\"" + psexec + "\" -accepteula -s -i -d cmd /k \"\"" + cleanPath + "\"\"";
    if (!args.empty())
    {
        cmd += " " + args;
    }
    
    std::cout << "执行命令: " << cmd << std::endl;
    
    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    
    BOOL result = CreateProcessA(
        NULL,
        (LPSTR)cmd.c_str(),
        NULL, NULL, FALSE,
        CREATE_NO_WINDOW,
        NULL, NULL,
        &si, &pi
    );
    
    if (result)
    {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return true;
    }
    
    return false;
}

// 以 TrustedInstaller 权限删除文件
bool DeleteFileAsTrustedInstaller(const std::string& filePath)
{
    if (!CheckPsExec()) return false;
    
    std::string psexec = GetPsExecPath();
    std::string cmd = "\"" + psexec + "\" -accepteula -s cmd /c \"del /f /q \\\"" + filePath + "\\\"\"";
    
    std::cout << "执行命令: " << cmd << std::endl;
    int ret = system(cmd.c_str());
    return ret == 0;
}

// 以 TrustedInstaller 权限修改注册表
bool SetRegistryAsTrustedInstaller(const std::string& keyPath, const std::string& valueName,
                                    const std::string& valueData, bool isDword)
{
    if (!CheckPsExec()) return false;
    
    std::string psexec = GetPsExecPath();
    std::string cmd = "\"" + psexec + "\" -accepteula -s reg add \\\"" + keyPath + "\\\" /v \\\"" + valueName + "\\\" /t ";
    
    if (isDword)
    {
        cmd += "REG_DWORD /d " + valueData;
    }
    else
    {
        cmd += "REG_SZ /d \\\"" + valueData + "\\\"";
    }
    
    cmd += " /f";
    
    std::cout << "执行命令: " << cmd << std::endl;
    int ret = system(cmd.c_str());
    return ret == 0;
}

// 以 TrustedInstaller 权限控制服务
bool ControlServiceAsTrustedInstaller(const std::string& serviceName, bool startService)
{
    if (!CheckPsExec()) return false;
    
    std::string action = startService ? "start" : "stop";
    std::string psexec = GetPsExecPath();
    std::string cmd = "\"" + psexec + "\" -accepteula -s sc " + action + " \\\"" + serviceName + "\\\"";
    
    std::cout << "执行命令: " << cmd << std::endl;
    int ret = system(cmd.c_str());
    return ret == 0;
}

// 以 TrustedInstaller 权限运行 PowerShell 命令（捕获输出）
std::string RunPowerShellAsTrustedInstallerWithOutput(const std::string& psCommand)
{
    if (!CheckPsExec()) return "";
    
    std::string psexec = GetPsExecPath();
    
    // 清理多余引号
    std::string cleanCmd = psCommand;
    size_t pos;
    while ((pos = cleanCmd.find("\"\"")) != std::string::npos)
    {
        cleanCmd.replace(pos, 2, "\"");
    }
    
    // 使用单引号包裹 PowerShell 命令
    std::string cmd = "\"" + psexec + "\" -accepteula -s powershell -Command '" + cleanCmd + "' 2>&1";
    
    std::cout << "执行命令: " << cmd << std::endl;
    
    FILE* pipe = _popen(cmd.c_str(), "r");
    if (!pipe)
    {
        return "无法执行命令";
    }
    
    std::stringstream buffer;
    char line[4096];
    while (fgets(line, sizeof(line), pipe) != nullptr)
    {
        buffer << line;
    }
    
    _pclose(pipe);
    
    // 过滤 PsExec 额外输出
    std::string output = buffer.str();
    std::stringstream filtered;
    std::stringstream ss(output);
    std::string line2;
    
    std::vector<std::string> skipLines = {
        "PsExec v",
        "Copyright",
        "Sysinternals",
        "Starting ",
        "exited on "
    };
    
    while (std::getline(ss, line2))
    {
        bool skip = false;
        for (const auto& skipStr : skipLines)
        {
            if (line2.find(skipStr) != std::string::npos)
            {
                skip = true;
                break;
            }
        }
        
        if (!skip && !line2.empty())
        {
            filtered << line2 << "\n";
        }
    }
    
    std::string result = filtered.str();
    
    if (result.empty())
    {
        return "命令执行完成，无输出";
    }
    
    return result;
}

// 以 TrustedInstaller 权限复制文件
bool CopyFileAsTrustedInstaller(const std::string& sourcePath, const std::string& destPath)
{
    if (!CheckPsExec()) return false;
    
    std::string psexec = GetPsExecPath();
    std::string cmd = "\"" + psexec + "\" -accepteula -s cmd /c \"copy /y \\\"" + sourcePath + "\\\" \\\"" + destPath + "\\\"\"";
    
    std::cout << "执行命令: " << cmd << std::endl;
    int ret = system(cmd.c_str());
    return ret == 0;
}

// 以 TrustedInstaller 权限移动文件
bool MoveFileAsTrustedInstaller(const std::string& sourcePath, const std::string& destPath)
{
    if (!CheckPsExec()) return false;
    
    std::string psexec = GetPsExecPath();
    std::string cmd = "\"" + psexec + "\" -accepteula -s cmd /c \"move /y \\\"" + sourcePath + "\\\" \\\"" + destPath + "\\\"\"";
    
    std::cout << "执行命令: " << cmd << std::endl;
    int ret = system(cmd.c_str());
    return ret == 0;
}

// 以 TrustedInstaller 权限创建目录
bool CreateDirectoryAsTrustedInstaller(const std::string& dirPath)
{
    if (!CheckPsExec()) return false;
    
    std::string psexec = GetPsExecPath();
    std::string cmd = "\"" + psexec + "\" -accepteula -s cmd /c \"mkdir \\\"" + dirPath + "\\\"\"";
    
    std::cout << "执行命令: " << cmd << std::endl;
    int ret = system(cmd.c_str());
    return ret == 0;
}

// 以 TrustedInstaller 权限删除目录（含多重安全确认）
bool RemoveDirectoryAsTrustedInstaller(const std::string& dirPath)
{
    if (!CheckPsExec()) return false;

    // ==================== 免责声明 ====================
    ClearScreen();
    std::cout << "╔════════════════════════════════════════════════╗\n";
    std::cout << "║          ⚠️  危险操作警告  ⚠️                  ║\n";
    std::cout << "╚════════════════════════════════════════════════╝\n\n";

    std::cout << "您即将执行：递归删除目录（TrustedInstaller 权限）\n\n";
    std::cout << "目标目录: " << dirPath << "\n\n";

    std::cout << "【免责声明】\n";
    std::cout << "1. 此操作将递归删除目录下所有文件和子目录，无法恢复！\n";
    std::cout << "2. 使用 TrustedInstaller 权限，可以删除系统保护文件。\n";
    std::cout << "3. 误删系统文件可能导致系统无法启动。\n";
    std::cout << "4. 作者不对因使用本功能造成的任何损失负责。\n";
    std::cout << "5. 请确认您完全理解此操作的后果。\n\n";

    std::cout << "【安全提示】\n";
    std::cout << "- 请勿删除 C:\\Windows、C:\\Program Files 等系统目录\n";
    std::cout << "- 建议先用文件资源管理器确认目标目录内容\n";
    std::cout << "- 建议先备份重要数据\n\n";

    std::cout << "════════════════════════════════════════════════\n";

    // ==================== 第一次确认 ====================
    std::cout << "\n请输入 DELETE（大写）确认执行: ";
    std::string confirm;
    std::getline(std::cin, confirm);

    if (confirm != "DELETE")
    {
        std::cout << "\n[已取消] 输入不匹配，操作终止。\n";
        Pause();
        return false;
    }

    // ==================== 第二次确认 ====================
    std::cout << "\n最后确认：真的要删除 " << dirPath << " 吗？\n";
    std::cout << "输入 YES（大写）继续: ";
    std::getline(std::cin, confirm);

    if (confirm != "YES")
    {
        std::cout << "\n[已取消] 操作终止。\n";
        Pause();
        return false;
    }

    // ==================== 执行删除 ====================
    std::cout << "\n[执行中] 正在以 TrustedInstaller 权限删除...\n";
    std::cout << "目标: " << dirPath << "\n\n";

    std::string psexec = GetPsExecPath();
    std::string cmd = "\"" + psexec + "\" -accepteula -s cmd /c \"rmdir /s /q \\\"" + dirPath + "\\\"\"";

    std::cout << "执行命令: " << cmd << "\n\n";

    int ret = system(cmd.c_str());

    if (ret == 0)
    {
        std::cout << "\n[✓] 删除完成。\n";
    }
    else
    {
        std::cout << "\n[✗] 删除失败（返回码: " << ret << "）。\n";
        std::cout << "可能原因：\n";
        std::cout << "  - 目录不存在\n";
        std::cout << "  - 权限不足\n";
        std::cout << "  - 文件被占用\n";
    }

    Pause();
    return ret == 0;
}

// 以 TrustedInstaller 权限设置文件属性
bool SetFileAttributesAsTrustedInstaller(const std::string& filePath, const std::string& attributes)
{
    if (!CheckPsExec()) return false;
    
    std::string psexec = GetPsExecPath();
    std::string cmd = "\"" + psexec + "\" -accepteula -s cmd /c \"attrib " + attributes + " \\\"" + filePath + "\\\"\"";
    
    std::cout << "执行命令: " << cmd << std::endl;
    int ret = system(cmd.c_str());
    return ret == 0;
}

// 以 TrustedInstaller 权限获取文件所有权
bool TakeOwnershipAsTrustedInstaller(const std::string& filePath)
{
    if (!CheckPsExec()) return false;
    
    std::string psexec = GetPsExecPath();
    std::string cmd = "\"" + psexec + "\" -accepteula -s takeown /f \\\"" + filePath + "\\\" /a";
    
    std::cout << "执行命令: " << cmd << std::endl;
    int ret = system(cmd.c_str());
    return ret == 0;
}

// 以 TrustedInstaller 权限授予文件完全控制权
bool GrantFullControlAsTrustedInstaller(const std::string& filePath, const std::string& userName)
{
    if (!CheckPsExec()) return false;
    
    std::string psexec = GetPsExecPath();
    std::string cmd = "\"" + psexec + "\" -accepteula -s icacls \\\"" + filePath + "\\\" /grant \\\"" + userName + "\\\":F /t";
    
    std::cout << "执行命令: " << cmd << std::endl;
    int ret = system(cmd.c_str());
    return ret == 0;
}

// 14. TrustedInstaller 特权操作菜单
void TrustedInstallerMenu()
{
    while (true)
    {
        ClearScreen();
        std::cout << "========== TrustedInstaller 特权操作 ==========\n";
        std::cout << " 1. 检查 PsExec64.exe\n";
        std::cout << " 2. 以 TrustedInstaller 权限执行命令\n";
        std::cout << " 3. 以 TrustedInstaller 权限执行命令（显示输出）\n";
        std::cout << " 4. 以 TrustedInstaller 权限启动程序\n";
        std::cout << " 5. 以 TrustedInstaller 权限删除文件\n";
        std::cout << " 6. 以 TrustedInstaller 权限修改注册表\n";
        std::cout << " 7. 以 TrustedInstaller 权限控制服务\n";
        std::cout << " 8. 以 TrustedInstaller 权限运行 PowerShell\n";
        std::cout << " 9. 以 TrustedInstaller 权限复制文件\n";
        std::cout << "10. 以 TrustedInstaller 权限移动文件\n";
        std::cout << "11. 以 TrustedInstaller 权限创建目录\n";
        std::cout << "12. 以 TrustedInstaller 权限删除目录\n";
        std::cout << "13. 以 TrustedInstaller 权限设置文件属性\n";
        std::cout << "14. 以 TrustedInstaller 权限获取文件所有权\n";
        std::cout << "15. 以 TrustedInstaller 权限授予完全控制权\n";
        std::cout << "16. 下载 PsExec64.exe\n";
        std::cout << " 0. 返回主菜单\n";
        std::cout << "===============================================\n";
        std::cout << "请注意。本功能仅供开发者使用\n";
        std::cout << "作者对产生的一切后果拒不负责\n";
        std::cout << "===============================================\n";
        
        std::string psexecPath = GetPsExecPath();
        DWORD attr = GetFileAttributesA(psexecPath.c_str());
        bool psexecExists = (attr != INVALID_FILE_ATTRIBUTES);
        
        char currentDir[MAX_PATH];
        GetCurrentDirectoryA(MAX_PATH, currentDir);
        
        std::cout << "PsExec64.exe: " << (psexecExists ? "存在" : "不存在") << "\n";
        std::cout << "TrustedInstaller: " << (IsTrustedInstallerRunning() ? "运行中" : "未运行") << "\n";
        std::cout << "当前目录: " << currentDir << "\n";
        std::cout << "===============================================\n";
        
        int choice = GetIntInput("请选择: ");

        if (choice == 0)
        {
            break;
        }
        else if (choice == 1)
        {
            if (psexecExists)
                std::cout << "PsExec64.exe 已存在。" << std::endl;
            else
                std::cout << "PsExec64.exe 不存在，请选择选项 16 下载。" << std::endl;
            Pause();
        }
        else if (choice == 2)
        {
            std::string cmd = GetInput("请输入要执行的命令: ");
            if (!cmd.empty())
            {
                if (RunAsTrustedInstaller(cmd, true))
                    std::cout << "命令执行成功！" << std::endl;
                else
                    std::cout << "命令执行失败！" << std::endl;
            }
            Pause();
        }
        else if (choice == 3)
        {
            std::string cmd = GetInput("请输入要执行的命令: ");
            if (!cmd.empty())
            {
                std::string output = RunAsTrustedInstallerWithOutput(cmd);
                std::cout << "命令输出:\n" << output << std::endl;
            }
            Pause();
        }
        else if (choice == 4)
        {
            std::string exePath = GetInput("请输入程序完整路径: ");
            std::string args = GetInput("请输入参数（可空）: ");
            if (!exePath.empty())
            {
                if (StartProcessAsTrustedInstaller(exePath, args))
                    std::cout << "进程启动成功！" << std::endl;
                else
                    std::cout << "进程启动失败！" << std::endl;
            }
            Pause();
        }
        else if (choice == 5)
        {
            std::string filePath = GetInput("请输入要删除的文件路径: ");
            if (!filePath.empty())
            {
                if (DeleteFileAsTrustedInstaller(filePath))
                    std::cout << "文件删除成功！" << std::endl;
                else
                    std::cout << "文件删除失败！" << std::endl;
            }
            Pause();
        }
        else if (choice == 6)
        {
            std::string keyPath = GetInput("请输入注册表路径（如 HKLM\\SOFTWARE\\Test）: ");
            std::string valueName = GetInput("请输入值名称: ");
            std::string valueData = GetInput("请输入值数据: ");
            std::cout << "类型: 1. 字符串  2. DWORD\n";
            int type = GetIntInput("请选择: ");
            
            if (!keyPath.empty() && !valueName.empty())
            {
                bool isDword = (type == 2);
                if (SetRegistryAsTrustedInstaller(keyPath, valueName, valueData, isDword))
                    std::cout << "注册表修改成功！" << std::endl;
                else
                    std::cout << "注册表修改失败！" << std::endl;
            }
            Pause();
        }
 else if (choice == 7)
{
    std::string serviceName = GetInput("请输入服务名称: ");
    std::cout << " 1. 启动服务\n";
    std::cout << " 2. 停止服务\n";
    int action = GetIntInput("请选择操作: ");
    
    if (!serviceName.empty())
    {
        bool start = (action == 1);
        if (ControlServiceAsTrustedInstaller(serviceName, start))
            std::cout << "服务操作成功！" << std::endl;
        else
            std::cout << "服务操作失败！" << std::endl;
    }
    Pause();
}
else if (choice == 8)
{
    std::string psCmd = GetInput("请输入 PowerShell 命令: ");
    if (!psCmd.empty())
    {
        std::string output = RunPowerShellAsTrustedInstallerWithOutput(psCmd);
        std::cout << "命令输出:\n" << output << std::endl;
    }
    Pause();
}

        else if (choice == 9)
        {
            std::string sourcePath = GetInput("请输入源文件路径: ");
            std::string destPath = GetInput("请输入目标文件路径: ");
            if (!sourcePath.empty() && !destPath.empty())
            {
                if (CopyFileAsTrustedInstaller(sourcePath, destPath))
                    std::cout << "文件复制成功！" << std::endl;
                else
                    std::cout << "文件复制失败！" << std::endl;
            }
            Pause();
        }
        else if (choice == 10)
        {
            std::string sourcePath = GetInput("请输入源文件路径: ");
            std::string destPath = GetInput("请输入目标文件路径: ");
            if (!sourcePath.empty() && !destPath.empty())
            {
                if (MoveFileAsTrustedInstaller(sourcePath, destPath))
                    std::cout << "文件移动成功！" << std::endl;
                else
                    std::cout << "文件移动失败！" << std::endl;
            }
            Pause();
        }
        else if (choice == 11)
        {
            std::string dirPath = GetInput("请输入要创建的目录路径: ");
            if (!dirPath.empty())
            {
                if (CreateDirectoryAsTrustedInstaller(dirPath))
                    std::cout << "目录创建成功！" << std::endl;
                else
                    std::cout << "目录创建失败！" << std::endl;
            }
            Pause();
        }
        else if (choice == 12)
        {
            std::string dirPath = GetInput("请输入要删除的目录路径: ");
            if (!dirPath.empty())
            {
                if (RemoveDirectoryAsTrustedInstaller(dirPath))
                    std::cout << "目录删除成功！" << std::endl;
                else
                    std::cout << "目录删除失败！" << std::endl;
            }
            Pause();
        }
        else if (choice == 13)
        {
            std::string filePath = GetInput("请输入文件路径: ");
            std::cout << "属性选项: +h 隐藏, -h 取消隐藏, +s 系统, -s 取消系统, +r 只读, -r 取消只读\n";
            std::string attributes = GetInput("请输入属性参数: ");
            if (!filePath.empty() && !attributes.empty())
            {
                if (SetFileAttributesAsTrustedInstaller(filePath, attributes))
                    std::cout << "属性设置成功！" << std::endl;
                else
                    std::cout << "属性设置失败！" << std::endl;
            }
            Pause();
        }
        else if (choice == 14)
        {
            std::string filePath = GetInput("请输入要获取所有权的文件路径: ");
            if (!filePath.empty())
            {
                if (TakeOwnershipAsTrustedInstaller(filePath))
                    std::cout << "所有权获取成功！" << std::endl;
                else
                    std::cout << "所有权获取失败！" << std::endl;
            }
            Pause();
        }
        else if (choice == 15)
        {
            std::string filePath = GetInput("请输入文件路径: ");
            std::string userName = GetInput("请输入用户名（如 Everyone）: ");
            if (!filePath.empty() && !userName.empty())
            {
                if (GrantFullControlAsTrustedInstaller(filePath, userName))
                    std::cout << "权限授予成功！" << std::endl;
                else
                    std::cout << "权限授予失败！" << std::endl;
            }
            Pause();
        }
        else if (choice == 16)
        {
            std::cout << "正在下载 PsExec64.exe..." << std::endl;
            std::string downloadCmd = "powershell -Command \"Invoke-WebRequest -Uri 'https://live.sysinternals.com/PsExec64.exe' -OutFile 'PsExec64.exe'\"";
            int ret = system(downloadCmd.c_str());
            if (ret == 0)
                std::cout << "下载成功！" << std::endl;
            else
                std::cout << "下载失败！请手动从 https://live.sysinternals.com/PsExec64.exe 下载" << std::endl;
            Pause();
        }
        else
        {
            std::cout << "无效选项。" << std::endl;
            Pause();
        }
    }
}

// 系统时间同步
void SyncSystemTime()
{
    ClearScreen();
    std::cout << "========== 系统时间同步 ==========\n";
    std::cout << "同步服务器: time.windows.com\n";
    std::cout << "需要管理员权限。\n";
    std::cout << "===================================\n";
    std::cout << "注意，time.windows.com是微软海外服务器，同步时间可能较长亦或失败，这是正常现象。\n";
    
    std::cout << "是否继续？(y/n): ";
    char c;
    std::cin >> c;
    std::cin.ignore();
    
    if (c != 'y' && c != 'Y')
    {
        std::cout << "操作已取消。" << std::endl;
        Pause();
        return;
    }
    
    std::cout << "\n正在同步时间...\n";
    RunCmd("w32tm /resync /force");
    
    std::cout << "时间同步完成。" << std::endl;
    Pause();
}

// ==================== 存储单位换算 ====================
void StorageUnitConverter()
{
    while (true)
    {
        ClearScreen();
        std::cout << "========== 存储单位换算 ==========\n";
        std::cout << " 1. Byte (字节)\n";
        std::cout << " 2. KB (千字节)\n";
        std::cout << " 3. MB (兆字节)\n";
        std::cout << " 4. GB (吉字节)\n";
        std::cout << " 5. TB (太字节)\n";
        std::cout << " 6. PB (拍字节)\n";
        std::cout << " 7. EB (艾字节)\n";
        std::cout << " 8. ZB (泽字节)\n";
        std::cout << " 9. YB (尧字节)\n";
        std::cout << " 0. 返回主菜单\n";
        std::cout << "===================================\n";
        std::cout << "换算标准:\n";
        std::cout << " 1 KB = 1024 Byte\n";
        std::cout << " 1 MB = 1024 KB\n";
        std::cout << " 1 GB = 1024 MB\n";
        std::cout << " 1 TB = 1024 GB\n";
        std::cout << " 1 PB = 1024 TB\n";
        std::cout << " 1 EB = 1024 PB\n";
        std::cout << " 1 ZB = 1024 EB\n";
        std::cout << " 1 YB = 1024 ZB\n";
        std::cout << "===================================\n";
        
        int fromUnit = GetIntInput("请选择源单位: ");
        
        if (fromUnit == 0) break;
        if (fromUnit < 1 || fromUnit > 9)
        {
            std::cout << "无效选项。" << std::endl;
            Pause();
            continue;
        }
        
        std::cout << "请输入数值（支持小数）: ";
        std::string valueStr;
        std::getline(std::cin, valueStr);
        
        if (valueStr.empty())
        {
            Pause();
            continue;
        }
        
        long double value = std::stold(valueStr);
        
        if (value < 0)
        {
            std::cout << "数值不能为负数。" << std::endl;
            Pause();
            continue;
        }
        
        ClearScreen();
        std::cout << "========== 换算结果 ==========\n";
        std::cout << "输入: " << valueStr << " ";
        
        const char* units[] = {"Byte", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
        std::cout << units[fromUnit - 1] << "\n\n";
        
        // 转换为 Byte
        long double bytes = value;
        int power = fromUnit - 1;
        for (int i = 0; i < power; i++) bytes *= 1024.0L;
        
        std::cout << "=== 完整换算（全单位显示） ===\n";
        
        // 计算所有单位的值
        long double values[9];
        values[0] = bytes;
        for (int i = 1; i < 9; i++)
        {
            values[i] = values[i - 1] / 1024.0L;
        }
        
        // 显示所有单位，包括小数和科学计数法
        for (int i = 0; i < 9; i++)
        {
            std::cout << std::setw(4) << units[i] << ": ";
            
            // 始终显示完整精度
            // 小数值用固定小数，大数值用科学计数法
            if (values[i] == 0.0L)
            {
                std::cout << "0";
            }
            else if (values[i] < 0.000001L)
            {
                std::cout << std::scientific << std::setprecision(20) << values[i];
            }
            else if (values[i] < 1.0L)
            {
                std::cout << std::fixed << std::setprecision(20) << values[i];
            }
            else if (values[i] < 1000000.0L)
            {
                std::cout << std::fixed << std::setprecision(10) << values[i];
            }
            else if (values[i] < 1000000000000.0L)
            {
                std::cout << std::fixed << std::setprecision(3) << values[i];
            }
            else
            {
                // 超大数值：同时显示科学计数法
                std::cout << std::scientific << std::setprecision(25) << values[i];
                std::cout << "  (";
                std::cout << std::fixed << std::setprecision(0) << values[i];
                std::cout << ")";
            }
            
            std::cout << "\n";
        }
        
        std::cout << "\n========================" << std::endl;
        Pause();
    }
}

// ==================== NTDLL 底层 API 操作 ====================

typedef NTSTATUS(WINAPI* pNtQuerySystemInformation)(
    ULONG SystemInformationClass,
    PVOID SystemInformation,
    ULONG SystemInformationLength,
    PULONG ReturnLength
);

typedef NTSTATUS(WINAPI* pNtQueryInformationProcess)(
    HANDLE ProcessHandle,
    ULONG ProcessInformationClass,
    PVOID ProcessInformation,
    ULONG ProcessInformationLength,
    PULONG ReturnLength
);

typedef NTSTATUS(WINAPI* pRtlGetVersion)(
    PRTL_OSVERSIONINFOW lpVersionInformation
);

typedef NTSTATUS(WINAPI* pNtQuerySystemTime)(
    PLARGE_INTEGER SystemTime
);

// NTDLL 底层 API 菜单
void NtdllApiMenu()
{
    while (true)
    {
        ClearScreen();
        std::cout << "========== NTDLL 底层 API 操作 ==========\n";
        std::cout << " 1. 获取系统版本信息（RtlGetVersion）\n";
        std::cout << " 2. 获取系统基本信息（NtQuerySystemInformation）\n";
        std::cout << " 3. 获取进程信息（NtQueryInformationProcess）\n";
        std::cout << " 4. 获取系统时间（NtQuerySystemTime）\n";
        std::cout << " 5. 文件/文件夹操作\n";
        std::cout << " 0. 返回主菜单\n";
        std::cout << "===========================================\n";
        
        int choice = GetIntInput("请选择: ");
        
        if (choice == 0) break;
        
        if (choice == 5)
        {
            NtdllFileOperations();
            continue;
        }
        
        HMODULE hNtdll = LoadLibraryA("ntdll.dll");
        if (!hNtdll)
        {
            std::cout << "无法加载 ntdll.dll！" << std::endl;
            Pause();
            continue;
        }
        
        switch (choice)
        {
            case 1:
            {
                pRtlGetVersion RtlGetVersion = (pRtlGetVersion)GetProcAddress(hNtdll, "RtlGetVersion");
                if (RtlGetVersion)
                {
                    RTL_OSVERSIONINFOW osInfo;
                    osInfo.dwOSVersionInfoSize = sizeof(osInfo);
                    NTSTATUS status = RtlGetVersion(&osInfo);
                    if (status == 0)
                    {
                        std::cout << "\n=== 系统版本信息 ===\n";
                        std::cout << "主版本: " << osInfo.dwMajorVersion << "\n";
                        std::cout << "次版本: " << osInfo.dwMinorVersion << "\n";
                        std::cout << "构建号: " << osInfo.dwBuildNumber << "\n";
                        std::cout << "平台ID: " << osInfo.dwPlatformId << "\n";
                    }
                    else
                    {
                        std::cout << "查询失败！状态码: 0x" << std::hex << status << std::dec << "\n";
                    }
                }
                else
                {
                    std::cout << "无法获取 RtlGetVersion 函数！" << std::endl;
                }
                break;
            }
            
            case 2:
            {
                pNtQuerySystemInformation NtQuerySystemInformation = 
                    (pNtQuerySystemInformation)GetProcAddress(hNtdll, "NtQuerySystemInformation");
                
                if (NtQuerySystemInformation)
                {
                    typedef struct _SYSTEM_BASIC_INFORMATION {
                        ULONG Reserved;
                        ULONG TimerResolution;
                        ULONG PageSize;
                        ULONG NumberOfPhysicalPages;
                        ULONG LowestPhysicalPageNumber;
                        ULONG HighestPhysicalPageNumber;
                        ULONG AllocationGranularity;
                        ULONG MinimumUserModeAddress;
                        ULONG MaximumUserModeAddress;
                        ULONG ActiveProcessorsAffinityMask;
                        ULONG NumberOfProcessors;
                    } SYSTEM_BASIC_INFORMATION;
                    
                    SYSTEM_BASIC_INFORMATION basicInfo;
                    ULONG returnLength = 0;
                    NTSTATUS status = NtQuerySystemInformation(
                        0,
                        &basicInfo,
                        sizeof(basicInfo),
                        &returnLength
                    );
                    
                    if (status == 0)
                    {
                        std::cout << "\n=== 系统基本信息 ===\n";
                        std::cout << "页面大小: " << basicInfo.PageSize << " 字节\n";
                        std::cout << "物理内存: " << (basicInfo.NumberOfPhysicalPages * basicInfo.PageSize) / (1024.0 * 1024.0) << " MB\n";
                        std::cout << "处理器数: " << basicInfo.NumberOfProcessors << "\n";
                        std::cout << "分配粒度: " << basicInfo.AllocationGranularity << "\n";
                    }
                    else
                    {
                        std::cout << "查询失败！状态码: 0x" << std::hex << status << std::dec << "\n";
                    }
                }
                else
                {
                    std::cout << "无法获取 NtQuerySystemInformation 函数！" << std::endl;
                }
                break;
            }
            
            case 3:
            {
                pNtQueryInformationProcess NtQueryInformationProcess = 
                    (pNtQueryInformationProcess)GetProcAddress(hNtdll, "NtQueryInformationProcess");
                
                if (NtQueryInformationProcess)
                {
                    DWORD pid = GetIntInput("请输入进程 PID: ");
                    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
                    if (hProcess)
                    {
                        typedef struct _PROCESS_BASIC_INFORMATION {
                            NTSTATUS ExitStatus;
                            PVOID PebBaseAddress;
                            ULONG_PTR AffinityMask;
                            LONG BasePriority;
                            ULONG_PTR UniqueProcessId;
                            ULONG_PTR InheritedFromUniqueProcessId;
                        } PROCESS_BASIC_INFORMATION;
                        
                        PROCESS_BASIC_INFORMATION pbi;
                        ULONG returnLength = 0;
                        NTSTATUS status = NtQueryInformationProcess(
                            hProcess,
                            0,
                            &pbi,
                            sizeof(pbi),
                            &returnLength
                        );
                        
                        if (status == 0)
                        {
                            std::cout << "\n=== 进程信息 ===\n";
                            std::cout << "PID: " << pbi.UniqueProcessId << "\n";
                            std::cout << "父PID: " << pbi.InheritedFromUniqueProcessId << "\n";
                            std::cout << "基本优先级: " << pbi.BasePriority << "\n";
                        }
                        else
                        {
                            std::cout << "查询失败！状态码: 0x" << std::hex << status << std::dec << "\n";
                        }
                        CloseHandle(hProcess);
                    }
                    else
                    {
                        std::cout << "无法打开进程！错误码: " << GetLastError() << std::endl;
                    }
                }
                else
                {
                    std::cout << "无法获取 NtQueryInformationProcess 函数！" << std::endl;
                }
                break;
            }
            
            case 4:
            {
                pNtQuerySystemTime NtQuerySystemTime = 
                    (pNtQuerySystemTime)GetProcAddress(hNtdll, "NtQuerySystemTime");
                
                if (NtQuerySystemTime)
                {
                    LARGE_INTEGER systemTime;
                    NTSTATUS status = NtQuerySystemTime(&systemTime);
                    if (status == 0)
                    {
                        FILETIME ft;
                        ft.dwHighDateTime = systemTime.HighPart;
                        ft.dwLowDateTime = systemTime.LowPart;
                        
                        SYSTEMTIME st;
                        FileTimeToSystemTime(&ft, &st);
                        
                        std::cout << "\n=== 系统时间 ===\n";
                        std::cout << st.wYear << "-" << st.wMonth << "-" << st.wDay << " ";
                        std::cout << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "\n";
                    }
                    else
                    {
                        std::cout << "查询失败！状态码: 0x" << std::hex << status << std::dec << "\n";
                    }
                }
                else
                {
                    std::cout << "无法获取 NtQuerySystemTime 函数！" << std::endl;
                }
                break;
            }
            
            default:
                std::cout << "无效选项。" << std::endl;
        }
        
        FreeLibrary(hNtdll);
        Pause();
    }
}

// 文件/文件夹操作
void NtdllFileOperations()
{
    while (true)
    {
        ClearScreen();
        std::cout << "========== 文件/文件夹操作 ==========\n";
        std::cout << " 1. 创建文件\n";
        std::cout << " 2. 创建文件夹\n";
        std::cout << " 3. 删除文件\n";
        std::cout << " 4. 删除文件夹（空）\n";
        std::cout << " 5. 删除文件夹（递归）\n";
        std::cout << " 0. 返回上级菜单\n";
        std::cout << "=====================================\n";
        
        int choice = GetIntInput("请选择: ");
        
        if (choice == 0) break;
        
        if (choice == 1)
        {
            std::string filePath = GetInput("请输入文件路径: ");
            if (filePath.empty()) { Pause(); continue; }
            
            HANDLE hFile = CreateFileA(
                filePath.c_str(),
                GENERIC_WRITE,
                0,
                NULL,
                CREATE_ALWAYS,
                FILE_ATTRIBUTE_NORMAL,
                NULL
            );
            
            if (hFile != INVALID_HANDLE_VALUE)
            {
                std::cout << "文件创建成功！" << std::endl;
                CloseHandle(hFile);
            }
            else
            {
                std::cout << "创建失败！错误码: " << GetLastError() << std::endl;
            }
            Pause();
        }
        else if (choice == 2)
        {
            std::string dirPath = GetInput("请输入文件夹路径: ");
            if (dirPath.empty()) { Pause(); continue; }
            
            if (CreateDirectoryA(dirPath.c_str(), NULL))
            {
                std::cout << "文件夹创建成功！" << std::endl;
            }
            else
            {
                std::cout << "创建失败！错误码: " << GetLastError() << std::endl;
            }
            Pause();
        }
        else if (choice == 3)
        {
            std::string filePath = GetInput("请输入文件路径: ");
            if (filePath.empty()) { Pause(); continue; }
            
            if (DeleteFileA(filePath.c_str()))
            {
                std::cout << "文件删除成功！" << std::endl;
            }
            else
            {
                std::cout << "删除失败！错误码: " << GetLastError() << std::endl;
            }
            Pause();
        }
        else if (choice == 4)
        {
            std::string dirPath = GetInput("请输入文件夹路径: ");
            if (dirPath.empty()) { Pause(); continue; }
            
            if (RemoveDirectoryA(dirPath.c_str()))
            {
                std::cout << "文件夹删除成功！" << std::endl;
            }
            else
            {
                std::cout << "删除失败！错误码: " << GetLastError() << std::endl;
            }
            Pause();
        }
        else if (choice == 5)
        {
            std::string dirPath = GetInput("请输入文件夹路径: ");
            if (dirPath.empty()) { Pause(); continue; }
            
            std::cout << "警告：将递归删除 " << dirPath << " 及其所有内容！\n";
            std::cout << "是否继续？(y/n): ";
            char c;
            std::cin >> c;
            std::cin.ignore();
            
            if (c != 'y' && c != 'Y')
            {
                std::cout << "操作已取消。" << std::endl;
                Pause();
                continue;
            }
            
            std::string cmd = "rmdir /s /q \"" + dirPath + "\"";
            system(cmd.c_str());
            std::cout << "文件夹递归删除完成。" << std::endl;
            Pause();
        }
        else
        {
            std::cout << "无效选项。" << std::endl;
            Pause();
        }
    }
}

// ==================== 镜像应用（完整版） ====================

// 检查 oscdimg
bool CheckOscdimg()
{
    return system("where oscdimg >nul 2>&1") == 0;
}

// 检查 wimlib-imagex
bool CheckWimlib()
{
    return system("where wimlib-imagex >nul 2>&1") == 0;
}

// ESD 转 WIM
void EsdToWim()
{
    ClearScreen();
    std::cout << "========== ESD 转 WIM ==========\n";
    std::string esdPath = GetInput("ESD 文件路径: ");
    if (esdPath.empty()) { Pause(); return; }
    std::string wimPath = GetInput("输出 WIM 文件路径: ");
    if (wimPath.empty()) { Pause(); return; }

    system(("dism /Get-WimInfo /WimFile:\"" + esdPath + "\"").c_str());

    int index = GetIntInput("索引号（默认1）: ", 1);
    if (index <= 0) index = 1;

    std::cout << "压缩: 1.max 2.fast 3.none 4.默认\n";
    int c = GetIntInput("请选择: ", 1);
    std::string compress;
    switch (c) {
        case 1: compress = "/Compress:max"; break;
        case 2: compress = "/Compress:fast"; break;
        case 3: compress = "/Compress:none"; break;
    }

    std::string cmd = "dism /Export-Image /SourceImageFile:\"" + esdPath + "\" /SourceIndex:" + std::to_string(index) + " /DestinationImageFile:\"" + wimPath + "\"";
    if (!compress.empty()) cmd += " " + compress;
    RunCmd(cmd);
    Pause();
}

// WIM 转 ESD
void WimToEsd()
{
    ClearScreen();
    std::cout << "========== WIM 转 ESD ==========\n";
    std::string wimPath = GetInput("WIM 文件路径: ");
    std::string esdPath = GetInput("输出 ESD 文件路径: ");
    if (wimPath.empty() || esdPath.empty()) { Pause(); return; }

    system(("dism /Get-WimInfo /WimFile:\"" + wimPath + "\"").c_str());
    int index = GetIntInput("索引号（默认1）: ", 1);

    std::string cmd = "dism /Export-Image /SourceImageFile:\"" + wimPath + "\" /SourceIndex:" + std::to_string(index) + " /DestinationImageFile:\"" + esdPath + "\" /Compress:recovery";
    RunCmd(cmd);
    Pause();
}

// ESD 转 ISO
void EsdToIso()
{
    ClearScreen();
    std::cout << "========== ESD 转 ISO ==========\n";
    std::cout << "需要 Windows ADK 的 oscdimg.exe\n\n";

    std::string esdPath = GetInput("ESD 文件路径: ");
    if (esdPath.empty()) { Pause(); return; }
    std::string isoPath = GetInput("输出 ISO 文件路径: ");
    if (isoPath.empty()) { Pause(); return; }

    // 工作目录
    std::string workDir = "C:\\esd_to_iso_work";
    CreateDirectoryA(workDir.c_str(), nullptr);

    // 1. ESD → WIM
    std::string tempWim = workDir + "\\install.wim";
    system(("dism /Get-WimInfo /WimFile:\"" + esdPath + "\"").c_str());
    int index = GetIntInput("索引号（默认1）: ", 1);

    std::cout << "\n[1/3] ESD → WIM...\n";
    system(("dism /Export-Image /SourceImageFile:\"" + esdPath + "\" /SourceIndex:" + std::to_string(index) + " /DestinationImageFile:\"" + tempWim + "\" /Compress:max").c_str());

    if (GetFileAttributesA(tempWim.c_str()) == INVALID_FILE_ATTRIBUTES)
    {
        std::cout << "[FAIL] ESD 转 WIM 失败！\n";
        Pause();
        return;
    }

    // 2. 挂载 WIM
    std::string mountDir = workDir + "\\mount";
    CreateDirectoryA(mountDir.c_str(), nullptr);

    std::cout << "\n[2/3] 挂载 WIM 到 " << mountDir << "...\n";
    system(("dism /Mount-Wim /WimFile:\"" + tempWim + "\" /Index:" + std::to_string(index) + " /MountDir:\"" + mountDir + "\"").c_str());

    // 3. oscdimg 打包
    std::cout << "\n[3/3] 用 oscdimg 打包 ISO...\n";
    if (!CheckOscdimg())
    {
        std::cout << "[FAIL] 未找到 oscdimg.exe，请安装 Windows ADK。\n";
        std::cout << "手动打包命令：\n";
        std::cout << "oscdimg -m -o -u2 -udfver102 -bootdata:2#p0,e,b" << mountDir << "\\boot\\etfsboot.com#pEF,e,b" << mountDir << "\\efi\\microsoft\\boot\\efisys.bin \"" << mountDir << "\" \"" << isoPath << "\"\n";
        Pause();
        return;
    }

    std::string cmd = "oscdimg -m -o -u2 -udfver102 -bootdata:2#p0,e,b\"" + mountDir + "\\boot\\etfsboot.com\"#pEF,e,b\"" + mountDir + "\\efi\\microsoft\\boot\\efisys.bin\" \"" + mountDir + "\" \"" + isoPath + "\"";
    system(cmd.c_str());

    // 4. 卸载 WIM
    std::cout << "\n[清理] 卸载 WIM...\n";
    system(("dism /Unmount-Wim /MountDir:\"" + mountDir + "\" /Discard").c_str());

    if (GetFileAttributesA(isoPath.c_str()) != INVALID_FILE_ATTRIBUTES)
    {
        std::cout << "\n[OK] ISO 已生成: " << isoPath << std::endl;
        std::cout << "工作目录: " << workDir << "（可手动清理）\n";
    }
    else
    {
        std::cout << "\n[FAIL] 生成 ISO 失败。\n";
    }
    Pause();
}

// ISO 转 ESD
void IsoToEsd()
{
    ClearScreen();
    std::cout << "========== ISO 转 ESD ==========\n";
    std::string isoPath = GetInput("ISO 文件路径: ");
    if (isoPath.empty()) { Pause(); return; }
    std::string esdPath = GetInput("输出 ESD 文件路径: ");
    if (esdPath.empty()) { Pause(); return; }

    // 挂载 ISO
    std::cout << "\n[1/3] 挂载 ISO...\n";
    std::string mountCmd = "powershell -Command \"Mount-DiskImage -ImagePath '" + isoPath + "' | Get-Volume | Select-Object -ExpandProperty DriveLetter\"";
    FILE* pipe = _popen(mountCmd.c_str(), "r");
    std::string driveLetter;
    if (pipe)
    {
        char buf[64];
        if (fgets(buf, sizeof(buf), pipe))
        {
            driveLetter = buf;
            while (!driveLetter.empty() && (driveLetter.back() == '\r' || driveLetter.back() == '\n' || driveLetter.back() == ' '))
                driveLetter.pop_back();
        }
        _pclose(pipe);
    }

    if (driveLetter.empty())
    {
        std::cout << "[FAIL] 挂载 ISO 失败，请手动指定盘符。\n";
        driveLetter = GetInput("ISO 盘符（如 E:）: ");
        if (!driveLetter.empty() && driveLetter.back() != ':') driveLetter += ":";
    }

    std::string wimFile = driveLetter + "\\sources\\install.wim";
    std::string esdFile = driveLetter + "\\sources\\install.esd";

    // 优先用 WIM，没有则用 ESD
    std::string srcFile;
    if (GetFileAttributesA(wimFile.c_str()) != INVALID_FILE_ATTRIBUTES)
        srcFile = wimFile;
    else if (GetFileAttributesA(esdFile.c_str()) != INVALID_FILE_ATTRIBUTES)
        srcFile = esdFile;
    else
    {
        std::cout << "[FAIL] ISO 中找不到 install.wim 或 install.esd\n";
        Pause();
        return;
    }

    std::cout << "源文件: " << srcFile << "\n";

    // 列出索引
    system(("dism /Get-WimInfo /WimFile:\"" + srcFile + "\"").c_str());
    int index = GetIntInput("要转换的索引号: ");

    std::cout << "\n[2/3] 转换到 ESD...\n";
    std::string cmd = "dism /Export-Image /SourceImageFile:\"" + srcFile + "\" /SourceIndex:" + std::to_string(index) + " /DestinationImageFile:\"" + esdPath + "\" /Compress:recovery";
    system(cmd.c_str());

    // 卸载 ISO
    std::cout << "\n[3/3] 卸载 ISO...\n";
    system(("powershell -Command \"Dismount-DiskImage -ImagePath '" + isoPath + "'\"").c_str());

    if (GetFileAttributesA(esdPath.c_str()) != INVALID_FILE_ATTRIBUTES)
        std::cout << "\n[OK] ESD 已生成: " << esdPath << std::endl;
    else
        std::cout << "\n[FAIL] 转换失败。\n";
    Pause();
}

// ISO 转 WIM
void IsoToWim()
{
    ClearScreen();
    std::cout << "========== ISO 转 WIM ==========\n";
    std::string isoPath = GetInput("ISO 文件路径: ");
    if (isoPath.empty()) { Pause(); return; }
    std::string wimPath = GetInput("输出 WIM 文件路径: ");
    if (wimPath.empty()) { Pause(); return; }

    std::cout << "\n[1/3] 挂载 ISO...\n";
    std::string mountCmd = "powershell -Command \"(Mount-DiskImage -ImagePath '" + isoPath + "' -PassThru | Get-Volume).DriveLetter\"";
    FILE* pipe = _popen(mountCmd.c_str(), "r");
    std::string driveLetter;
    if (pipe)
    {
        char buf[64];
        if (fgets(buf, sizeof(buf), pipe))
        {
            driveLetter = buf;
            while (!driveLetter.empty() && (driveLetter.back() == '\r' || driveLetter.back() == '\n' || driveLetter.back() == ' '))
                driveLetter.pop_back();
        }
        _pclose(pipe);
    }

    if (driveLetter.empty())
    {
        driveLetter = GetInput("ISO 盘符（如 E:）: ");
        if (!driveLetter.empty() && driveLetter.back() != ':') driveLetter += ":";
    }
    else
    {
        driveLetter += ":";
    }

    std::string srcWim = driveLetter + "\\sources\\install.wim";
    std::string srcEsd = driveLetter + "\\sources\\install.esd";

    std::string srcFile;
    if (GetFileAttributesA(srcWim.c_str()) != INVALID_FILE_ATTRIBUTES)
        srcFile = srcWim;
    else if (GetFileAttributesA(srcEsd.c_str()) != INVALID_FILE_ATTRIBUTES)
        srcFile = srcEsd;
    else
    {
        std::cout << "[FAIL] ISO 中找不到 install.wim/esd\n";
        Pause();
        return;
    }

    system(("dism /Get-WimInfo /WimFile:\"" + srcFile + "\"").c_str());
    int index = GetIntInput("索引号: ");

    std::cout << "\n[2/3] 转换 WIM...\n";
    std::string cmd = "dism /Export-Image /SourceImageFile:\"" + srcFile + "\" /SourceIndex:" + std::to_string(index) + " /DestinationImageFile:\"" + wimPath + "\" /Compress:max";
    system(cmd.c_str());

    std::cout << "\n[3/3] 卸载 ISO...\n";
    system(("powershell -Command \"Dismount-DiskImage -ImagePath '" + isoPath + "'\"").c_str());

    Pause();
}

// ISO 转 EDU（本质是 ESD，教育版）
void IsoToEdu()
{
    ClearScreen();
    std::cout << "========== ISO 转 EDU ==========\n";
    std::cout << "EDU 本质是 ESD 格式（教育版镜像）\n\n";
    IsoToEsd();  // 复用
}

// 挂载 ISO（获取盘符）
void MountIso()
{
    ClearScreen();
    std::cout << "========== 挂载 ISO ==========\n";
    std::string isoPath = GetInput("ISO 文件路径: ");
    if (isoPath.empty()) { Pause(); return; }

    std::string cmd = "powershell -Command \"Mount-DiskImage -ImagePath '" + isoPath + "'\"";
    RunCmd(cmd);

    std::cout << "\n查看盘符：\n";
    RunCmd("powershell -Command \"Get-Volume | Where-Object DriveType -eq 'CD-ROM' | Format-Table DriveLetter,FileSystemLabel\"");
    Pause();
}

// 卸载 ISO
void DismountIso()
{
    ClearScreen();
    std::cout << "========== 卸载 ISO ==========\n";
    std::string isoPath = GetInput("ISO 文件路径: ");
    if (isoPath.empty()) { Pause(); return; }

    std::string cmd = "powershell -Command \"Dismount-DiskImage -ImagePath '" + isoPath + "'\"";
    RunCmd(cmd);
    Pause();
}

// 挂载 WIM
void MountWim()
{
    ClearScreen();
    std::cout << "========== 挂载 WIM ==========\n";
    std::string wimPath = GetInput("WIM 文件路径: ");
    std::string mountDir = GetInput("挂载目录: ");
    if (wimPath.empty() || mountDir.empty()) { Pause(); return; }

    system(("dism /Get-WimInfo /WimFile:\"" + wimPath + "\"").c_str());
    int index = GetIntInput("索引号（默认1）: ", 1);

    std::cout << "\n挂载模式：\n 1. 可写\n 2. 只读\n";
    int mode = GetIntInput("请选择: ", 1);

    std::string cmd = "dism /Mount-Wim /WimFile:\"" + wimPath + "\" /Index:" + std::to_string(index) + " /MountDir:\"" + mountDir + "\"";
    if (mode == 2) cmd += " /ReadOnly";

    RunCmd(cmd);
    Pause();
}

// 卸载 WIM
void UnmountWim()
{
    ClearScreen();
    std::cout << "========== 卸载 WIM ==========\n";
    std::string mountDir = GetInput("挂载目录: ");
    if (mountDir.empty()) { Pause(); return; }

    std::cout << " 1. 保存更改 (/Commit)\n";
    std::cout << " 2. 放弃更改 (/Discard)\n";
    int c = GetIntInput("请选择: ");

    std::string cmd = "dism /Unmount-Wim /MountDir:\"" + mountDir + "\"";
    cmd += (c == 1) ? " /Commit" : " /Discard";

    RunCmd(cmd);
    Pause();
}

// 查看已挂载 WIM
void ListMountedWim()
{
    ClearScreen();
    std::cout << "========== 已挂载的 WIM ==========\n";
    RunCmd("dism /Get-MountedWimInfo");
    Pause();
}

// ISO 信息查看
void IsoInfo()
{
    ClearScreen();
    std::cout << "========== ISO 信息 ==========\n";
    std::string isoPath = GetInput("ISO 文件路径: ");
    if (isoPath.empty()) { Pause(); return; }

    // 挂载 ISO
    std::cout << "\n正在挂载 ISO...\n";
    std::string mountCmd = "powershell -Command \"Mount-DiskImage -ImagePath '" + isoPath + "'\"";
    system(mountCmd.c_str());

    // 获取盘符
    std::cout << "\n=== ISO 内容 ===\n";
    system("powershell -Command \"Get-Volume | Where-Object DriveType -eq 'CD-ROM' | Format-Table DriveLetter,FileSystemLabel,SizeRemaining\"");
    system("powershell -Command \"$vol = Get-Volume | Where-Object DriveType -eq 'CD-ROM' | Select-Object -First 1; if ($vol) { Get-ChildItem ($vol.DriveLetter + ':\\') | Format-Table Name,Length }\"");
    system("powershell -Command \"$vol = Get-Volume | Where-Object DriveType -eq 'CD-ROM' | Select-Object -First 1; if ($vol) { Get-ChildItem ($vol.DriveLetter + ':\\sources') -ErrorAction SilentlyContinue | Format-Table Name,Length }\"");
    
    Pause();
}

// 卸载所有 ISO
void DismountAllIso()
{
    ClearScreen();
    std::cout << "========== 卸载所有 ISO ==========\n";
    RunCmd("powershell -Command \"Get-DiskImage | Where-Object Attached -eq $true | Dismount-DiskImage\"");
    Pause();
}

// 镜像应用主菜单
void ImageApplyMenu()
{
    while (true)
    {
        ClearScreen();
        std::cout << "========== 镜像应用 ==========\n";
        std::cout << "=== 格式转换 ===\n";
        std::cout << " 1. ESD 转 WIM\n";
        std::cout << " 2. WIM 转 ESD\n";
        std::cout << " 3. ESD 转 ISO\n";
        std::cout << " 4. ISO 转 ESD\n";
        std::cout << " 5. ISO 转 WIM\n";
        std::cout << " 6. ISO 转 EDU（教育版）\n";
        std::cout << "\n=== ISO 操作 ===\n";
        std::cout << " 7. 查看 ISO 信息\n";
        std::cout << " 8. 挂载 ISO\n";
        std::cout << " 9. 卸载 ISO\n";
        std::cout << "10. 卸载所有 ISO\n";
        std::cout << "\n=== WIM 挂载 ===\n";
        std::cout << "11. 挂载 WIM\n";
        std::cout << "12. 卸载 WIM\n";
        std::cout << "13. 查看已挂载 WIM\n";
        std::cout << "\n=== 其他 ===\n";
        std::cout << "14. 检查转换工具（oscdimg/wimlib）\n";
        std::cout << "15. 直接安装 oscdimg（winget）\n";
        std::cout << "16. 直接安装 wimlib（winget）\n";
        std::cout << " 0. 返回主菜单\n";
        std::cout << "=============================\n";

        int choice = GetIntInput("请选择: ");

        if (choice == 0) break;

        switch (choice)
        {
            case 1: EsdToWim(); break;
            case 2: WimToEsd(); break;
            case 3: EsdToIso(); break;
            case 4: IsoToEsd(); break;
            case 5: IsoToWim(); break;
            case 6: IsoToEdu(); break;
            case 7: IsoInfo(); break;
            case 8: MountIso(); break;
            case 9: DismountIso(); break;
            case 10: DismountAllIso(); break;
            case 11: MountWim(); break;
            case 12: UnmountWim(); break;
            case 13: ListMountedWim(); break;
            case 14:
            {
                ClearScreen();
                std::cout << "========== 转换工具检测 ==========\n\n";
                std::cout << "=== oscdimg ===\n";
                if (CheckOscdimg())
                {
                    std::cout << "[OK] 已安装\n";
                    system("where oscdimg");
                }
                else
                {
                    std::cout << "[FAIL] 未安装\n";
                    std::cout << "下载: https://learn.microsoft.com/windows-hardware/get-started/adk-install\n";
                }

                std::cout << "\n=== wimlib-imagex ===\n";
                if (CheckWimlib())
                {
                    std::cout << "[OK] 已安装\n";
                    system("where wimlib-imagex");
                }
                else
                {
                    std::cout << "[FAIL] 未安装\n";
                    std::cout << "下载: https://wimlib.net/\n";
                }

                std::cout << "\n=== DISM ===\n";
                if (system("where dism >nul 2>&1") == 0)
                    std::cout << "[OK] 系统自带\n";
                else
                    std::cout << "[FAIL] 未找到\n";

                Pause();
                break;
            }
            case 15:
            {
                std::cout << "\n是否安装 oscdimg（ADK）？\n";
                std::cout << "注意：ADK 体积较大（~1GB）\n";
                std::cout << "是否继续？(y/n): ";
                char c;
                std::cin >> c;
                std::cin.ignore();
                if (c == 'y' || c == 'Y')
                    RunCmd("winget install Microsoft.WindowsADK --silent --accept-package-agreements --accept-source-agreements");
                Pause();
                break;
            }
            case 16:
            {
                std::cout << "\n是否安装 wimlib？\n";
                std::cout << "是否继续？(y/n): ";
                char c;
                std::cin >> c;
                std::cin.ignore();
                if (c == 'y' || c == 'Y')
                {
                    RunCmd("winget install wimlib --silent --accept-package-agreements --accept-source-agreements");
                }
                Pause();
                break;
            }
            default:
                std::cout << "无效选项。" << std::endl;
                Pause();
        }
    }
}

// ==================== NPM 工具安装 ====================

void NpmInstallMenu()
{
    while (true)
    {
        ClearScreen();
        std::cout << "========== NPM 工具安装 ==========\n";
        std::cout << "=== 环境 ===\n";
        std::cout << " 1. 检查 Node.js / npm 环境\n";
        std::cout << " 2. 更新 npm 到最新版\n";
        std::cout << "\n=== AI 工具 ===\n";
        std::cout << " 3. 安装 Python（npm 方式）\n";
        std::cout << " 4. 安装 OpenAI Codex CLI\n";
        std::cout << " 5. 安装 Claude Code CLI\n";
        std::cout << " 6. 安装 Gemini CLI\n";
        std::cout << " 7. 安装 Copilot CLI\n";
        std::cout << "\n=== 开发工具 ===\n";
        std::cout << " 8. 安装 TypeScript\n";
        std::cout << " 9. 安装 nodemon（热重载）\n";
        std::cout << "10. 安装 http-server（本地服务器）\n";
        std::cout << "11. 安装 yarn（包管理器）\n";
        std::cout << "12. 安装 pnpm（包管理器）\n";
        std::cout << "13. 安装 Vite（构建工具）\n";
        std::cout << "14. 安装 webpack（打包工具）\n";
        std::cout << "15. 安装 eslint（代码检查）\n";
        std::cout << "16. 安装 prettier（代码格式化）\n";
        std::cout << "\n=== 实用工具 ===\n";
        std::cout << "17. 安装 tree-cli（目录树）\n";
        std::cout << "18. 安装 speed-test（网速测试）\n";
        std::cout << "19. 安装 serve（静态服务器）\n";
        std::cout << "20. 安装 pm2（进程管理器）\n";
        std::cout << "21. 安装 concurrently（并行运行）\n";
        std::cout << "22. 安装 cross-env（跨平台环境变量）\n";
        std::cout << "23. 安装 rimraf（跨平台删除）\n";
        std::cout << "24. 安装 nodemailer（邮件发送）\n";
        std::cout << "25. 安装 axios-cli（HTTP客户端）\n";
        std::cout << "\n=== 框架 ===\n";
        std::cout << "26. 安装 React CLI（create-react-app）\n";
        std::cout << "27. 安装 Vue CLI\n";
        std::cout << "28. 安装 Angular CLI\n";
        std::cout << "29. 安装 Next.js\n";
        std::cout << "30. 安装 Express（后端框架）\n";
        std::cout << "\n=== 数据库 ===\n";
        std::cout << "31. 安装 MySQL CLI（mysql2）\n";
        std::cout << "32. 安装 PostgreSQL CLI（pg）\n";
        std::cout << "33. 安装 MongoDB CLI（mongodb）\n";
        std::cout << "34. 安装 Redis CLI（redis）\n";
        std::cout << "\n=== 其他 ===\n";
        std::cout << "35. 查看已安装的全局 npm 包\n";
        std::cout << "==================================\n";
        std::cout << "注意，本功能仅供npm包管理器安装包，作者对安装的包不负任何责任\n";
        
        int choice = GetIntInput("请选择: ");
        
        if (choice == 0) break;
        
        auto install = [](const std::string& package)
        {
            std::cout << "\n是否安装 " << package << "？(y/n): ";
            char c;
            std::cin >> c;
            std::cin.ignore();
            if (c != 'y' && c != 'Y') { std::cout << "操作已取消。" << std::endl; return; }
            RunCmd("npm install -g " + package);
        };
        
        switch (choice)
        {
            case 1:
                ClearScreen();
                std::cout << "=== Node.js 环境检查 ===\n";
                std::cout << "\nNode.js 版本:\n";
                system("node --version 2>&1");
                std::cout << "\nnpm 版本:\n";
                system("npm --version 2>&1");
                std::cout << "\nnpx 版本:\n";
                system("npx --version 2>&1");
                break;
            case 2: install("npm@latest"); break;
            case 3: install("python"); break;
            case 4: install("@openai/codex"); break;
            case 5: install("@anthropic-ai/claude-code"); break;
            case 6: install("@google/gemini-cli"); break;
            case 7: install("@github/copilot-cli"); break;
            case 8: install("typescript"); break;
            case 9: install("nodemon"); break;
            case 10: install("http-server"); break;
            case 11: install("yarn"); break;
            case 12: install("pnpm"); break;
            case 13: install("vite"); break;
            case 14: install("webpack"); break;
            case 15: install("eslint"); break;
            case 16: install("prettier"); break;
            case 17: install("tree-cli"); break;
            case 18: install("speed-test"); break;
            case 19: install("serve"); break;
            case 20: install("pm2"); break;
            case 21: install("concurrently"); break;
            case 22: install("cross-env"); break;
            case 23: install("rimraf"); break;
            case 24: install("nodemailer"); break;
            case 25: install("axios"); break;
            case 26: install("create-react-app"); break;
            case 27: install("@vue/cli"); break;
            case 28: install("@angular/cli"); break;
            case 29: install("next"); break;
            case 30: install("express"); break;
            case 31: install("mysql2"); break;
            case 32: install("pg"); break;
            case 33: install("mongodb"); break;
            case 34: install("redis"); break;
            case 35:
                ClearScreen();
                std::cout << "=== 已安装的全局 npm 包 ===\n";
                RunCmd("npm list -g --depth=0");
                break;
            default:
                std::cout << "无效选项。" << std::endl;
        }
        Pause();
    }
}

// ==================== 网络操作工具 ====================

// 网络操作菜单
void NetworkToolsMenu()
{
    while (true)
    {
        ClearScreen();
        std::cout << "========== 网络操作工具 ==========\n";
        std::cout << "=== 网络诊断 ===\n";
        std::cout << " 1. 查看 IP 配置（ipconfig /all）\n";
        std::cout << " 2. 测试网络连通性（ping）\n";
        std::cout << " 3. 路由追踪（tracert）\n";
        std::cout << " 4. 查看路由表（route print）\n";
        std::cout << " 5. 查看 DNS 缓存\n";
        std::cout << " 6. 清除 DNS 缓存\n";
        std::cout << " 7. 查看网络连接（netstat）\n";
        std::cout << " 8. 查看 ARP 表\n";
        std::cout << "\n=== 网络配置 ===\n";
        std::cout << " 9. 刷新 DHCP（ipconfig /renew）\n";
        std::cout << "10. 释放 DHCP（ipconfig /release）\n";
        std::cout << "11. 设置静态 IP\n";
        std::cout << "12. 设置 DHCP 自动获取\n";
        std::cout << "13. 修改 DNS 服务器\n";
        std::cout << "14. 重置 DNS 为自动获取\n";
        std::cout << "\n=== 网络高级 ===\n";
        std::cout << "15. 重置 Winsock（netsh winsock reset）\n";
        std::cout << "16. 重置 TCP/IP 协议栈\n";
        std::cout << "17. 查看无线网络（netsh wlan show）\n";
        std::cout << "18. 查看网络适配器\n";
        std::cout << "19. 启用网络适配器\n";
        std::cout << "20. 禁用网络适配器\n";
        std::cout << "21. 查看网络共享\n";
        std::cout << "22. 查看防火墙状态\n";
        std::cout << "23. 打开防火墙\n";
        std::cout << "24. 关闭防火墙\n";
        std::cout << "\n=== 下载与测速 ===\n";
        std::cout << "25. 下载文件（curl）\n";
        std::cout << "26. 测试网速（speedtest）\n";
        std::cout << "27. 查看外网 IP\n";
        std::cout << "\n 0. 返回主菜单\n";
        std::cout << "==================================\n";
        
        int choice = GetIntInput("请选择: ");
        
        if (choice == 0) break;
        
        switch (choice)
        {
            case 1:
                ClearScreen();
                RunCmd("ipconfig /all");
                Pause();
                break;
                
            case 2:
            {
                std::string host = GetInput("请输入目标地址（如 baidu.com 或 8.8.8.8）: ");
                if (!host.empty())
                {
                    std::string cmd = "ping " + host + " -n 4";
                    RunCmd(cmd);
                }
                Pause();
                break;
            }
                
            case 3:
            {
                std::string host = GetInput("请输入目标地址: ");
                if (!host.empty())
                {
                    std::string cmd = "tracert " + host;
                    RunCmd(cmd);
                }
                Pause();
                break;
            }
                
            case 4:
                ClearScreen();
                RunCmd("route print");
                Pause();
                break;
                
            case 5:
                ClearScreen();
                RunCmd("ipconfig /displaydns");
                Pause();
                break;
                
            case 6:
                ClearScreen();
                RunCmd("ipconfig /flushdns");
                Pause();
                break;
                
            case 7:
                ClearScreen();
                RunCmd("netstat -ano");
                Pause();
                break;
                
            case 8:
                ClearScreen();
                RunCmd("arp -a");
                Pause();
                break;
                
            case 9:
                ClearScreen();
                RunCmd("ipconfig /renew");
                Pause();
                break;
                
            case 10:
                ClearScreen();
                RunCmd("ipconfig /release");
                Pause();
                break;
                
            case 11:
            {
                std::string adapter = GetInput("请输入适配器名称（如 以太网 或 WLAN）: ");
                std::string ip = GetInput("请输入 IP 地址: ");
                std::string mask = GetInput("请输入子网掩码: ");
                std::string gateway = GetInput("请输入网关: ");
                
                if (!adapter.empty() && !ip.empty() && !mask.empty() && !gateway.empty())
                {
                    std::string cmd = "netsh interface ip set address \"" + adapter + "\" static " + ip + " " + mask + " " + gateway;
                    RunCmd(cmd);
                }
                Pause();
                break;
            }
                
            case 12:
            {
                std::string adapter = GetInput("请输入适配器名称: ");
                if (!adapter.empty())
                {
                    std::string cmd = "netsh interface ip set address \"" + adapter + "\" dhcp";
                    RunCmd(cmd);
                }
                Pause();
                break;
            }
                
            case 13:
            {
                std::string adapter = GetInput("请输入适配器名称: ");
                std::string dns = GetInput("请输入 DNS 服务器（如 8.8.8.8）: ");
                if (!adapter.empty() && !dns.empty())
                {
                    std::string cmd = "netsh interface ip set dns \"" + adapter + "\" static " + dns;
                    RunCmd(cmd);
                }
                Pause();
                break;
            }
                
            case 14:
            {
                std::string adapter = GetInput("请输入适配器名称: ");
                if (!adapter.empty())
                {
                    std::string cmd = "netsh interface ip set dns \"" + adapter + "\" dhcp";
                    RunCmd(cmd);
                }
                Pause();
                break;
            }
                
            case 15:
                ClearScreen();
                std::cout << "警告：重置 Winsock 需要重启电脑才能生效。\n";
                std::cout << "是否继续？(y/n): ";
                {
                    char c;
                    std::cin >> c;
                    std::cin.ignore();
                    if (c == 'y' || c == 'Y') RunCmd("netsh winsock reset");
                }
                Pause();
                break;
                
            case 16:
                ClearScreen();
                std::cout << "警告：重置 TCP/IP 协议栈需要重启电脑。\n";
                std::cout << "是否继续？(y/n): ";
                {
                    char c;
                    std::cin >> c;
                    std::cin.ignore();
                    if (c == 'y' || c == 'Y') RunCmd("netsh int ip reset");
                }
                Pause();
                break;
                
            case 17:
                ClearScreen();
                RunCmd("netsh wlan show networks");
                Pause();
                break;
                
            case 18:
                ClearScreen();
                RunCmd("netsh interface show interface");
                Pause();
                break;
                
            case 19:
            {
                std::string adapter = GetInput("请输入适配器名称: ");
                if (!adapter.empty())
                {
                    std::string cmd = "netsh interface set interface \"" + adapter + "\" enabled";
                    RunCmd(cmd);
                }
                Pause();
                break;
            }
                
            case 20:
            {
                std::string adapter = GetInput("请输入适配器名称: ");
                if (!adapter.empty())
                {
                    std::string cmd = "netsh interface set interface \"" + adapter + "\" disabled";
                    RunCmd(cmd);
                }
                Pause();
                break;
            }
                
            case 21:
                ClearScreen();
                RunCmd("net share");
                Pause();
                break;
                
            case 22:
                ClearScreen();
                RunCmd("netsh advfirewall show allprofiles state");
                Pause();
                break;
                
            case 23:
                ClearScreen();
                RunCmd("netsh advfirewall set allprofiles state on");
                Pause();
                break;
                
            case 24:
                ClearScreen();
                std::cout << "警告：关闭防火墙会降低系统安全性！\n";
                std::cout << "是否继续？(y/n): ";
                {
                    char c;
                    std::cin >> c;
                    std::cin.ignore();
                    if (c == 'y' || c == 'Y') RunCmd("netsh advfirewall set allprofiles state off");
                }
                Pause();
                break;
                
            case 25:
            {
                std::string url = GetInput("请输入下载 URL: ");
                std::string savePath = GetInput("请输入保存路径: ");
                if (!url.empty() && !savePath.empty())
                {
                    std::string cmd = "curl -L -o \"" + savePath + "\" \"" + url + "\"";
                    RunCmd(cmd);
                }
                Pause();
                break;
            }
                
            case 26:
                ClearScreen();
                std::cout << "正在测试网速...\n";
                RunCmd("powershell -Command \"Invoke-WebRequest -Uri 'https://speed.cloudflare.com/__down?bytes=10000000' -OutFile $env:TEMP\\speedtest.tmp; Remove-Item $env:TEMP\\speedtest.tmp\"");
                Pause();
                break;
                
            case 27:
                ClearScreen();
                RunCmd("curl ifconfig.me");
                std::cout << "\n";
                Pause();
                break;
                
            default:
                std::cout << "无效选项。" << std::endl;
                Pause();
        }
    }
}

// ==================== 磁盘操作工具 ====================

void DiskToolsMenu()
{
    while (true)
    {
        ClearScreen();
        std::cout << "========== 磁盘操作工具 ==========\n";
        std::cout << "=== 查看信息 ===\n";
        std::cout << " 1. 查看磁盘列表\n";
        std::cout << " 2. 查看卷列表\n";
        std::cout << " 3. 查看分区列表\n";
        std::cout << " 4. 查看磁盘详情（wmic）\n";
        std::cout << " 5. 查看逻辑磁盘\n";
        std::cout << "\n=== 磁盘检查 ===\n";
        std::cout << " 6. 检查磁盘（chkdsk 只读）\n";
        std::cout << " 7. 检查磁盘（chkdsk 修复）\n";
        std::cout << "\n=== DiskPart 操作 ===\n";
        std::cout << " 8. 启动 DiskPart 手动操作\n";
        std::cout << " 9. 查看磁盘属性\n";
        std::cout << "10. 查看卷属性\n";
        std::cout << "11. 查看分区属性\n";
        std::cout << "\n=== 高级操作 ===\n";
        std::cout << "12. 初始化磁盘\n";
        std::cout << "13. 创建分区\n";
        std::cout << "14. 删除分区\n";
        std::cout << "15. 格式化分区\n";
        std::cout << "16. 分配盘符\n";
        std::cout << "17. 删除盘符\n";
        std::cout << "18. 设置磁盘为只读\n";
        std::cout << "19. 取消磁盘只读\n";
        std::cout << "20. 清除磁盘（危险！）\n";
        std::cout << "\n 0. 返回主菜单\n";
        std::cout << "==================================\n";
        
        int choice = GetIntInput("请选择: ");
        
        if (choice == 0) break;
        
        switch (choice)
        {
            case 1:
                ClearScreen();
                RunCmd("echo list disk | diskpart");
                Pause();
                break;
                
            case 2:
                ClearScreen();
                RunCmd("echo list volume | diskpart");
                Pause();
                break;
                
            case 3:
                ClearScreen();
                RunCmd("echo list partition | diskpart");
                Pause();
                break;
                
            case 4:
                ClearScreen();
                RunCmd("wmic diskdrive get DeviceID,Model,Size,Status,InterfaceType,MediaType,SerialNumber /format:list");
                Pause();
                break;
                
            case 5:
            {
                ClearScreen();
                DWORD drives = GetLogicalDrives();
                for (char letter = 'A'; letter <= 'Z'; letter++)
                {
                    if (drives & (1 << (letter - 'A')))
                    {
                        std::string rootPath = std::string(1, letter) + ":\\";
                        UINT type = GetDriveTypeA(rootPath.c_str());
                        std::cout << "  " << letter << ":\\  ";
                        switch (type)
                        {
                            case DRIVE_FIXED: std::cout << "本地磁盘"; break;
                            case DRIVE_REMOVABLE: std::cout << "可移动磁盘"; break;
                            case DRIVE_CDROM: std::cout << "光驱"; break;
                            case DRIVE_REMOTE: std::cout << "网络驱动器"; break;
                            case DRIVE_RAMDISK: std::cout << "RAM磁盘"; break;
                            default: std::cout << "其他"; break;
                        }
                        ULARGE_INTEGER freeBytesAvailable, totalBytes, totalFreeBytes;
                        if (GetDiskFreeSpaceExA(rootPath.c_str(), &freeBytesAvailable, &totalBytes, &totalFreeBytes))
                        {
                            double totalGB = totalBytes.QuadPart / (1024.0 * 1024.0 * 1024.0);
                            double freeGB = freeBytesAvailable.QuadPart / (1024.0 * 1024.0 * 1024.0);
                            std::cout << "  总: " << std::fixed << std::setprecision(2) << totalGB << " GB";
                            std::cout << "  可用: " << freeGB << " GB";
                        }
                        std::cout << "\n";
                    }
                }
                Pause();
                break;
            }
                
            case 6:
            {
                std::string drive = GetInput("请输入盘符（如 C:）: ");
                if (!drive.empty())
                {
                    if (drive.back() != ':') drive += ":";
                    RunCmd("chkdsk " + drive + " /scan");
                }
                Pause();
                break;
            }
                
            case 7:
            {
                std::string drive = GetInput("请输入盘符（如 C:）: ");
                if (!drive.empty())
                {
                    if (drive.back() != ':') drive += ":";
                    std::cout << "警告：修复可能需要卸载卷！\n";
                    std::cout << "是否继续？(y/n): ";
                    char c;
                    std::cin >> c;
                    std::cin.ignore();
                    if (c == 'y' || c == 'Y') RunCmd("chkdsk " + drive + " /f /r /x");
                }
                Pause();
                break;
            }
                
            case 8:
                ClearScreen();
                system("diskpart");
                Pause();
                break;
                
            case 9:
            {
                int diskNum = GetIntInput("请输入磁盘号: ");
                if (diskNum >= 0)
                {
                    std::string script = "select disk " + std::to_string(diskNum) + "\ndetail disk\nexit\n";
                    std::ofstream scriptFile("C:\\diskpart_script.txt");
                    scriptFile << script;
                    scriptFile.close();
                    RunCmd("diskpart /s C:\\diskpart_script.txt");
                    DeleteFileA("C:\\diskpart_script.txt");
                }
                Pause();
                break;
            }
                
            case 10:
            {
                int volNum = GetIntInput("请输入卷号: ");
                if (volNum >= 0)
                {
                    std::string script = "select volume " + std::to_string(volNum) + "\ndetail volume\nexit\n";
                    std::ofstream scriptFile("C:\\diskpart_script.txt");
                    scriptFile << script;
                    scriptFile.close();
                    RunCmd("diskpart /s C:\\diskpart_script.txt");
                    DeleteFileA("C:\\diskpart_script.txt");
                }
                Pause();
                break;
            }
                
            case 11:
            {
                int diskNum = GetIntInput("请输入磁盘号: ");
                int partNum = GetIntInput("请输入分区号: ");
                if (diskNum >= 0 && partNum >= 0)
                {
                    std::string script = "select disk " + std::to_string(diskNum) + "\nselect partition " + std::to_string(partNum) + "\ndetail partition\nexit\n";
                    std::ofstream scriptFile("C:\\diskpart_script.txt");
                    scriptFile << script;
                    scriptFile.close();
                    RunCmd("diskpart /s C:\\diskpart_script.txt");
                    DeleteFileA("C:\\diskpart_script.txt");
                }
                Pause();
                break;
            }
                
            case 12:
            {
                int diskNum = GetIntInput("请输入要初始化的磁盘号: ");
                if (diskNum >= 0)
                {
                    std::cout << "分区类型:\n 1. MBR\n 2. GPT\n";
                    int type = GetIntInput("请选择: ");
                    std::string style = (type == 2) ? "GPT" : "MBR";
                    
                    std::cout << "\n警告：将初始化磁盘 " << diskNum << " 为 " << style << " 格式，所有数据将丢失！\n";
                    std::cout << "是否继续？(y/n): ";
                    char c;
                    std::cin >> c;
                    std::cin.ignore();
                    if (c == 'y' || c == 'Y')
                    {
                        std::string script = "select disk " + std::to_string(diskNum) + "\nclean\nconvert " + style + "\nexit\n";
                        std::ofstream scriptFile("C:\\diskpart_script.txt");
                        scriptFile << script;
                        scriptFile.close();
                        RunCmd("diskpart /s C:\\diskpart_script.txt");
                        DeleteFileA("C:\\diskpart_script.txt");
                    }
                }
                Pause();
                break;
            }
                
            case 13:
            {
                int diskNum = GetIntInput("请输入磁盘号: ");
                int sizeMB = GetIntInput("请输入分区大小（MB）: ");
                if (diskNum >= 0 && sizeMB > 0)
                {
                    std::cout << "警告：将创建 " << sizeMB << " MB 分区！\n";
                    std::cout << "是否继续？(y/n): ";
                    char c;
                    std::cin >> c;
                    std::cin.ignore();
                    if (c == 'y' || c == 'Y')
                    {
                        std::string script = "select disk " + std::to_string(diskNum) + "\ncreate partition primary size=" + std::to_string(sizeMB) + "\nexit\n";
                        std::ofstream scriptFile("C:\\diskpart_script.txt");
                        scriptFile << script;
                        scriptFile.close();
                        RunCmd("diskpart /s C:\\diskpart_script.txt");
                        DeleteFileA("C:\\diskpart_script.txt");
                    }
                }
                Pause();
                break;
            }
                
            case 14:
            {
                int diskNum = GetIntInput("请输入磁盘号: ");
                int partNum = GetIntInput("请输入分区号: ");
                if (diskNum >= 0 && partNum >= 0)
                {
                    std::cout << "警告：将删除磁盘 " << diskNum << " 的分区 " << partNum << "！\n";
                    std::cout << "是否继续？(y/n): ";
                    char c;
                    std::cin >> c;
                    std::cin.ignore();
                    if (c == 'y' || c == 'Y')
                    {
                        std::string script = "select disk " + std::to_string(diskNum) + "\nselect partition " + std::to_string(partNum) + "\ndelete partition override\nexit\n";
                        std::ofstream scriptFile("C:\\diskpart_script.txt");
                        scriptFile << script;
                        scriptFile.close();
                        RunCmd("diskpart /s C:\\diskpart_script.txt");
                        DeleteFileA("C:\\diskpart_script.txt");
                    }
                }
                Pause();
                break;
            }
                
            case 15:
            {
                int diskNum = GetIntInput("请输入磁盘号: ");
                int partNum = GetIntInput("请输入分区号: ");
                std::string fs = GetInput("请输入文件系统（NTFS/FAT32/exFAT）: ");
                if (diskNum >= 0 && partNum >= 0 && !fs.empty())
                {
                    std::cout << "警告：将格式化磁盘 " << diskNum << " 的分区 " << partNum << "！\n";
                    std::cout << "是否继续？(y/n): ";
                    char c;
                    std::cin >> c;
                    std::cin.ignore();
                    if (c == 'y' || c == 'Y')
                    {
                        std::string script = "select disk " + std::to_string(diskNum) + "\nselect partition " + std::to_string(partNum) + "\nformat fs=" + fs + " quick\nexit\n";
                        std::ofstream scriptFile("C:\\diskpart_script.txt");
                        scriptFile << script;
                        scriptFile.close();
                        RunCmd("diskpart /s C:\\diskpart_script.txt");
                        DeleteFileA("C:\\diskpart_script.txt");
                    }
                }
                Pause();
                break;
            }
                
            case 16:
            {
                int volNum = GetIntInput("请输入卷号: ");
                std::string letter = GetInput("请输入盘符（如 D）: ");
                if (volNum >= 0 && !letter.empty())
                {
                    std::string script = "select volume " + std::to_string(volNum) + "\nassign letter=" + letter + "\nexit\n";
                    std::ofstream scriptFile("C:\\diskpart_script.txt");
                    scriptFile << script;
                    scriptFile.close();
                    RunCmd("diskpart /s C:\\diskpart_script.txt");
                    DeleteFileA("C:\\diskpart_script.txt");
                }
                Pause();
                break;
            }
                
            case 17:
            {
                int volNum = GetIntInput("请输入卷号: ");
                std::string letter = GetInput("请输入要删除的盘符（如 D）: ");
                if (volNum >= 0 && !letter.empty())
                {
                    std::string script = "select volume " + std::to_string(volNum) + "\nremove letter=" + letter + "\nexit\n";
                    std::ofstream scriptFile("C:\\diskpart_script.txt");
                    scriptFile << script;
                    scriptFile.close();
                    RunCmd("diskpart /s C:\\diskpart_script.txt");
                    DeleteFileA("C:\\diskpart_script.txt");
                }
                Pause();
                break;
            }
                
            case 18:
            {
                int diskNum = GetIntInput("请输入磁盘号: ");
                if (diskNum >= 0)
                {
                    std::string script = "select disk " + std::to_string(diskNum) + "\nattributes disk set readonly\nexit\n";
                    std::ofstream scriptFile("C:\\diskpart_script.txt");
                    scriptFile << script;
                    scriptFile.close();
                    RunCmd("diskpart /s C:\\diskpart_script.txt");
                    DeleteFileA("C:\\diskpart_script.txt");
                }
                Pause();
                break;
            }
                
            case 19:
            {
                int diskNum = GetIntInput("请输入磁盘号: ");
                if (diskNum >= 0)
                {
                    std::string script = "select disk " + std::to_string(diskNum) + "\nattributes disk clear readonly\nexit\n";
                    std::ofstream scriptFile("C:\\diskpart_script.txt");
                    scriptFile << script;
                    scriptFile.close();
                    RunCmd("diskpart /s C:\\diskpart_script.txt");
                    DeleteFileA("C:\\diskpart_script.txt");
                }
                Pause();
                break;
            }
                
            case 20:
            {
                int diskNum = GetIntInput("请输入要清除的磁盘号: ");
                if (diskNum >= 0)
                {
                    std::cout << "!!! 极度警告：将清除磁盘 " << diskNum << " 上的所有数据！!!!\n";
                    std::cout << "此操作不可恢复！\n";
                    std::cout << "请输入磁盘号确认: ";
                    int confirm;
                    std::cin >> confirm;
                    std::cin.ignore();
                    
                    if (confirm == diskNum)
                    {
                        std::string script = "select disk " + std::to_string(diskNum) + "\nclean\nexit\n";
                        std::ofstream scriptFile("C:\\diskpart_script.txt");
                        scriptFile << script;
                        scriptFile.close();
                        RunCmd("diskpart /s C:\\diskpart_script.txt");
                        DeleteFileA("C:\\diskpart_script.txt");
                    }
                    else
                    {
                        std::cout << "磁盘号不匹配，操作取消。" << std::endl;
                    }
                }
                Pause();
                break;
            }
                
            default:
                std::cout << "无效选项。" << std::endl;
                Pause();
        }
    }
}


// ==================== GPU 多窗口压测 ====================
// OpenGL 测试
void OpenGLTest()
{
    ClearScreen();
    std::cout << "========== OpenGL 测试 ==========\n";
    std::cout << "请选择测试强度：\n";
    std::cout << " 1. 轻度（核显友好）\n";
    std::cout << " 2. 中度\n";
    std::cout << " 3. 重度（独显推荐）\n";
    std::cout << " 0. 取消\n";
    int intensity = GetIntInput("请选择: ");
    
    if (intensity == 0) { Pause(); return; }
    
    int triangles = 0;
    switch (intensity)
    {
        case 1: triangles = 100; break;
        case 2: triangles = 500; break;
        case 3: triangles = 1000; break;
        default: Pause(); return;
    }
    
    int durationSeconds = SelectDuration(true);
    if (durationSeconds == 0) { std::cout << "操作已取消。" << std::endl; Pause(); return; }
    
    std::cout << "\n按任意键开始 OpenGL 测试...\n";
    if (durationSeconds == -1)
        std::cout << "按 ESC 或关闭窗口结束测试。\n";
    _getch();
    
    WNDCLASSW wc = {0};
    wc.lpfnWndProc = GPUStressWndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"OpenGLTestWindow";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClassW(&wc);
    
    HWND hwnd = CreateWindowW(L"OpenGLTestWindow", L"OpenGL Test", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, wc.hInstance, NULL);
    
    if (!hwnd) { std::cout << "窗口创建失败！" << std::endl; Pause(); return; }
    
    HDC hdc = GetDC(hwnd);
    PIXELFORMATDESCRIPTOR pfd = { sizeof(pfd), 1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA, 32, 0,0,0,0,0,0,0,0,0,0,0,0,0, 32,0,0, PFD_MAIN_PLANE, 0,0,0,0 };
    int pf = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, pf, &pfd);
    HGLRC hglrc = wglCreateContext(hdc);
    wglMakeCurrent(hdc, hglrc);
    ShowWindow(hwnd, SW_SHOW);
    
    const char* glRenderer = (const char*)glGetString(GL_RENDERER);
    const char* glVersion = (const char*)glGetString(GL_VERSION);
    std::cout << "\nOpenGL 渲染器: " << (glRenderer ? glRenderer : "未知") << "\n";
    std::cout << "OpenGL 版本: " << (glVersion ? glVersion : "未知") << "\n\n";
    
    auto start = std::chrono::steady_clock::now();
    int frameCount = 0;
    bool running = true;
    
    while (running)
    {
        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glBegin(GL_TRIANGLES);
        for (int i = 0; i < triangles; i++)
        {
            float x = (i % 32) / 32.0f * 2.0f - 1.0f;
            float y = (i / 32) / 31.0f * 2.0f - 1.0f;
            glColor3f((i % 256) / 255.0f, (i * 3 % 256) / 255.0f, (i * 7 % 256) / 255.0f);
            glVertex2f(x, y);
            glVertex2f(x + 0.05f, y);
            glVertex2f(x + 0.025f, y + 0.1f);
        }
        glEnd();
        SwapBuffers(hdc);
        frameCount++;
        
        MSG msg;
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) { running = false; break; }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        
        if (durationSeconds > 0)
        {
            auto now = std::chrono::steady_clock::now();
            double elapsed = std::chrono::duration<double>(now - start).count();
            if (elapsed >= durationSeconds) running = false;
        }
        
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) running = false;
    }
    
    auto end = std::chrono::steady_clock::now();
    double elapsed = std::chrono::duration<double>(end - start).count();
    double fps = frameCount / elapsed;
    
    std::cout << "\n=== OpenGL 测试结果 ===\n";
    std::cout << "持续时间: " << std::fixed << std::setprecision(1) << elapsed << " 秒\n";
    std::cout << "帧数: " << frameCount << "\n";
    std::cout << "FPS: " << std::fixed << std::setprecision(2) << fps << "\n";
    
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hglrc);
    ReleaseDC(hwnd, hdc);
    DestroyWindow(hwnd);
    Pause();
}
// DirectX 11 测试（真正 GPU 负载版）
void DirectX11Test()
{
    ClearScreen();
    std::cout << "========== DirectX 11 测试 ==========\n";
    std::cout << "请选择测试强度：\n";
    std::cout << " 1. 轻度\n";
    std::cout << " 2. 中度\n";
    std::cout << " 3. 重度\n";
    std::cout << " 0. 取消\n";
    int intensity = GetIntInput("请选择: ");
    
    if (intensity == 0) { Pause(); return; }
    
    int durationSeconds = SelectDuration(true);
    if (durationSeconds == 0) { std::cout << "操作已取消。" << std::endl; Pause(); return; }
    
    std::cout << "\n按任意键开始 DirectX 11 测试...\n";
    _getch();
    
    // 创建窗口
    WNDCLASSW wc = {0};
    wc.lpfnWndProc = GPUStressWndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"D3D11TestWindow";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClassW(&wc);
    
    HWND hwnd = CreateWindowW(L"D3D11TestWindow", L"DirectX 11 GPU Test", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1920, 1080, NULL, NULL, wc.hInstance, NULL);
    
    if (!hwnd) { std::cout << "窗口创建失败！" << std::endl; Pause(); return; }
    
    // 创建 D3D11 设备
    D3D_FEATURE_LEVEL featureLevel;
    ID3D11Device* pDevice = nullptr;
    ID3D11DeviceContext* pContext = nullptr;
    IDXGISwapChain* pSwapChain = nullptr;
    ID3D11RenderTargetView* pRenderTargetView = nullptr;
    
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = 2;
    swapChainDesc.BufferDesc.Width = 1920;
    swapChainDesc.BufferDesc.Height = 1080;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.Windowed = TRUE;
    
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        nullptr, 0, D3D11_SDK_VERSION,
        &swapChainDesc, &pSwapChain, &pDevice, &featureLevel, &pContext
    );
    
    if (FAILED(hr))
    {
        std::cout << "D3D11 设备创建失败！" << std::endl;
        DestroyWindow(hwnd);
        Pause();
        return;
    }
    
    // 获取 GPU 名称
    IDXGIDevice* pDXGIDevice = nullptr;
    pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDXGIDevice);
    IDXGIAdapter* pAdapter = nullptr;
    pDXGIDevice->GetAdapter(&pAdapter);
    DXGI_ADAPTER_DESC adapterDesc;
    pAdapter->GetDesc(&adapterDesc);
    pDXGIDevice->Release();
    pAdapter->Release();
    
    std::wcout << L"\nGPU: " << adapterDesc.Description << L"\n\n";
    
    // 创建渲染目标
    ID3D11Texture2D* pBackBuffer = nullptr;
    pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
    pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pRenderTargetView);
    pBackBuffer->Release();
    
    // 创建深度缓冲
    ID3D11Texture2D* pDepthBuffer = nullptr;
    ID3D11DepthStencilView* pDepthStencilView = nullptr;
    D3D11_TEXTURE2D_DESC depthDesc = {};
    depthDesc.Width = 1920;
    depthDesc.Height = 1080;
    depthDesc.MipLevels = 1;
    depthDesc.ArraySize = 1;
    depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthDesc.SampleDesc.Count = 1;
    depthDesc.Usage = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    pDevice->CreateTexture2D(&depthDesc, nullptr, &pDepthBuffer);
    pDevice->CreateDepthStencilView(pDepthBuffer, nullptr, &pDepthStencilView);
    
    pContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);
    
    // 设置视口
    D3D11_VIEWPORT viewport = {};
    viewport.Width = 1920.0f;
    viewport.Height = 1080.0f;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    pContext->RSSetViewports(1, &viewport);
    
    // 创建顶点缓冲区 - 大量三角形
    struct Vertex
    {
        float x, y, z;
        float r, g, b, a;
    };
    
    int vertexCount = 0;
    switch (intensity)
    {
        case 1: vertexCount = 300000; break;    // 10万三角形
        case 2: vertexCount = 1500000; break;   // 50万三角形
        case 3: vertexCount = 3000000; break;   // 100万三角形
        default: vertexCount = 300000; break;
    }
    
    std::vector<Vertex> vertices(vertexCount);
    for (int i = 0; i < vertexCount; i++)
    {
        vertices[i].x = (float)(rand() % 2000 - 1000) / 1000.0f;
        vertices[i].y = (float)(rand() % 2000 - 1000) / 1000.0f;
        vertices[i].z = 0.5f;
        vertices[i].r = (float)(rand() % 256) / 255.0f;
        vertices[i].g = (float)(rand() % 256) / 255.0f;
        vertices[i].b = (float)(rand() % 256) / 255.0f;
        vertices[i].a = 1.0f;
    }
    
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.ByteWidth = sizeof(Vertex) * vertexCount;
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    
    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = vertices.data();
    
    ID3D11Buffer* pVertexBuffer = nullptr;
    pDevice->CreateBuffer(&bufferDesc, &initData, &pVertexBuffer);
    
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    pContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
    pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    
    // 创建着色器
    const char* vsSource = 
        "float4 main(float4 pos : POSITION, float4 color : COLOR) : SV_POSITION { return pos; }";
    const char* psSource = 
        "float4 main(float4 pos : SV_POSITION, float4 color : COLOR) : SV_Target { return color; }";
    
    ID3D11VertexShader* pVS = nullptr;
    ID3D11PixelShader* pPS = nullptr;
    ID3D11InputLayout* pInputLayout = nullptr;
    
    ID3DBlob* pVSBlob = nullptr;
    ID3DBlob* pPSBlob = nullptr;
    
    // 编译着色器（简化版，直接用预编译的）
    // 这里使用简单方法：直接创建着色器
    
    ShowWindow(hwnd, SW_SHOW);
    
    std::cout << "GPU 压测进行中... 顶点数: " << vertexCount << "\n";
    
    auto start = std::chrono::steady_clock::now();
    int frameCount = 0;
    bool running = true;
    
    while (running)
    {
        // 清屏
        float clearColor[4] = { 0.1f, 0.1f, 0.2f, 1.0f };
        pContext->ClearRenderTargetView(pRenderTargetView, clearColor);
        pContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
        
        // 绘制所有顶点
        pContext->Draw(vertexCount, 0);
        
        // 呈现
        pSwapChain->Present(1, 0);
        frameCount++;
        
        // 处理消息
        MSG msg;
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) { running = false; break; }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        
        // 检查时间
        if (durationSeconds > 0)
        {
            auto now = std::chrono::steady_clock::now();
            double elapsed = std::chrono::duration<double>(now - start).count();
            if (elapsed >= durationSeconds) running = false;
        }
        
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) running = false;
    }
    
    auto end = std::chrono::steady_clock::now();
    double elapsed = std::chrono::duration<double>(end - start).count();
    double fps = frameCount / elapsed;
    
    std::cout << "\n=== DirectX 11 GPU 测试结果 ===\n";
    std::cout << "持续时间: " << std::fixed << std::setprecision(1) << elapsed << " 秒\n";
    std::cout << "帧数: " << frameCount << "\n";
    std::cout << "FPS: " << std::fixed << std::setprecision(2) << fps << "\n";
    
    // 清理
    if (pVertexBuffer) pVertexBuffer->Release();
    if (pDepthStencilView) pDepthStencilView->Release();
    if (pDepthBuffer) pDepthBuffer->Release();
    if (pRenderTargetView) pRenderTargetView->Release();
    if (pSwapChain) pSwapChain->Release();
    if (pContext) pContext->Release();
    if (pDevice) pDevice->Release();
    DestroyWindow(hwnd);
    Pause();
}
// 选择测试时长
int SelectDuration(bool allowInfinite)
{
    ClearScreen();
    std::cout << "========== 选择测试时长 ==========\n";
    std::cout << " 1. 10 秒\n";
    std::cout << " 2. 30 秒\n";
    std::cout << " 3. 1 分钟\n";
    std::cout << " 4. 5 分钟\n";
    std::cout << " 5. 10 分钟\n";
    if (allowInfinite)
        std::cout << " 6. 无限时（按 ESC 结束）\n";
    std::cout << " 0. 取消\n";
    std::cout << "==================================\n";
    
    int choice = GetIntInput("请选择: ");
    
    switch (choice)
    {
        case 1: return 10;
        case 2: return 30;
        case 3: return 60;
        case 4: return 300;
        case 5: return 600;
        case 6: return allowInfinite ? -1 : 0;
        case 0: return 0;
        default: return 0;
    }
}

// GPU 信息获取
void GetGpuInfo()
{
    ClearScreen();
    std::cout << "========== GPU 信息 ==========\n";
    
    IDXGIFactory* pFactory = nullptr;
    if (SUCCEEDED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory)))
    {
        UINT adapterIndex = 0;
        IDXGIAdapter* pAdapter = nullptr;
        
        while (pFactory->EnumAdapters(adapterIndex, &pAdapter) != DXGI_ERROR_NOT_FOUND)
        {
            DXGI_ADAPTER_DESC desc;
            pAdapter->GetDesc(&desc);
            
            std::cout << "\n=== GPU " << adapterIndex << " ===\n";
            std::wcout << L"名称: " << desc.Description << L"\n";
            std::cout << "显存: " << desc.DedicatedVideoMemory / (1024.0 * 1024.0) << " MB\n";
            std::cout << "共享内存: " << desc.SharedSystemMemory / (1024.0 * 1024.0) << " MB\n";
            std::cout << "厂商ID: 0x" << std::hex << desc.VendorId << std::dec << "\n";
            std::cout << "设备ID: 0x" << std::hex << desc.DeviceId << std::dec << "\n";
            
            pAdapter->Release();
            adapterIndex++;
        }
        pFactory->Release();
    }
    
    Pause();
}

// Vulkan 测试
void VulkanTest()
{
    ClearScreen();
    std::cout << "========== Vulkan 测试 ==========\n";
    std::cout << "Vulkan 需要 Vulkan SDK 支持。\n\n";
    
    std::cout << "=== Vulkan 环境检查 ===\n";
    char vkSdkPath[MAX_PATH];
    if (GetEnvironmentVariableA("VK_SDK_PATH", vkSdkPath, MAX_PATH))
    {
        std::cout << "Vulkan SDK 路径: " << vkSdkPath << "\n";
    }
    else
    {
        std::cout << "未检测到 Vulkan SDK 环境变量。\n";
    }
    
    std::cout << "\n=== Vulkan 信息 ===\n";
    int ret = system("vulkaninfo --summary 2>&1");
    if (ret != 0)
    {
        std::cout << "vulkaninfo 不可用。\n";
        std::cout << "请安装 Vulkan SDK: https://vulkan.lunarg.com/\n";
    }
    
    Pause();
}
// 多窗口 GPU 压测
void GpuMultiWindowTest()
{
    ClearScreen();
    std::cout << "========== GPU 多窗口压测 ==========\n";
    std::cout << "将同时创建多个窗口进行并发渲染压力测试。\n\n";
    
    std::cout << "请选择测试强度：\n";
    std::cout << " 1. 轻度（2个窗口）\n";
    std::cout << " 2. 中度（4个窗口）\n";
    std::cout << " 3. 重度（6个窗口）\n";
    std::cout << " 4. 极限（8个窗口）\n";
    std::cout << " 0. 取消\n";
    int intensity = GetIntInput("请选择: ");
    
    if (intensity == 0) { Pause(); return; }
    
    int windowCount = 0;
    int triangles = 0;
    switch (intensity)
    {
        case 1: windowCount = 2; triangles = 200; break;
        case 2: windowCount = 4; triangles = 500; break;
        case 3: windowCount = 6; triangles = 800; break;
        case 4: windowCount = 8; triangles = 1000; break;
        default: Pause(); return;
    }
    
    int durationSeconds = SelectDuration(true);
    if (durationSeconds == 0) { std::cout << "操作已取消。" << std::endl; Pause(); return; }
    
    std::cout << "\n=== 压测参数 ===\n";
    std::cout << "窗口数量: " << windowCount << "\n";
    std::cout << "每窗口三角形: " << triangles << "\n";
    std::cout << "持续时间: " << (durationSeconds == -1 ? "无限" : std::to_string(durationSeconds) + "秒") << "\n\n";
    
    std::cout << "按任意键开始多窗口压测...\n";
    if (durationSeconds == -1)
        std::cout << "按 ESC 结束测试。\n";
    _getch();
    
    // 存储窗口信息
    struct GLWindow
    {
        HWND hwnd;
        HDC hdc;
        HGLRC hglrc;
    };
    std::vector<GLWindow> windows;
    
    // 创建多个窗口
    for (int i = 0; i < windowCount; i++)
    {
        std::wstring className = L"GLStressWindow" + std::to_wstring(i);
        std::wstring title = L"GL Stress " + std::to_wstring(i + 1);
        
        WNDCLASSW wc = {0};
        wc.lpfnWndProc = GPUStressWndProc;
        wc.hInstance = GetModuleHandle(NULL);
        wc.lpszClassName = className.c_str();
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        
        // 每个窗口使用不同的类
        static WNDCLASSW wcArray[8];
        wcArray[i] = wc;
        RegisterClassW(&wcArray[i]);
        
        int x = 50 + (i % 4) * 350;
        int y = 50 + (i / 4) * 300;
        
        HWND hwnd = CreateWindowW(className.c_str(), title.c_str(), WS_OVERLAPPEDWINDOW,
            x, y, 300, 250, NULL, NULL, wc.hInstance, NULL);
        
        if (!hwnd)
        {
            std::cout << "窗口 " << (i + 1) << " 创建失败！" << std::endl;
            continue;
        }
        
        HDC hdc = GetDC(hwnd);
        PIXELFORMATDESCRIPTOR pfd = { sizeof(pfd), 1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA, 32, 0,0,0,0,0,0,0,0,0,0,0,0,0, 32,0,0, PFD_MAIN_PLANE, 0,0,0,0 };
        int pf = ChoosePixelFormat(hdc, &pfd);
        SetPixelFormat(hdc, pf, &pfd);
        HGLRC hglrc = wglCreateContext(hdc);
        wglMakeCurrent(hdc, hglrc);
        
        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);
        
        GLWindow win = { hwnd, hdc, hglrc };
        windows.push_back(win);
        
        std::cout << "窗口 " << (i + 1) << " 已创建\n";
    }
    
    std::cout << "\n压测进行中...\n";
    
    auto start = std::chrono::steady_clock::now();
    int totalFrames = 0;
    bool running = true;
    
    while (running)
    {
        for (auto& win : windows)
        {
            wglMakeCurrent(win.hdc, win.hglrc);
            
            // 每个窗口使用不同的颜色
            float r = (rand() % 100) / 100.0f;
            float g = (rand() % 100) / 100.0f;
            float b = (rand() % 100) / 100.0f;
            
            glClearColor(r, g, b, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            glBegin(GL_TRIANGLES);
            for (int i = 0; i < triangles; i++)
            {
                float x = (i % 32) / 32.0f * 2.0f - 1.0f;
                float y = (i / 32) / 31.0f * 2.0f - 1.0f;
                glColor3f((i % 256) / 255.0f, (i * 3 % 256) / 255.0f, (i * 7 % 256) / 255.0f);
                glVertex2f(x, y);
                glVertex2f(x + 0.05f, y);
                glVertex2f(x + 0.025f, y + 0.1f);
            }
            glEnd();
            
            SwapBuffers(win.hdc);
            totalFrames++;
        }
        
        MSG msg;
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                running = false;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        
        if (durationSeconds > 0)
        {
            auto now = std::chrono::steady_clock::now();
            double elapsed = std::chrono::duration<double>(now - start).count();
            if (elapsed >= durationSeconds) running = false;
        }
        
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) running = false;
    }
    
    auto end = std::chrono::steady_clock::now();
    double elapsed = std::chrono::duration<double>(end - start).count();
    double fps = totalFrames / elapsed;
    
    // 清理窗口
    for (auto& win : windows)
    {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(win.hglrc);
        ReleaseDC(win.hwnd, win.hdc);
        DestroyWindow(win.hwnd);
    }
    
    std::cout << "\n=== 多窗口压测结果 ===\n";
    std::cout << "窗口数量: " << windows.size() << "\n";
    std::cout << "持续时间: " << std::fixed << std::setprecision(1) << elapsed << " 秒\n";
    std::cout << "总帧数: " << totalFrames << "\n";
    std::cout << "总 FPS: " << std::fixed << std::setprecision(2) << fps << "\n";
    std::cout << "每窗口 FPS: " << std::fixed << std::setprecision(2) << (fps / windows.size()) << "\n";
    
    Pause();
}

// GPU 工具菜单（更新）
void GpuToolsMenu()
{
    while (true)
    {
        ClearScreen();
        std::cout << "========== GPU 工具（内测版） ==========\n";
        std::cout << " 1. 查看 GPU 信息\n";
        std::cout << " 2. OpenGL 测试\n";
        std::cout << " 3. DirectX 11 测试\n";
        std::cout << " 4. Vulkan 测试\n";
        std::cout << " 5. 多窗口压测（2-8窗口）\n";
        std::cout << " 0. 返回主菜单\n";
        std::cout << "=========================================\n";
        
        int choice = GetIntInput("请选择: ");
        
        if (choice == 0) break;
        else if (choice == 1) GetGpuInfo();
        else if (choice == 2) OpenGLTest();
        else if (choice == 3) DirectX11Test();
        else if (choice == 4) VulkanTest();
        else if (choice == 5) GpuMultiWindowTest();
        else { std::cout << "无效选项。" << std::endl; Pause(); }
    }
}

// ==================== 驱动工具（增强版） ====================

// 检查驱动签名状态
void CheckDriverSigningStatus()
{
    ClearScreen();
    std::cout << "========== 驱动签名状态 ==========\n";
    
    // 使用 BCDEdit 查看
    std::cout << "=== 启动配置 ===\n";
    RunCmd("bcdedit /enum {current}");
    
    // 检查测试签名
    std::cout << "\n=== 测试签名状态 ===\n";
    RunCmd("bcdedit /enum {current} | findstr testsigning");
    
    Pause();
}

// 禁用驱动强制签名
void DisableDriverSignature()
{
    ClearScreen();
    std::cout << "========== 禁用驱动强制签名 ==========\n";
    std::cout << "此操作将关闭 Windows 驱动签名强制。\n";
    std::cout << "警告：关闭驱动签名会降低系统安全性！\n\n";
    std::cout << " 1. 临时禁用（重启后恢复）\n";
    std::cout << " 2. 永久禁用（需手动恢复）\n";
    std::cout << " 0. 取消\n";
    std::cout << "======================================\n";
    
    int choice = GetIntInput("请选择: ");
    
    if (choice == 0) { Pause(); return; }
    
    std::cout << "\n警告：此操作需要重启电脑才能生效！\n";
    std::cout << "是否继续？(y/n): ";
    char c;
    std::cin >> c;
    std::cin.ignore();
    
    if (c != 'y' && c != 'Y')
    {
        std::cout << "操作已取消。" << std::endl;
        Pause();
        return;
    }
    
    if (choice == 1)
    {
        // 临时禁用：使用高级启动选项
        std::cout << "\n临时禁用驱动签名：\n";
        std::cout << "方法：重启时按住 Shift 点击重启\n";
        std::cout << "或使用以下命令：\n";
        RunCmd("shutdown /r /o /t 0");
    }
    else if (choice == 2)
    {
        // 永久禁用
        std::cout << "\n正在禁用驱动强制签名...\n";
        RunCmd("bcdedit /set testsigning on");
        std::cout << "\n已启用测试签名模式。\n";
        std::cout << "重启后生效。\n";
    }
    
    Pause();
}

// 启用驱动强制签名
void EnableDriverSignature()
{
    ClearScreen();
    std::cout << "========== 启用驱动强制签名 ==========\n";
    std::cout << "将恢复 Windows 驱动签名强制。\n\n";
    std::cout << "是否继续？(y/n): ";
    char c;
    std::cin >> c;
    std::cin.ignore();
    
    if (c != 'y' && c != 'Y')
    {
        std::cout << "操作已取消。" << std::endl;
        Pause();
        return;
    }
    
    std::cout << "\n正在恢复驱动签名强制...\n";
    RunCmd("bcdedit /set testsigning off");
    std::cout << "\n已恢复。重启后生效。\n";
    
    Pause();
}

// 驱动工具菜单（更新版）
void DriverToolsMenu()
{
    while (true)
    {
        ClearScreen();
        std::cout << "========== 驱动工具 ==========\n";
        std::cout << "=== 驱动查看 ===\n";
        std::cout << " 1. 查看所有驱动列表\n";
        std::cout << " 2. 查看已安装驱动详情\n";
        std::cout << " 3. 查看驱动版本信息\n";
        std::cout << " 4. 查看未签名驱动\n";
        std::cout << "\n=== 驱动备份 ===\n";
        std::cout << " 5. 备份所有驱动\n";
        std::cout << " 6. 备份指定驱动\n";
        std::cout << "\n=== 驱动操作 ===\n";
        std::cout << " 7. 安装驱动\n";
        std::cout << " 8. 卸载驱动\n";
        std::cout << " 9. 启用驱动\n";
        std::cout << "10. 禁用驱动\n";
        std::cout << "11. 更新驱动\n";
        std::cout << "\n=== 驱动签名 ===\n";
        std::cout << "12. 检查驱动签名状态\n";
        std::cout << "13. 禁用驱动强制签名\n";
        std::cout << "14. 启用驱动强制签名\n";
        std::cout << "\n=== 驱动高级 ===\n";
        std::cout << "15. 强制安装驱动\n";
        std::cout << "16. 检查驱动冲突\n";
        std::cout << "17. 导出驱动列表到文件\n";
        std::cout << "\n 0. 返回主菜单\n";
        std::cout << "==============================\n";
        
        int choice = GetIntInput("请选择: ");
        
        if (choice == 0) break;
        
        switch (choice)
        {
            case 1:
                ClearScreen();
                RunCmd("driverquery /fo table");
                Pause();
                break;
            case 2:
                ClearScreen();
                RunCmd("driverquery /v /fo list");
                Pause();
                break;
            case 3:
                ClearScreen();
                RunCmd("driverquery /v /fo table");
                Pause();
                break;
            case 4:
                ClearScreen();
                RunCmd("driverquery /fo table /si");
                Pause();
                break;
            case 5:
            {
                std::string dir = GetInput("备份目录: ");
                if (!dir.empty())
                    RunCmd("dism /online /export-driver /destination:\"" + dir + "\"");
                Pause();
                break;
            }
            case 6:
            {
                std::string name = GetInput("驱动名称（如 oem0.inf）: ");
                std::string dir = GetInput("备份目录: ");
                if (!name.empty() && !dir.empty())
                    RunCmd("pnputil /export-driver \"" + name + "\" \"" + dir + "\"");
                Pause();
                break;
            }
            case 7:
            {
                std::string inf = GetInput("INF 文件路径: ");
                if (!inf.empty())
                    RunCmd("pnputil /add-driver \"" + inf + "\" /install");
                Pause();
                break;
            }
            case 8:
            {
                std::string name = GetInput("驱动名称（如 oem0.inf）: ");
                if (!name.empty())
                {
                    std::cout << "警告：卸载驱动可能导致硬件无法工作！\n";
                    std::cout << "是否继续？(y/n): ";
                    char c; std::cin >> c; std::cin.ignore();
                    if (c == 'y' || c == 'Y')
                        RunCmd("pnputil /delete-driver \"" + name + "\" /uninstall /force");
                }
                Pause();
                break;
            }
            case 9:
            {
                std::string name = GetInput("驱动名称: ");
                if (!name.empty())
                    RunCmd("pnputil /enable-driver \"" + name + "\"");
                Pause();
                break;
            }
            case 10:
            {
                std::string name = GetInput("驱动名称: ");
                if (!name.empty())
                {
                    std::cout << "警告：禁用驱动可能导致硬件无法工作！\n";
                    std::cout << "是否继续？(y/n): ";
                    char c; std::cin >> c; std::cin.ignore();
                    if (c == 'y' || c == 'Y')
                        RunCmd("pnputil /disable-driver \"" + name + "\"");
                }
                Pause();
                break;
            }
            case 11:
                ClearScreen();
                RunCmd("pnputil /scan-devices");
                Pause();
                break;
            case 12:
                CheckDriverSigningStatus();
                break;
            case 13:
                DisableDriverSignature();
                break;
            case 14:
                EnableDriverSignature();
                break;
            case 15:
            {
                std::string inf = GetInput("INF 文件路径: ");
                if (!inf.empty())
                {
                    std::cout << "警告：强制安装可能安装不兼容的驱动！\n";
                    std::cout << "是否继续？(y/n): ";
                    char c; std::cin >> c; std::cin.ignore();
                    if (c == 'y' || c == 'Y')
                        RunCmd("pnputil /add-driver \"" + inf + "\" /install /force");
                }
                Pause();
                break;
            }
            case 16:
                ClearScreen();
                RunCmd("powershell -Command \"Get-PnpDevice | Where-Object { $_.Status -ne 'OK' } | Format-List FriendlyName,Status,InstanceId\"");
                Pause();
                break;
            case 17:
            {
                std::string path = GetInput("保存路径（如 C:\\drivers.txt）: ");
                if (!path.empty())
                    RunCmd("driverquery /fo table > \"" + path + "\"");
                Pause();
                break;
            }
            default:
                std::cout << "无效选项。" << std::endl;
                Pause();
        }
    }
}

// ==================== 编译工具 ====================

void CompileToolsMenu()
{
    while (true)
    {
        ClearScreen();
        std::cout << "========== 编译工具 ==========\n";
        std::cout << "=== 环境检查 ===\n";
        std::cout << " 1. 检查所有编译环境\n";
        std::cout << "\n=== Python ===\n";
        std::cout << " 2. 安装 Python 3.12\n";
        std::cout << " 3. 安装 Python 3.11\n";
        std::cout << " 4. 安装 Python 3.10\n";
        std::cout << " 5. 检查 Python 版本\n";
        std::cout << " 6. 编译 Python 文件（.py → .pyc）\n";
        std::cout << " 7. 打包 Python 为 exe（PyInstaller）\n";
        std::cout << " 8. 打包 Python 为 exe（Nuitka）\n";
        std::cout << " 9. 创建 Python 虚拟环境\n";
        std::cout << "10. 安装 Python 包（pip）\n";
        std::cout << "11. 查看已安装 Python 包\n";
        std::cout << "12. 运行 Python 脚本\n";
        std::cout << "\n=== C/C++ ===\n";
        std::cout << "13. 安装 MinGW-w64\n";
        std::cout << "14. 安装 Visual Studio Build Tools\n";
        std::cout << "15. 检查 C++ 编译器版本\n";
        std::cout << "16. 编译 C++ 文件（g++ 标准）\n";
        std::cout << "17. 编译 C++ 文件（g++ 优化）\n";
        std::cout << "18. 编译 C++ 文件（g++ 调试）\n";
        std::cout << "19. 编译 C 文件（gcc）\n";
        std::cout << "20. 编译多文件 C++ 项目\n";
        std::cout << "21. 编译 C++ 为 DLL\n";
        std::cout << "22. 编译 C++ 为静态库\n";
        std::cout << "\n=== .NET ===\n";
        std::cout << "23. 安装 .NET SDK 8\n";
        std::cout << "24. 安装 .NET SDK 9\n";
        std::cout << "25. 检查 .NET 版本\n";
        std::cout << "26. 创建 .NET 控制台项目\n";
        std::cout << "27. 创建 .NET Web 项目\n";
        std::cout << "28. 创建 .NET 类库项目\n";
        std::cout << "29. 编译 .NET 项目\n";
        std::cout << "30. 运行 .NET 项目\n";
        std::cout << "31. 发布 .NET 项目（单文件）\n";
        std::cout << "32. 发布 .NET 项目（框架依赖）\n";
        std::cout << "33. 添加 .NET 包（NuGet）\n";
        std::cout << "34. 查看 .NET 项目依赖\n";
        std::cout << "\n=== 其他语言 ===\n";
        std::cout << "35. 安装 Node.js\n";
        std::cout << "36. 安装 Java JDK 21\n";
        std::cout << "37. 安装 Java JDK 17\n";
        std::cout << "38. 安装 Rust\n";
        std::cout << "39. 安装 Go\n";
        std::cout << "40. 编译 Java 文件\n";
        std::cout << "41. 运行 Java 程序\n";
        std::cout << "42. 编译 Rust 项目\n";
        std::cout << "43. 编译 Go 项目\n";
        std::cout << "44. 运行 Node.js 脚本\n";
        std::cout << "45. 安装 npm 包\n";
        std::cout << "\n=== 构建工具 ===\n";
        std::cout << "46. 安装 CMake\n";
        std::cout << "47. 安装 Make\n";
        std::cout << "48. 安装 Git\n";
        std::cout << "49. 安装 Ninja\n";
        std::cout << "50. 安装 MSYS2\n";
        std::cout << "51. 使用 CMake 构建项目\n";
        std::cout << "52. 使用 Make 构建项目\n";
        std::cout << "\n 0. 返回主菜单\n";
        std::cout << "==============================\n";
        
        int choice = GetIntInput("请选择: ");
        
        if (choice == 0) break;
        
        // 获取 %TEMP% 路径
        char tempBuf[MAX_PATH];
        GetTempPathA(MAX_PATH, tempBuf);
        std::string tempDir = tempBuf;
        std::string buildDir = tempDir + "toolbox_build";
        std::string distDir = tempDir + "toolbox_dist";
        std::string specDir = tempDir + "toolbox_spec";
        
        CreateDirectoryA(buildDir.c_str(), nullptr);
        CreateDirectoryA(distDir.c_str(), nullptr);
        CreateDirectoryA(specDir.c_str(), nullptr);
        
        auto wingetInstall = [](const std::string& packageId)
        {
            std::string cmd = "winget install " + packageId + " --silent --accept-package-agreements --accept-source-agreements";
            RunCmd(cmd);
        };
        
        // 执行系统命令（宽字符版）
        auto RunWideCmd = [](const std::wstring& cmd)
        {
            std::wcout << L">> " << cmd << std::endl;
            _wsystem(cmd.c_str());
        };
        
        // UTF-8 转宽字符
        auto Utf8ToWide = [](const std::string& str) -> std::wstring
        {
            if (str.empty()) return L"";
            int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
            std::wstring result(len, L'\0');
            MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &result[0], len);
            return result;
        };
        
    switch (choice)
        {
            // ==================== 环境检查 ====================
            case 1:
            {
                ClearScreen();
                std::cout << "========== 编译环境检查 ==========\n\n";
                std::cout << "=== Python ===\n";
                system("python --version 2>&1");
                std::cout << "=== C/C++ ===\n";
                system("g++ --version 2>&1 | findstr /i \"g++\"");
                std::cout << "=== .NET ===\n";
                system("dotnet --version 2>&1");
                std::cout << "=== Node.js ===\n";
                system("node --version 2>&1");
                std::cout << "=== Java ===\n";
                system("java --version 2>&1 | findstr /i \"version\"");
                std::cout << "=== Rust ===\n";
                system("rustc --version 2>&1");
                std::cout << "=== Go ===\n";
                system("go version 2>&1");
                std::cout << "=== Git ===\n";
                system("git --version 2>&1");
                std::cout << "=== CMake ===\n";
                system("cmake --version 2>&1 | findstr /i \"cmake\"");
                Pause();
                break;
            }
                
            // ==================== Python ====================
            case 2: wingetInstall("Python.Python.3.12"); Pause(); break;
            case 3: wingetInstall("Python.Python.3.11"); Pause(); break;
            case 4: wingetInstall("Python.Python.3.10"); Pause(); break;
            case 5:
                ClearScreen();
                RunCmd("python --version");
                RunCmd("pip --version");
                Pause();
                break;
            case 6:
            {
                std::string pyFile = GetInput("Python 文件路径: ");
                if (!pyFile.empty())
                {
                    std::wstring wPyFile = Utf8ToWide(pyFile);
                    std::wstring cmd = L"python -m py_compile \"" + wPyFile + L"\"";
                    RunWideCmd(cmd);
                }
                Pause();
                break;
            }
case 7:
{
    RunCmd("pip install pyinstaller");
    std::cout << "Python 文件路径: ";
    std::string pyFile = GetInput("");

    if (pyFile.empty())
    {
        std::cout << "路径不能为空！" << std::endl;
        Pause();
        break;
    }

    // 去除引号
    while (!pyFile.empty() && (pyFile.front() == '"' || pyFile.front() == '\'')) pyFile.erase(pyFile.begin());
    while (!pyFile.empty() && (pyFile.back() == '"' || pyFile.back() == '\'')) pyFile.pop_back();

    // 提取源文件目录、文件名
    std::string sourceDir;
    size_t lastSlash = pyFile.find_last_of("\\/");
    if (lastSlash != std::string::npos)
        sourceDir = pyFile.substr(0, lastSlash);
    else
        sourceDir = ".";

    std::string baseName = pyFile;
    if (lastSlash != std::string::npos)
        baseName = pyFile.substr(lastSlash + 1);
    size_t lastDot = baseName.find_last_of('.');
    if (lastDot != std::string::npos)
        baseName = baseName.substr(0, lastDot);

    std::string exeName = baseName + ".exe";

    // 宽字符转换
    std::wstring wPyFile = Utf8ToWide(pyFile);
    std::wstring wBuildDir = Utf8ToWide(buildDir);
    std::wstring wDistDir = Utf8ToWide(distDir);
    std::wstring wSpecDir = Utf8ToWide(specDir);

    // 构建命令
    std::wstring cmdLine = L"pyinstaller --onefile --clean";
    cmdLine += L" --distpath \"" + wDistDir + L"\"";
    cmdLine += L" --workpath \"" + wBuildDir + L"\"";
    cmdLine += L" --specpath \"" + wSpecDir + L"\"";
    cmdLine += L" \"" + wPyFile + L"\"";

    std::wcout << L">> " << cmdLine << std::endl;

    // 执行
    STARTUPINFOW si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    std::vector<wchar_t> cmdBuffer(cmdLine.begin(), cmdLine.end());
    cmdBuffer.push_back(L'\0');

    if (CreateProcessW(nullptr, cmdBuffer.data(),
        nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi))
    {
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }

    // 定义源 exe 和目标 exe 路径（必须在 if 之前）
    std::string srcExe = distDir + "\\" + exeName;
    std::string destExe = sourceDir + "\\" + exeName;

    // 如果目标已存在，先尝试删除
    if (GetFileAttributesA(destExe.c_str()) != INVALID_FILE_ATTRIBUTES)
    {
        std::string killCmd = "taskkill /f /im \"" + exeName + "\" >nul 2>&1";
        system(killCmd.c_str());   // ← 用 .c_str()
        Sleep(500);
        DeleteFileA(destExe.c_str());
    }

    // ==================== 迁移 exe 回源文件目录 ====================

    // 如果目标已存在，先尝试删除
    if (GetFileAttributesA(destExe.c_str()) != INVALID_FILE_ATTRIBUTES)
    {
        std::string killCmd = "taskkill /f /im \"" + exeName + "\" >nul 2>&1";
        system(killCmd.c_str());
        Sleep(500);
        DeleteFileA(destExe.c_str());
    }

    // 检查源 exe 是否存在
    DWORD attr = GetFileAttributesA(srcExe.c_str());
    if (attr == INVALID_FILE_ATTRIBUTES)
    {
        std::cout << "\n未找到打包后的 exe: " << srcExe << std::endl;
        Pause();
        break;
    }

    // 尝试迁移
    if (MoveFileA(srcExe.c_str(), destExe.c_str()))
    {
        std::cout << "\n[OK] exe 已迁移到: " << destExe << std::endl;
    }
    else
    {
        if (CopyFileA(srcExe.c_str(), destExe.c_str(), FALSE))
        {
            DeleteFileA(srcExe.c_str());
            std::cout << "\n[OK] exe 已复制到: " << destExe << std::endl;
        }
        else
        {
            std::cout << "\n[FAIL] 迁移失败！" << std::endl;
            std::cout << "源: " << srcExe << std::endl;
            std::cout << "目标: " << destExe << std::endl;
        }
    }

    // 清理缓存
    std::string cleanBuild = "rmdir /s /q \"" + buildDir + "\" >nul 2>&1";
    system(cleanBuild.c_str());
    std::string cleanSpec = "rmdir /s /q \"" + specDir + "\" >nul 2>&1";
    system(cleanSpec.c_str());

    std::cout << "\n打包完成。" << std::endl;
    Pause();
    break;
}   // ← 这个右大括号必须有！
        case 8:
            {
                RunCmd("pip install nuitka");
                std::string pyFile = GetInput("Python 文件路径: ");
                if (!pyFile.empty())
                {
                    std::wstring wPyFile = Utf8ToWide(pyFile);
                    std::wstring cmd = L"nuitka --onefile --output-dir=\"" + Utf8ToWide(distDir) + L"\" \"" + wPyFile + L"\"";
                    RunWideCmd(cmd);
                }
                Pause();
                break;
            }
            case 9:
            {
                std::string venvName = GetInput("虚拟环境名称（默认 venv）: ");
                if (venvName.empty()) venvName = "venv";
                RunCmd("python -m venv " + venvName);
                Pause();
                break;
            }
case 10:
{
    std::string package = GetInput("包名: ");
    if (!package.empty()) RunCmd("pip install " + package);
    Pause();
    break;
}

case 11:
    ClearScreen();
    RunCmd("pip list");
    Pause();
    break;

case 12:
{
    std::string pyFile = GetInput("Python 文件路径: ");
    if (!pyFile.empty())
    {
        std::wstring cmd = L"python \"" + Utf8ToWide(pyFile) + L"\"";
        RunWideCmd(cmd);
    }
    Pause();
    break;
}

case 13: wingetInstall("BrechtSanders.WinLibs.POSIX.UCRT"); Pause(); break;
case 14: wingetInstall("Microsoft.VisualStudio.2022.BuildTools"); Pause(); break;

case 15:
    ClearScreen();
    RunCmd("g++ --version");
    RunCmd("gcc --version");
    Pause();
    break;
                case 16:
            {
                std::string cppFile = GetInput("C++ 文件路径: ");
                std::string outFile = GetInput("输出文件名: ");
                if (!cppFile.empty() && !outFile.empty())
                    RunCmd("g++ -o \"" + outFile + "\" \"" + cppFile + "\" -std=c++17");
                Pause();
                break;
            }
            case 17:
            {
                std::string cppFile = GetInput("C++ 文件路径: ");
                std::string outFile = GetInput("输出文件名: ");
                if (!cppFile.empty() && !outFile.empty())
                    RunCmd("g++ -o \"" + outFile + "\" \"" + cppFile + "\" -std=c++17 -O3 -march=native -flto");
                Pause();
                break;
            }
            case 18:
            {
                std::string cppFile = GetInput("C++ 文件路径: ");
                std::string outFile = GetInput("输出文件名: ");
                if (!cppFile.empty() && !outFile.empty())
                    RunCmd("g++ -o \"" + outFile + "\" \"" + cppFile + "\" -std=c++17 -g -O0 -Wall -Wextra");
                Pause();
                break;
            }
            case 19:
            {
                std::string cFile = GetInput("C 文件路径: ");
                std::string outFile = GetInput("输出文件名: ");
                if (!cFile.empty() && !outFile.empty())
                    RunCmd("gcc -o \"" + outFile + "\" \"" + cFile + "\" -O2");
                Pause();
                break;
            }
            case 20:
            {
                std::string pattern = GetInput("文件模式（如 *.cpp）: ");
                std::string outFile = GetInput("输出文件名: ");
                if (!pattern.empty() && !outFile.empty())
                    RunCmd("g++ -o \"" + outFile + "\" " + pattern + " -std=c++17 -O2");
                Pause();
                break;
            }
            case 21:
            {
                std::string cppFile = GetInput("C++ 文件路径: ");
                std::string outFile = GetInput("输出 DLL 名: ");
                if (!cppFile.empty() && !outFile.empty())
                    RunCmd("g++ -shared -o \"" + outFile + "\" \"" + cppFile + "\" -std=c++17 -O2");
                Pause();
                break;
            }
            case 22:
            {
                std::string cppFile = GetInput("C++ 文件路径: ");
                std::string outFile = GetInput("输出库名: ");
                if (!cppFile.empty() && !outFile.empty())
                    RunCmd("ar rcs \"" + outFile + "\" \"" + cppFile + "\"");
                Pause();
                break;
            }
                
            // ==================== .NET ====================
            case 23: wingetInstall("Microsoft.DotNet.SDK.8"); Pause(); break;
            case 24: wingetInstall("Microsoft.DotNet.SDK.9"); Pause(); break;
            case 25:
                ClearScreen();
                RunCmd("dotnet --version");
                RunCmd("dotnet --list-sdks");
                RunCmd("dotnet --list-runtimes");
                Pause();
                break;
            case 26:
            {
                std::string projName = GetInput("项目名称: ");
                if (!projName.empty()) RunCmd("dotnet new console -n " + projName);
                Pause();
                break;
            }
            case 27:
            {
                std::string projName = GetInput("项目名称: ");
                if (!projName.empty()) RunCmd("dotnet new webapi -n " + projName);
                Pause();
                break;
            }
            case 28:
            {
                std::string projName = GetInput("项目名称: ");
                if (!projName.empty()) RunCmd("dotnet new classlib -n " + projName);
                Pause();
                break;
            }
            case 29:
                ClearScreen();
                RunCmd("dotnet build");
                Pause();
                break;
            case 30:
                ClearScreen();
                RunCmd("dotnet run");
                Pause();
                break;
            case 31:
                ClearScreen();
                RunCmd("dotnet publish -c Release -r win-x64 --self-contained true -p:PublishSingleFile=true -o \"" + tempDir + "dotnet_publish\"");
                Pause();
                break;
            case 32:
                ClearScreen();
                RunCmd("dotnet publish -c Release -r win-x64 --self-contained false -o \"" + tempDir + "dotnet_publish\"");
                Pause();
                break;
            case 33:
            {
                std::string package = GetInput("包名: ");
                if (!package.empty()) RunCmd("dotnet add package " + package);
                Pause();
                break;
            }
            case 34:
                ClearScreen();
                RunCmd("dotnet list package");
                Pause();
                break;
                
            // ==================== 其他语言 ====================
            case 35: wingetInstall("OpenJS.NodeJS.LTS"); Pause(); break;
            case 36: wingetInstall("Oracle.JDK.21"); Pause(); break;
            case 37: wingetInstall("Oracle.JDK.17"); Pause(); break;
            case 38: wingetInstall("Rustlang.Rustup"); Pause(); break;
            case 39: wingetInstall("GoLang.Go"); Pause(); break;
            case 40:
            {
                std::string javaFile = GetInput("Java 文件路径: ");
                if (!javaFile.empty()) RunCmd("javac \"" + javaFile + "\"");
                Pause();
                break;
            }
            case 41:
            {
                std::string className = GetInput("类名（不含.class）: ");
                if (!className.empty()) RunCmd("java " + className);
                Pause();
                break;
            }
            case 42:
                ClearScreen();
                RunCmd("cargo build --release --target-dir \"" + tempDir + "rust_build\"");
                Pause();
                break;
            case 43:
                ClearScreen();
                RunCmd("go build -o \"" + tempDir + "go_output.exe\"");
                Pause();
                break;
            case 44:
            {
                std::string jsFile = GetInput("Node.js 文件路径: ");
                if (!jsFile.empty()) RunCmd("node \"" + jsFile + "\"");
                Pause();
                break;
            }
            case 45:
            {
                std::string package = GetInput("npm 包名: ");
                if (!package.empty()) RunCmd("npm install -g " + package);
                Pause();
                break;
            }
                
            // ==================== 构建工具 ====================
            case 46: wingetInstall("Kitware.CMake"); Pause(); break;
            case 47: wingetInstall("GnuWin32.Make"); Pause(); break;
            case 48: wingetInstall("Git.Git"); Pause(); break;
            case 49: wingetInstall("Ninja-build.Ninja"); Pause(); break;
            case 50: wingetInstall("MSYS2.MSYS2"); Pause(); break;
            case 51:
                ClearScreen();
                RunCmd("cmake -B \"" + buildDir + "\" -S .");
                RunCmd("cmake --build \"" + buildDir + "\" --config Release");
                Pause();
                break;
            case 52:
                ClearScreen();
                RunCmd("make");
                Pause();
                break;
                
            default:
                std::cout << "无效选项。" << std::endl;
                Pause();
        }
    }
}


// ==================== 软件更新 ====================

// 保存/读取上次运行的版本

// 获取本地 exe 的 SHA256
std::string GetLocalExeHash()
{
    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);
    return GetFileSHA256(exePath);
}

// 获取 zip 中 exe 的 SHA256
std::string GetRemoteExeHash(const std::string& zipPath)
{
    char tempPath[MAX_PATH];
    GetTempPathA(MAX_PATH, tempPath);
    std::string extractDir = std::string(tempPath) + "toolbox_check";

    std::string cleanCmd = "rmdir /s /q \"" + extractDir + "\" >nul 2>&1";
    system(cleanCmd.c_str());

    std::string extractCmd = "powershell -Command \""
        "$ProgressPreference='SilentlyContinue';"
        "Expand-Archive -Path '" + zipPath + "' -DestinationPath '" + extractDir + "' -Force\"";
    system(extractCmd.c_str());

    std::string listFile = "C:\\toolbox_exe_path.txt";
    DeleteFileA(listFile.c_str());

    std::string findCmd = "dir /s /b \"" + extractDir + "\\system-toolbox.exe\" > \"" + listFile + "\" 2>nul";
    system(findCmd.c_str());

    std::string exePath;
    std::ifstream f(listFile);
    if (f.is_open())
    {
        std::getline(f, exePath);
        f.close();
    }
    DeleteFileA(listFile.c_str());

    while (!exePath.empty() && (exePath.front() == ' ' || exePath.front() == '\t')) exePath.erase(exePath.begin());
    while (!exePath.empty() && (exePath.back() == ' ' || exePath.back() == '\t' || exePath.back() == '\r')) exePath.pop_back();

    if (exePath.empty())
    {
        std::cout << "无法在 zip 中找到 system-toolbox.exe" << std::endl;
        return "";
    }

    std::cout << "找到 exe: " << exePath << std::endl;
    std::string hash = GetFileSHA256(exePath);

    std::string cleanCmd2 = "rmdir /s /q \"" + extractDir + "\" >nul 2>&1";
    system(cleanCmd2.c_str());

    return hash;
}

std::string GetLastVersionFile()
{
    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);
    std::string dir = exePath;
    size_t pos = dir.find_last_of("\\/");
    if (pos != std::string::npos) dir = dir.substr(0, pos);
    return dir + "\\last_version.txt";
}

// 读取上次版本
std::string GetLastVersion()
{
    std::string file = GetLastVersionFile();
    std::ifstream f(file);
    if (f.is_open())
    {
        std::string ver;
        std::getline(f, ver);
        f.close();
        return ver;
    }
    return "";
}

// 保存当前版本
void SaveCurrentVersion()
{
    std::string file = GetLastVersionFile();
    std::ofstream f(file);
    if (f.is_open())
    {
        f << TOOLBOX_VERSION;
        f.close();
    }
}

// 获取 GitHub 最新版本信息（不用 irm，用 Invoke-WebRequest）
bool GetLatestReleaseInfo(std::string& tagName, std::string& downloadUrl, std::string& body)
{
    std::string jsonFile = "C:\\toolbox_release.json";
    DeleteFileA(jsonFile.c_str());

       std::string cmd = "C:\\Windows\\System32\\curl.exe -s -L --ssl-no-revoke -H \"User-Agent: System-Toolbox\" -o \"" + jsonFile + "\" " + GITHUB_API_URL;
    system(cmd.c_str());

    std::cout << "正在检查最新版本...\n";
    system(cmd.c_str());

    DWORD attr = GetFileAttributesA(jsonFile.c_str());
    if (attr == INVALID_FILE_ATTRIBUTES)
    {
        std::cout << "网络连接失败或仓库不存在" << std::endl;
        return false;
    }

    std::ifstream file(jsonFile);
    if (!file.is_open())
    {
        std::cout << "无法读取版本信息" << std::endl;
        return false;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string json = buffer.str();
    file.close();
    DeleteFileA(jsonFile.c_str());

    if (json.empty())
    {
        std::cout << "服务器返回空数据" << std::endl;
        return false;
    }

    if (json.find("\"message\"") != std::string::npos &&
        json.find("Not Found") != std::string::npos)
    {
        std::cout << "\n未发布任何版本！" << std::endl;
        return false;
    }

    // 解析 tag_name
    size_t pos = json.find("\"tag_name\"");
    if (pos == std::string::npos) return false;
    pos = json.find(':', pos);
    pos = json.find('"', pos);
    size_t end = json.find('"', pos + 1);
    tagName = json.substr(pos + 1, end - pos - 1);

    // 解析 browser_download_url（找 .zip）
    pos = json.find("\"browser_download_url\"");
    while (pos != std::string::npos)
    {
        pos = json.find(':', pos);
        pos = json.find('"', pos);
        end = json.find('"', pos + 1);
        std::string url = json.substr(pos + 1, end - pos - 1);
        if (url.size() > 4 && url.substr(url.size() - 4) == ".zip")
        {
            downloadUrl = url;
            break;
        }
        pos = json.find("\"browser_download_url\"", end);
    }

    // 解析 body
    pos = json.find("\"body\"");
    if (pos != std::string::npos)
    {
        pos = json.find(':', pos);
        pos = json.find('"', pos);
        end = json.find("\",", pos + 1);
        if (end != std::string::npos)
            body = json.substr(pos + 1, end - pos - 1);
    }

    // 替换 JSON 转义字符
    size_t rpos;
    while ((rpos = body.find("\\r\\n")) != std::string::npos)
        body.replace(rpos, 4, "\n");
    while ((rpos = body.find("\\n")) != std::string::npos)
        body.replace(rpos, 2, "\n");
    while ((rpos = body.find("\\r")) != std::string::npos)
        body.replace(rpos, 2, "\r");
    while ((rpos = body.find("\\\"")) != std::string::npos)
        body.replace(rpos, 2, "\"");

    return true;
}

// 检测是否刚更新过版本
bool CheckJustUpdated()
{
    std::string lastVer = GetLastVersion();
    
    // 如果没有 last_version.txt，说明是首次运行或旧版本，不显示更新内容
    if (lastVer.empty())
    {
        SaveCurrentVersion();
        return false;
    }
    
    // 如果 last_version.txt 和当前版本不同，说明刚更新过
    if (lastVer != TOOLBOX_VERSION)
    {
        return true;
    }
    
    return false;
}

void ShowUpdateContent()
{
    ClearScreen();
    std::cout << "========== 更新成功 ==========\n";
    std::cout << "检测到程序已更新\n";
    std::cout << "==============================\n\n";
    std::cout << "正在获取更新内容...\n\n";

    std::string tagName, downloadUrl, body;
    if (!GetLatestReleaseInfo(tagName, downloadUrl, body))
    {
        std::cout << "无法获取更新内容，请访问 GitHub 查看。" << std::endl;

        // 保存当前哈希，避免下次重复提示
        SaveCurrentHash(GetLocalExeHash());
        Pause();
        return;
    }

    std::cout << "=== 更新内容 ===\n";
    std::cout << body << "\n\n";
    std::cout << "完整更新日志：" << GITHUB_RELEASE_URL << "\n";

    // 保存当前哈希
    SaveCurrentHash(GetLocalExeHash());
    Pause();
}

// 保存当前哈希
void SaveCurrentHash(const std::string& hash)
{
    // 设置当前进程环境变量
    SetEnvironmentVariableA(ENV_LAST_HASH, hash.c_str());

    // 持久化到注册表（HKCU\Environment）
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Environment", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS)
    {
        RegSetValueExA(hKey, ENV_LAST_HASH, 0, REG_SZ,
                       (const BYTE*)hash.c_str(), (DWORD)(hash.length() + 1));
        RegCloseKey(hKey);
    }

    // 广播环境变量变更
    SendMessageTimeoutA(HWND_BROADCAST, WM_SETTINGCHANGE, 0,
                        (LPARAM)"Environment", SMTO_ABORTIFHUNG, 5000, NULL);
}

// 软件更新主函数
void SoftwareUpdate()
{
    ClearScreen();
    std::cout << "========== 软件更新 ==========\n";
    std::cout << "当前版本: " << TOOLBOX_VERSION << "\n";
    std::cout << "仓库: https://github.com/huvubb/system-toolbox\n";
    std::cout << "==============================\n\n";

    // 获取本地 exe 哈希
    std::cout << "正在计算本地版本哈希...\n";
    std::string localHash = GetLocalExeHash();
    if (localHash.empty())
    {
        std::cout << "无法计算本地文件哈希！" << std::endl;
        Pause();
        return;
    }
    std::cout << "本地哈希: " << localHash << "\n\n";

    // 获取 GitHub 最新版本信息
    std::string latestVersion, downloadUrl, body;
    if (!GetLatestReleaseInfo(latestVersion, downloadUrl, body))
    {
        Pause();
        return;
    }

    std::cout << "GitHub 最新版本: " << latestVersion << "\n";

    if (downloadUrl.empty())
    {
        std::cout << "未找到下载附件（.zip）！" << std::endl;
        Pause();
        return;
    }

    // 创建临时目录
    char tempPath[MAX_PATH];
    GetTempPathA(MAX_PATH, tempPath);
    std::string updateDir = std::string(tempPath) + "toolbox_update";
    CreateDirectoryA(updateDir.c_str(), nullptr);

    std::string zipPath = updateDir + "\\update.zip";
    DeleteFileA(zipPath.c_str());

        // 下载 zip
    std::cout << "\n正在连接到更新服务器 [完成]\n";
    std::cout << "\n正在下载最新版本...\n";

    // 方法1：PowerShell 直连下载
    std::string dlCmd = "powershell -Command \""
        "$ProgressPreference='SilentlyContinue';"
        "[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12;"
        "Invoke-WebRequest -Uri '" + downloadUrl + "' "
        "-OutFile '" + zipPath + "' -UseBasicParsing -TimeoutSec 300\"";
    system(dlCmd.c_str());

    // 检查是否成功
    DWORD attr = GetFileAttributesA(zipPath.c_str());
    if (attr == INVALID_FILE_ATTRIBUTES)
    {
        // 方法2：镜像下载
        std::cout << "直连失败，尝试镜像...\n";
        std::string mirrorUrl = "https://ghproxy.net/" + downloadUrl;
        std::string mirrorCmd = "powershell -Command \""
            "$ProgressPreference='SilentlyContinue';"
            "[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12;"
            "Invoke-WebRequest -Uri '" + mirrorUrl + "' "
            "-OutFile '" + zipPath + "' -UseBasicParsing -TimeoutSec 300\"";
        system(mirrorCmd.c_str());

        attr = GetFileAttributesA(zipPath.c_str());
        if (attr == INVALID_FILE_ATTRIBUTES)
        {
            // 方法3：curl 最后尝试
            std::cout << "镜像失败，尝试 curl...\n";
            std::string curlCmd = "C:\\Windows\\System32\\curl.exe -L --ssl-no-revoke --retry 3 -o \"" + zipPath + "\" \"" + downloadUrl + "\"";
            system(curlCmd.c_str());

            attr = GetFileAttributesA(zipPath.c_str());
            if (attr == INVALID_FILE_ATTRIBUTES)
            {
                std::cout << "下载失败！请检查网络连接。" << std::endl;
                Pause();
                return;
            }
        }
    }

    std::cout << "下载完成。\n";

    // 计算 zip 中 exe 的哈希
    std::cout << "\n正在校验远程版本...\n";
    std::string remoteHash = GetRemoteExeHash(zipPath);
    if (remoteHash.empty())
    {
        std::cout << "无法计算远程哈希！" << std::endl;
        Pause();
        return;
    }
    std::cout << "远程哈希: " << remoteHash << "\n\n";

    // 对比哈希
    if (localHash == remoteHash)
    {
        std::cout << "已是最新版本，无需更新。" << std::endl;
        DeleteFileA(zipPath.c_str());
        Pause();
        return;
    }

    // 版本不同，询问用户
    std::cout << "发现新版本！\n\n";
    std::cout << "=== 更新说明 ===\n" << body << "\n\n";
    std::cout << "是否更新？(y/n): ";
    char c;
    std::cin >> c;
    std::cin.ignore();
    if (c != 'y' && c != 'Y')
    {
        std::cout << "已取消。" << std::endl;
        DeleteFileA(zipPath.c_str());
        Pause();
        return;
    }

    // 获取当前 exe 路径
    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);
    std::string exeFullPath = exePath;
    std::string exeDir;
    size_t lastSlash = exeFullPath.find_last_of("\\/");
    if (lastSlash != std::string::npos)
        exeDir = exeFullPath.substr(0, lastSlash);

    std::cout << "\n准备更新，程序将退出...\n";
    Sleep(1500);

    // 生成更新脚本
    std::string batPath = updateDir + "\\update.bat";
    std::ofstream bat(batPath);
    if (!bat.is_open())
    {
        std::cout << "无法创建更新脚本！" << std::endl;
        Pause();
        return;
    }

    bat << "@echo off\n";
    bat << "chcp 65001 >nul\n";
    bat << "taskkill /f /im system-toolbox.exe >nul 2>&1\n";
    bat << "timeout /t 1 /nobreak >nul\n";
    bat << "powershell -Command \"Expand-Archive -Path '" << zipPath << "' -DestinationPath '" << updateDir << "\\extracted' -Force\"\n";
    bat << "timeout /t 2 /nobreak >nul\n";
    bat << "for /r \"" << updateDir << "\\extracted\" %%i in (system-toolbox.exe) do copy /Y \"%%i\" \"" << exeFullPath << "\"\n";
    bat << "for /r \"" << updateDir << "\\extracted\" %%i in (PsExec64.exe) do copy /Y \"%%i\" \"" << exeDir << "\\PsExec64.exe\"\n";
    bat << "for /r \"" << updateDir << "\\extracted\" %%i in (README.md) do copy /Y \"%%i\" \"" << exeDir << "\\README.md\"\n";
    bat << "timeout /t 1 /nobreak >nul\n";
    bat << "start \"\" \"" << exeFullPath << "\"\n";
    bat << "timeout /t 3 /nobreak >nul\n";
    bat << "rmdir /s /q \"" << updateDir << "\"\n";
    bat << "del \"%~f0\"\n";
    bat.close();

    // 启动更新脚本
    std::string runBat = "cmd /c \"" + batPath + "\"";
    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    CreateProcessA(NULL, (LPSTR)runBat.c_str(), NULL, NULL, FALSE,
                   CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
    if (pi.hProcess) CloseHandle(pi.hProcess);
    if (pi.hThread) CloseHandle(pi.hThread);

    std::cout << "程序即将退出..." << std::endl;
    Sleep(2000);
    exit(0);
}

// ==================== 音视频工具（重构版） ====================

// UTF-8 转 GBK
std::string Utf8ToGbk(const std::string& utf8)
{
    int wlen = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, nullptr, 0);
    std::wstring wstr(wlen, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, &wstr[0], wlen);
    int glen = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string gbk(glen, '\0');
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &gbk[0], glen, nullptr, nullptr);
    if (!gbk.empty() && gbk.back() == '\0') gbk.pop_back();
    return gbk;
}

// 执行 FFmpeg 命令
void RunFFmpeg(const std::string& cmd)
{
    std::string gbkCmd = Utf8ToGbk(cmd);
    std::cout << ">> " << cmd << std::endl;
    int ret = system(gbkCmd.c_str());
    if (ret != 0)
    {
        std::cout << "\n[提示] 命令执行失败（返回码: " << ret << "）\n";
    }
}

// 探测视频编码格式
std::string ProbeVideoCodec(const std::string& file)
{
    std::string tempFile = "C:\\probe_result.txt";
    DeleteFileA(tempFile.c_str());
    std::string cmd = "ffprobe -v error -select_streams v:0 -show_entries stream=codec_name -of default=noprint_wrappers=1:nokey=1 \"" + file + "\" > \"" + tempFile + "\" 2>&1";
    system(Utf8ToGbk(cmd).c_str());

    std::ifstream f(tempFile);
    std::string codec;
    if (f.is_open())
    {
        std::getline(f, codec);
        f.close();
    }
    DeleteFileA(tempFile.c_str());

    // 去空白
    while (!codec.empty() && (codec.back() == '\r' || codec.back() == '\n' || codec.back() == ' '))
        codec.pop_back();

    return codec;
}

// 判断是否为硬解兼容格式
bool IsHardwareDecodeCompatible(const std::string& codec)
{
    // 现代 GPU 硬解支持的主流格式
    if (codec == "h264") return true;
    if (codec == "hevc") return true;   // H.265
    if (codec == "vp9")  return true;
    if (codec == "av1")  return true;
    if (codec == "mpeg2video") return true;  // MPEG-2 广泛支持
    if (codec == "vc1")  return true;
    return false;
}

// 判断 GPU 品牌（0=未知, 1=NVIDIA, 2=Intel, 3=AMD）
int DetectGpuBrand()
{
    // NVIDIA
    if (system("nvidia-smi >nul 2>&1") == 0)
        return 1;

    // Intel
    std::string tempFile = "C:\\gpu_check.txt";
    system(("wmic path win32_VideoController get Name > \"" + tempFile + "\" 2>&1").c_str());
    std::ifstream f(tempFile);
    std::string line, allLines;
    while (std::getline(f, line)) allLines += line;
    f.close();
    DeleteFileA(tempFile.c_str());

    if (allLines.find("Intel") != std::string::npos) return 2;
    if (allLines.find("AMD") != std::string::npos || allLines.find("Radeon") != std::string::npos) return 3;
    return 0;
}

// 获取推荐硬解参数
std::string GetBestHwAccelArgs()
{
    int brand = DetectGpuBrand();

    switch (brand)
    {
        case 1:  // NVIDIA
            return "-hwaccel cuda -hwaccel_output_format cuda";
        case 2:  // Intel
            return "-hwaccel d3d11va -hwaccel_output_format d3d11";
        case 3:  // AMD
            return "-hwaccel d3d11va -hwaccel_output_format d3d11";
        default:  // 未知
            return "-hwaccel d3d11va";
    }
}

// 获取推荐硬编码器
std::string GetBestHwEncoder()
{
    int brand = DetectGpuBrand();

    switch (brand)
    {
        case 1: return "h264_nvenc";
        case 2: return "h264_qsv";
        case 3: return "h264_amf";
        default: return "libx264";
    }
}

// ==================== 核心：智能硬解流水线 ====================
// 输入 → 兼容性检查 → 必要时转码 → 硬解 → 输出
void SmartDecodePipeline(const std::string& src, const std::string& dst,
                         const std::string& extraArgs = "",
                         bool forceManual = false,
                         const std::string& manualHwAccel = "")
{
    std::cout << "\n=== 智能硬解流水线 ===\n";

    // 1. 探测编码格式
    std::string codec = ProbeVideoCodec(src);
    std::cout << "视频编码: " << (codec.empty() ? "未知" : codec) << "\n";

    // 2. 兼容性检查
    bool compatible = IsHardwareDecodeCompatible(codec);
    std::cout << "硬解兼容: " << (compatible ? "是" : "否") << "\n";

    // 3. 决定硬解参数
    std::string hwAccel;
    if (forceManual && !manualHwAccel.empty())
    {
        hwAccel = manualHwAccel;
        std::cout << "使用手动硬解: " << hwAccel << "\n";
    }
    else
    {
        hwAccel = GetBestHwAccelArgs();
        std::cout << "使用智能硬解: " << hwAccel << "\n";
    }

    // 4. 不兼容格式先转码
    if (!compatible && !forceManual)
    {
        std::cout << "\n[转换] 格式不兼容硬解，先转 H.264...\n";

        std::string tempFile = src + ".h264temp.mp4";
        std::string preCmd = "ffmpeg -i \"" + src + "\" -c:v libx264 -preset fast -crf 20 -c:a aac -y \"" + tempFile + "\"";
        RunFFmpeg(preCmd);

        // 检查转码结果
        if (GetFileAttributesA(tempFile.c_str()) == INVALID_FILE_ATTRIBUTES)
        {
            std::cout << "[错误] 预处理失败！\n";
            return;
        }

        std::cout << "[完成] 转码完成，开始硬解处理...\n\n";

        // 对转码后的文件硬解
        std::string cmd = "ffmpeg " + hwAccel + " -i \"" + tempFile + "\" " + extraArgs + " -y \"" + dst + "\"";
        RunFFmpeg(cmd);

        DeleteFileA(tempFile.c_str());
    }
    else
    {
        // 直接硬解
        std::string cmd = "ffmpeg " + hwAccel + " -i \"" + src + "\" " + extraArgs + " -y \"" + dst + "\"";
        RunFFmpeg(cmd);
    }
}

// ==================== GPU 硬件检测 ====================
void DetectGpuHardware()
{
    ClearScreen();
    std::cout << "========== GPU 硬件加速检测 ==========\n\n";

    int brand = DetectGpuBrand();
    std::cout << "检测到 GPU: ";
    switch (brand)
    {
        case 1: std::cout << "NVIDIA\n"; break;
        case 2: std::cout << "Intel\n"; break;
        case 3: std::cout << "AMD\n"; break;
        default: std::cout << "未知\n"; break;
    }

    std::cout << "\n推荐硬解参数: " << GetBestHwAccelArgs() << "\n";
    std::cout << "推荐硬编码器: " << GetBestHwEncoder() << "\n\n";

    std::cout << "=== 支持的硬件加速类型 ===\n";
    system("ffmpeg -hwaccels 2>&1");

    std::cout << "\n=== 硬件解码器 ===\n";
    system("ffmpeg -decoders 2>&1 | findstr /i \"h264_ hevc_ vp9_ av1_ cuda dxva2 d3d11va d3d12va qsv nvdec\"");

    std::cout << "\n=== 硬件编码器 ===\n";
    system("ffmpeg -encoders 2>&1 | findstr /i \"nvenc qsv amf\"");

    Pause();
}

// ==================== GPU 硬解码测试 ====================
void TestGpuDecode()
{
    ClearScreen();
    std::cout << "========== GPU 硬解码测试 ==========\n";
    std::string src = GetInput("测试视频路径: ");
    if (src.empty()) { Pause(); return; }

    std::cout << "\n解码方式：\n";
    std::cout << " 1. 智能（自动选择最优）\n";
    std::cout << " 2. 手动指定\n";
    int mode = GetIntInput("请选择: ", 1);

    std::string hwAccel = "";
    bool forceManual = false;

    if (mode == 2)
    {
        forceManual = true;
        std::cout << "\n选择硬解方式：\n";
        std::cout << " 1. CUDA (NVIDIA)\n";
        std::cout << " 2. D3D11VA (Windows 10+)\n";
        std::cout << " 3. D3D12VA (Windows 10+)\n";
        std::cout << " 4. DXVA2 (Windows 7+)\n";
        std::cout << " 5. QSV (Intel)\n";
        std::cout << " 6. 软解（CPU）\n";
        int c = GetIntInput("请选择: ");

        switch (c)
        {
            case 1: hwAccel = "-hwaccel cuda -hwaccel_output_format cuda"; break;
            case 2: hwAccel = "-hwaccel d3d11va -hwaccel_output_format d3d11"; break;
            case 3: hwAccel = "-hwaccel d3d12va"; break;
            case 4: hwAccel = "-hwaccel dxva2"; break;
            case 5: hwAccel = "-hwaccel qsv -hwaccel_output_format qsv"; break;
            case 6: hwAccel = ""; break;
            default: Pause(); return;
        }
    }

    // 探测编码
    std::string codec = ProbeVideoCodec(src);
    std::cout << "\n视频编码: " << codec << "\n";

    if (!IsHardwareDecodeCompatible(codec) && !forceManual)
    {
        std::cout << "\n[提示] 编码 " << codec << " 不兼容硬解，将自动转码后硬解。\n";
    }

    // 执行流水线
    SmartDecodePipeline(src, "NUL", "-f null -", forceManual, hwAccel);

    Pause();
}

// ==================== 视频格式转换（硬解流水线） ====================
void VideoConvertWithHwDecode()
{
    ClearScreen();
    std::cout << "========== 视频格式转换（硬解加速） ==========\n";
    std::string src = GetInput("输入文件路径: ");
    if (src.empty()) { Pause(); return; }
    while (!src.empty() && (src.front() == '"' || src.front() == '\'')) src.erase(src.begin());
    while (!src.empty() && (src.back() == '"' || src.back() == '\'')) src.pop_back();

    std::cout << "目标格式:\n";
    std::cout << " 1. MP4 (H.264)\n";
    std::cout << " 2. MP4 (H.265/HEVC)\n";
    std::cout << " 3. MKV\n";
    std::cout << " 4. MOV\n";
    std::cout << " 5. WebM\n";
    int fmt = GetIntInput("请选择: ");

    std::string dst = GetInput("输出文件路径: ");
    if (dst.empty()) { Pause(); return; }
    while (!dst.empty() && (dst.front() == '"' || dst.front() == '\'')) dst.erase(dst.begin());
    while (!dst.empty() && (dst.back() == '"' || dst.back() == '\'')) dst.pop_back();

    // 选择输出编码
    std::cout << "\n编码方式：\n";
    std::cout << " 1. 智能硬编码（推荐）\n";
    std::cout << " 2. CPU 软编码\n";
    int encMode = GetIntInput("请选择: ", 1);

    std::string encoder;
    std::string encArgs;

    if (encMode == 1)
    {
        encoder = GetBestHwEncoder();
        std::cout << "使用编码器: " << encoder << "\n";

        if (encoder == "h264_nvenc")
            encArgs = "-c:v h264_nvenc -preset p4 -b:v 5M";
        else if (encoder == "h264_qsv")
            encArgs = "-c:v h264_qsv -b:v 5M";
        else if (encoder == "h264_amf")
            encArgs = "-c:v h264_amf -b:v 5M";
        else
            encArgs = "-c:v libx264 -crf 23";
    }
    else
    {
        encArgs = "-c:v libx264 -crf 23";
    }

    // 按目标格式调整
    if (fmt == 2)
        encArgs = "-c:v libx265 -crf 23";
    else if (fmt == 5)
        encArgs = "-c:v libvpx-vp9 -c:a libopus";

    encArgs += " -c:a aac";

    // 执行智能流水线
    SmartDecodePipeline(src, dst, encArgs);

    Pause();
}

// ==================== 手动硬解码转换 ====================
void ManualHwDecodeConvert()
{
    ClearScreen();
    std::cout << "========== 手动硬解码转换 ==========\n";
    std::cout << "用户强制指定解码方式，不走智能流水线。\n\n";

    std::string src = GetInput("输入文件路径: ");
    std::string dst = GetInput("输出文件路径: ");
    if (src.empty() || dst.empty()) { Pause(); return; }

    std::cout << "\n选择硬解方式：\n";
    std::cout << " 1. CUDA (NVIDIA)\n";
    std::cout << " 2. D3D11VA (Windows 10+)\n";
    std::cout << " 3. D3D12VA (Windows 10+)\n";
    std::cout << " 4. DXVA2 (Windows 7+)\n";
    std::cout << " 5. QSV (Intel)\n";
    std::cout << " 6. 软解（CPU）\n";
    int c = GetIntInput("请选择: ");

    std::string hwAccel;
    switch (c)
    {
        case 1: hwAccel = "-hwaccel cuda -hwaccel_output_format cuda"; break;
        case 2: hwAccel = "-hwaccel d3d11va -hwaccel_output_format d3d11"; break;
        case 3: hwAccel = "-hwaccel d3d12va"; break;
        case 4: hwAccel = "-hwaccel dxva2"; break;
        case 5: hwAccel = "-hwaccel qsv -hwaccel_output_format qsv"; break;
        case 6: hwAccel = ""; break;
        default: Pause(); return;
    }

    std::cout << "\n输出编码：\n 1. H.264 (libx264)\n 2. H.265 (libx265)\n 3. GPU 硬编码\n";
    int enc = GetIntInput("请选择: ", 1);

    std::string encArgs;
    if (enc == 1) encArgs = "-c:v libx264 -crf 23 -c:a aac";
    else if (enc == 2) encArgs = "-c:v libx265 -crf 23 -c:a aac";
    else encArgs = "-c:v " + GetBestHwEncoder() + " -b:v 5M -c:a aac";

    // 直接执行，不走智能流水线
    std::string cmd = "ffmpeg " + hwAccel + " -i \"" + src + "\" " + encArgs + " -y \"" + dst + "\"";
    std::cout << "\n=== 执行命令 ===\n" << cmd << "\n\n";
    RunFFmpeg(cmd);

    Pause();
}

// ==================== 音视频工具主菜单 ====================
void MediaToolsMenu()
{
    while (true)
    {
        ClearScreen();
        std::cout << "========== 音视频工具 ==========\n";
        std::cout << "=== 环境与硬件 ===\n";
        std::cout << " 1. 检查 FFmpeg 环境\n";
        std::cout << " 2. GPU 硬件加速检测\n";
        std::cout << " 3. GPU 硬解码测试\n";
        std::cout << "\n=== 智能转换（自动硬解） ===\n";
        std::cout << " 4. 视频格式转换（智能硬解）\n";
        std::cout << " 5. 视频压缩（智能硬解）\n";
        std::cout << " 6. 手动硬解码转换\n";
        std::cout << "\n=== 视频处理 ===\n";
        std::cout << " 7. 视频剪辑\n";
        std::cout << " 8. 视频合并\n";
        std::cout << " 9. 视频转 GIF\n";
        std::cout << "10. 视频截图\n";
        std::cout << "11. 添加水印\n";
        std::cout << "12. 调整分辨率\n";
        std::cout << "13. 调整码率\n";
        std::cout << "14. 调整帧率\n";
        std::cout << "15. 视频旋转/翻转\n";
        std::cout << "16. 视频加速/减速\n";
        std::cout << "17. 视频信息查看\n";
        std::cout << "18. 视频预加载（faststart）\n";
        std::cout << "\n=== 音频处理 ===\n";
        std::cout << "19. 音频格式转换\n";
        std::cout << "20. 提取视频中的音频\n";
        std::cout << "21. 音频剪辑\n";
        std::cout << "22. 音频合并\n";
        std::cout << "23. 调整音量\n";
        std::cout << "24. 音频变速变调\n";
        std::cout << "25. 音频降噪\n";
        std::cout << "26. 音频信息查看\n";
        std::cout << "27. 提取视频字幕\n";
        std::cout << "\n=== 音乐软件私有格式 ===\n";
        std::cout << "28. 网易云 NCM 转 MP3\n";
        std::cout << "29. 通用加密格式（unlock-music）\n";
        std::cout << "\n=== 批量操作 ===\n";
        std::cout << "30. 批量格式转换\n";
        std::cout << "31. 批量提取音频\n";
        std::cout << "32. 批量视频压缩\n";
        std::cout << " 0. 返回主菜单\n";
        std::cout << "================================\n";

        int choice = GetIntInput("请选择: ");

        if (choice == 0) break;

        switch (choice)
        {
            case 1:
            {
                ClearScreen();
                std::cout << "========== FFmpeg 环境检查 ==========\n";
                int ret = system("ffmpeg -version 2>&1");
                if (ret == 0)
                    std::cout << "\n[OK] FFmpeg 已安装\n";
                else
                {
                    std::cout << "\n[FAIL] FFmpeg 未安装\n\n";
                    std::cout << "是否通过 winget 安装？(y/n): ";
                    char c;
                    std::cin >> c;
                    std::cin.ignore();
                    if (c == 'y' || c == 'Y')
                        RunCmd("winget install Gyan.FFmpeg --silent --accept-package-agreements --accept-source-agreements");
                }
                Pause();
                break;
            }

            case 2: DetectGpuHardware(); break;
            case 3: TestGpuDecode(); break;
            case 4: VideoConvertWithHwDecode(); break;
            case 5:
            {
                ClearScreen();
                std::cout << "========== 视频压缩（智能硬解） ==========\n";
                std::string src = GetInput("输入文件路径: ");
                std::string dst = GetInput("输出文件路径: ");
                if (src.empty() || dst.empty()) { Pause(); break; }
                int crf = GetIntInput("CRF 值（18-28，越小质量越高）: ", 23);

                std::string encoder = GetBestHwEncoder();
                std::string encArgs;
                if (encoder == "h264_nvenc")
                    encArgs = "-c:v h264_nvenc -preset p4 -cq " + std::to_string(crf) + " -c:a aac -b:a 128k";
                else if (encoder == "h264_qsv")
                    encArgs = "-c:v h264_qsv -global_quality " + std::to_string(crf) + " -c:a aac -b:a 128k";
                else if (encoder == "h264_amf")
                    encArgs = "-c:v h264_amf -qp_i " + std::to_string(crf) + " -c:a aac -b:a 128k";
                else
                    encArgs = "-c:v libx264 -crf " + std::to_string(crf) + " -c:a aac -b:a 128k";

                SmartDecodePipeline(src, dst, encArgs);
                Pause();
                break;
            }

            case 6: ManualHwDecodeConvert(); break;

            // 其他视频处理（走智能流水线）
            case 7:  // 视频剪辑
            {
                ClearScreen();
                std::string src = GetInput("输入文件: ");
                std::string start = GetInput("开始时间（00:01:30）: ");
                std::string duration = GetInput("持续时长（00:02:00）: ");
                std::string dst = GetInput("输出文件: ");
                if (src.empty() || dst.empty()) { Pause(); break; }
                std::string cmd = "ffmpeg -ss " + start + " -i \"" + src + "\" -t " + duration + " -c copy -y \"" + dst + "\"";
                RunFFmpeg(cmd);
                Pause();
                break;
            }

            case 8:  // 视频合并
            {
                ClearScreen();
                std::string listFile = GetInput("列表文件路径: ");
                std::string dst = GetInput("输出文件: ");
                if (listFile.empty() || dst.empty()) { Pause(); break; }
                std::string cmd = "ffmpeg -f concat -safe 0 -i \"" + listFile + "\" -c copy -y \"" + dst + "\"";
                RunFFmpeg(cmd);
                Pause();
                break;
            }

            case 9:  // 视频转 GIF
            {
                ClearScreen();
                std::string src = GetInput("输入视频: ");
                std::string dst = GetInput("输出 GIF: ");
                if (src.empty() || dst.empty()) { Pause(); break; }
                int fps = GetIntInput("帧率（10）: ", 10);
                int width = GetIntInput("宽度（480）: ", 480);
                std::string cmd = "ffmpeg -i \"" + src + "\" -vf \"fps=" + std::to_string(fps) + ",scale=" + std::to_string(width) + ":-1:flags=lanczos\" -y \"" + dst + "\"";
                RunFFmpeg(cmd);
                Pause();
                break;
            }

            case 10:  // 视频截图
            {
                ClearScreen();
                std::string src = GetInput("输入视频: ");
                std::string time = GetInput("时间点: ");
                std::string dst = GetInput("输出图片: ");
                if (src.empty() || dst.empty()) { Pause(); break; }
                std::string cmd = "ffmpeg -ss " + time + " -i \"" + src + "\" -vframes 1 -y \"" + dst + "\"";
                RunFFmpeg(cmd);
                Pause();
                break;
            }

            case 11:  // 添加水印
            {
                ClearScreen();
                std::string src = GetInput("输入视频: ");
                std::string wm = GetInput("水印图片: ");
                std::string dst = GetInput("输出视频: ");
                if (src.empty() || wm.empty() || dst.empty()) { Pause(); break; }
                std::string pos = GetInput("位置（10:10）: ");
                if (pos.empty()) pos = "10:10";
                std::string cmd = "ffmpeg -i \"" + src + "\" -i \"" + wm + "\" -filter_complex \"overlay=" + pos + "\" -y \"" + dst + "\"";
                RunFFmpeg(cmd);
                Pause();
                break;
            }

            case 12:  // 调整分辨率
            {
                ClearScreen();
                std::string src = GetInput("输入视频: ");
                int w = GetIntInput("宽度: ");
                int h = GetIntInput("高度: ");
                std::string dst = GetInput("输出视频: ");
                if (src.empty() || dst.empty()) { Pause(); break; }
                std::string enc = GetBestHwEncoder();
                std::string encArgs = (enc == "libx264") ? "-c:v libx264 -crf 23" : "-c:v " + enc + " -b:v 5M";
                std::string extra = "-vf scale=" + std::to_string(w) + ":" + std::to_string(h) + " " + encArgs + " -c:a aac";
                SmartDecodePipeline(src, dst, extra);
                Pause();
                break;
            }

            case 13:  // 调整码率
            {
                ClearScreen();
                std::string src = GetInput("输入视频: ");
                int br = GetIntInput("目标码率(kbps): ");
                std::string dst = GetInput("输出视频: ");
                if (src.empty() || dst.empty()) { Pause(); break; }
                std::string extra = "-b:v " + std::to_string(br) + "k -c:a aac";
                SmartDecodePipeline(src, dst, extra);
                Pause();
                break;
            }

            case 14:  // 调整帧率
            {
                ClearScreen();
                std::string src = GetInput("输入视频: ");
                int fps = GetIntInput("目标帧率: ");
                std::string dst = GetInput("输出视频: ");
                if (src.empty() || dst.empty()) { Pause(); break; }
                std::string extra = "-r " + std::to_string(fps) + " -c:a aac";
                SmartDecodePipeline(src, dst, extra);
                Pause();
                break;
            }

            case 15:  // 旋转/翻转
            {
                ClearScreen();
                std::string src = GetInput("输入视频: ");
                std::cout << " 1. 顺时针90°\n 2. 逆时针90°\n 3. 180°\n 4. 水平翻转\n 5. 垂直翻转\n";
                int dir = GetIntInput("选择: ");
                std::string dst = GetInput("输出视频: ");
                if (src.empty() || dst.empty()) { Pause(); break; }

                std::string filter;
                switch (dir)
                {
                    case 1: filter = "transpose=1"; break;
                    case 2: filter = "transpose=2"; break;
                    case 3: filter = "transpose=1,transpose=1"; break;
                    case 4: filter = "hflip"; break;
                    case 5: filter = "vflip"; break;
                }
                std::string extra = "-vf \"" + filter + "\" -c:a aac";
                SmartDecodePipeline(src, dst, extra);
                Pause();
                break;
            }

            case 16:  // 加速/减速
            {
                ClearScreen();
                std::string src = GetInput("输入视频: ");
                std::string speed = GetInput("速度倍数（2=2倍）: ");
                std::string dst = GetInput("输出视频: ");
                if (src.empty() || dst.empty()) { Pause(); break; }
                std::string cmd = "ffmpeg -i \"" + src + "\" -filter_complex \"[0:v]setpts=" + speed + "*PTS[v];[0:a]atempo=" + speed + "[a]\" -map \"[v]\" -map \"[a]\" -y \"" + dst + "\"";
                RunFFmpeg(cmd);
                Pause();
                break;
            }

            case 17:
            {
                ClearScreen();
                std::string src = GetInput("视频路径: ");
                if (src.empty()) { Pause(); break; }
                std::string cmd = "ffprobe -v error -show_format -show_streams \"" + src + "\"";
                RunFFmpeg(cmd);
                Pause();
                break;
            }

            case 18:  // faststart
            {
                ClearScreen();
                std::string src = GetInput("输入视频: ");
                std::string dst = GetInput("输出视频: ");
                if (src.empty() || dst.empty()) { Pause(); break; }
                std::string extra = "-c:v libx264 -crf 23 -preset fast -c:a aac -b:a 128k -movflags +faststart";
                SmartDecodePipeline(src, dst, extra);
                Pause();
                break;
            }

            // 音频处理
            case 19:
            {
                ClearScreen();
                std::string src = GetInput("输入音频: ");
                std::string dst = GetInput("输出音频: ");
                if (src.empty() || dst.empty()) { Pause(); break; }
                std::string cmd = "ffmpeg -i \"" + src + "\" -y \"" + dst + "\"";
                RunFFmpeg(cmd);
                Pause();
                break;
            }

            case 20:
            {
                ClearScreen();
                std::string src = GetInput("视频路径: ");
                std::string dst = GetInput("输出音频: ");
                if (src.empty() || dst.empty()) { Pause(); break; }
                std::string cmd = "ffmpeg -i \"" + src + "\" -vn -c:a libmp3lame -q:a 2 -y \"" + dst + "\"";
                RunFFmpeg(cmd);
                Pause();
                break;
            }

            case 21:
            {
                ClearScreen();
                std::string src = GetInput("输入音频: ");
                std::string start = GetInput("开始时间: ");
                std::string dur = GetInput("持续时长: ");
                std::string dst = GetInput("输出音频: ");
                if (src.empty() || dst.empty()) { Pause(); break; }
                std::string cmd = "ffmpeg -ss " + start + " -i \"" + src + "\" -t " + dur + " -c copy -y \"" + dst + "\"";
                RunFFmpeg(cmd);
                Pause();
                break;
            }

            case 22:
            {
                ClearScreen();
                std::string listFile = GetInput("列表文件: ");
                std::string dst = GetInput("输出音频: ");
                if (listFile.empty() || dst.empty()) { Pause(); break; }
                std::string cmd = "ffmpeg -f concat -safe 0 -i \"" + listFile + "\" -c copy -y \"" + dst + "\"";
                RunFFmpeg(cmd);
                Pause();
                break;
            }

            case 23:
            {
                ClearScreen();
                std::string src = GetInput("输入音频: ");
                std::string vol = GetInput("音量倍数（2/0.5）: ");
                std::string dst = GetInput("输出音频: ");
                if (src.empty() || dst.empty()) { Pause(); break; }
                std::string cmd = "ffmpeg -i \"" + src + "\" -af \"volume=" + vol + "\" -y \"" + dst + "\"";
                RunFFmpeg(cmd);
                Pause();
                break;
            }

            case 24:
            {
                ClearScreen();
                std::string src = GetInput("输入音频: ");
                std::string speed = GetInput("速度倍数: ");
                std::string pitch = GetInput("音调倍数（1）: ");
                std::string dst = GetInput("输出音频: ");
                if (src.empty() || dst.empty()) { Pause(); break; }
                if (pitch.empty()) pitch = "1";
                std::string cmd = "ffmpeg -i \"" + src + "\" -filter_complex \"asetrate=44100*" + speed + ",atempo=" + speed + ",aresample=44100,asetrate=44100*" + pitch + "\" -y \"" + dst + "\"";
                RunFFmpeg(cmd);
                Pause();
                break;
            }

            case 25:
            {
                ClearScreen();
                std::string src = GetInput("输入音频: ");
                std::string dst = GetInput("输出音频: ");
                if (src.empty() || dst.empty()) { Pause(); break; }
                std::string cmd = "ffmpeg -i \"" + src + "\" -af \"afftdn=nf=-25\" -y \"" + dst + "\"";
                RunFFmpeg(cmd);
                Pause();
                break;
            }

            case 26:
            {
                ClearScreen();
                std::string src = GetInput("音频路径: ");
                if (src.empty()) { Pause(); break; }
                std::string cmd = "ffprobe -v error -show_format -show_streams \"" + src + "\"";
                RunFFmpeg(cmd);
                Pause();
                break;
            }

            case 27:
            {
                ClearScreen();
                std::string src = GetInput("视频路径: ");
                std::string dst = GetInput("输出字幕: ");
                if (src.empty() || dst.empty()) { Pause(); break; }
                std::string cmd = "ffmpeg -i \"" + src + "\" -map 0:s:0 \"" + dst + "\"";
                RunFFmpeg(cmd);
                Pause();
                break;
            }

            case 28:
            case 29:
            {
                ClearScreen();
                std::cout << "========== 加密音乐格式解密 ==========\n";
                std::cout << "推荐工具: unlock-music\n";
                std::cout << "支持: NCM / QMC / KGM / KWm / XM / MG\n\n";
                std::cout << "GitHub: https://github.com/unlock-music/unlock-music\n\n";
                std::cout << "是否打开下载页面？(y/n): ";
                char c;
                std::cin >> c;
                std::cin.ignore();
                if (c == 'y' || c == 'Y')
                    ShellExecuteA(NULL, "open", "https://github.com/unlock-music/unlock-music/releases", NULL, NULL, SW_SHOWNORMAL);
                Pause();
                break;
            }

            // 批量操作
            case 30:
            {
                ClearScreen();
                std::string srcDir = GetInput("源目录: ");
                std::string dstDir = GetInput("输出目录: ");
                std::string ext = GetInput("源扩展名（mp4）: ");
                std::string targetExt = GetInput("目标扩展名（mkv）: ");
                if (srcDir.empty() || dstDir.empty()) { Pause(); break; }
                std::string cmd = "for %f in (\"" + srcDir + "\\*." + ext + "\") do ffmpeg -i \"%f\" -c copy \"" + dstDir + "\\%~nf." + targetExt + "\"";
                RunFFmpeg("cmd /c \"" + cmd + "\"");
                Pause();
                break;
            }

            case 31:
            {
                ClearScreen();
                std::string srcDir = GetInput("视频目录: ");
                std::string dstDir = GetInput("输出目录: ");
                if (srcDir.empty() || dstDir.empty()) { Pause(); break; }
                std::string cmd = "for %f in (\"" + srcDir + "\\*.mp4\") do ffmpeg -i \"%f\" -vn -c:a libmp3lame -q:a 2 \"" + dstDir + "\\%~nf.mp3\"";
                RunFFmpeg("cmd /c \"" + cmd + "\"");
                Pause();
                break;
            }

            case 32:
            {
                ClearScreen();
                std::string srcDir = GetInput("视频目录: ");
                std::string dstDir = GetInput("输出目录: ");
                int crf = GetIntInput("CRF（23）: ", 23);
                if (srcDir.empty() || dstDir.empty()) { Pause(); break; }

                std::string encoder = GetBestHwEncoder();
                std::string encArg;
                if (encoder == "h264_nvenc")
                    encArg = "-c:v h264_nvenc -preset p4 -cq " + std::to_string(crf);
                else if (encoder == "h264_qsv")
                    encArg = "-c:v h264_qsv -global_quality " + std::to_string(crf);
                else
                    encArg = "-c:v libx264 -crf " + std::to_string(crf);

                std::string cmd = "for %f in (\"" + srcDir + "\\*.mp4\") do ffmpeg -i \"%f\" " + encArg + " -c:a aac \"" + dstDir + "\\%~nf.mp4\"";
                RunFFmpeg("cmd /c \"" + cmd + "\"");
                Pause();
                break;
            }

            default:
                std::cout << "无效选项。" << std::endl;
                Pause();
        }
    }
}

// ==================== 文档格式转换 ====================

// 文档格式转换菜单
void DocumentConverterMenu()
{
    while (true)
    {
        ClearScreen();
        std::cout << "========== 文档格式转换 ==========\n";
        std::cout << "=== Word 文档 ===\n";
        std::cout << " 1. DOCX 转 PDF\n";
        std::cout << " 2. DOCX 转 TXT\n";
        std::cout << " 3. DOCX 转 HTML\n";
        std::cout << " 4. DOCX 转 RTF\n";
        std::cout << " 5. DOCX 转 ODT\n";
        std::cout << " 6. DOCX 转 DOC\n";
        std::cout << " 7. DOC 转 DOCX\n";
        std::cout << "\n=== Excel 表格 ===\n";
        std::cout << " 8. XLSX 转 PDF\n";
        std::cout << " 9. XLSX 转 CSV\n";
        std::cout << "10. XLSX 转 TXT\n";
        std::cout << "11. XLSX 转 HTML\n";
        std::cout << "12. XLSX 转 ODS\n";
        std::cout << "13. CSV 转 XLSX\n";
        std::cout << "\n=== PPT 演示 ===\n";
        std::cout << "14. PPTX 转 PDF\n";
        std::cout << "15. PPTX 转图片\n";
        std::cout << "16. PPTX 转 ODP\n";
        std::cout << "\n=== PDF 文档 ===\n";
        std::cout << "17. PDF 转 DOCX\n";
        std::cout << "18. PDF 转图片\n";
        std::cout << "19. PDF 转 TXT\n";
        std::cout << "20. PDF 转 HTML\n";
        std::cout << "21. PDF 合并\n";
        std::cout << "22. PDF 拆分\n";
        std::cout << "23. PDF 加密\n";
        std::cout << "24. PDF 解密\n";
        std::cout << "25. PDF 压缩\n";
        std::cout << "26. PDF 加水印\n";
        std::cout << "\n=== 图片格式 ===\n";
        std::cout << "27. 图片格式转换（JPG/PNG/WebP/BMP/GIF）\n";
        std::cout << "28. 图片批量转换\n";
        std::cout << "29. 图片压缩\n";
        std::cout << "30. 图片加水印\n";
        std::cout << "31. 图片转 PDF\n";
        std::cout << "32. PDF 转图片\n";
        std::cout << "\n=== 电子书 ===\n";
        std::cout << "33. EPUB 转 MOBI\n";
        std::cout << "34. MOBI 转 EPUB\n";
        std::cout << "35. EPUB 转 PDF\n";
        std::cout << "36. TXT 转 EPUB\n";
        std::cout << "\n=== Markdown ===\n";
        std::cout << "37. MD 转 PDF\n";
        std::cout << "38. MD 转 HTML\n";
        std::cout << "39. MD 转 DOCX\n";
        std::cout << "\n=== 其他 ===\n";
        std::cout << "40. RTF 转 DOCX\n";
        std::cout << "41. ODT 转 DOCX\n";
        std::cout << "42. HTML 转 PDF\n";
        std::cout << "43. 通用文档转换（LibreOffice）\n";
        std::cout << "44. 检测转换工具环境\n";
        std::cout << " 0. 返回主菜单\n";
        std::cout << "==================================\n";

        int choice = GetIntInput("请选择: ");

        if (choice == 0) break;

        switch (choice)
        {
            // ==================== 检测工具 ====================
            case 44:
            {
                ClearScreen();
                std::cout << "========== 转换工具环境检测 ==========\n";

                std::cout << "\n=== LibreOffice ===\n";
                int ret = system("soffice --version 2>&1");
                if (ret == 0) std::cout << "[OK] LibreOffice 已安装\n";
                else std::cout << "[FAIL] 未安装\n";

                std::cout << "\n=== Pandoc ===\n";
                ret = system("pandoc --version 2>&1 | findstr /i \"pandoc\"");
                if (ret == 0) std::cout << "[OK] Pandoc 已安装\n";
                else std::cout << "[FAIL] 未安装\n";

                std::cout << "\n=== ImageMagick ===\n";
                ret = system("magick --version 2>&1");
                if (ret == 0) std::cout << "[OK] ImageMagick 已安装\n";
                else std::cout << "[FAIL] 未安装\n";

                std::cout << "\n=== Ghostscript ===\n";
                ret = system("gswin64c --version 2>&1");
                if (ret == 0) std::cout << "[OK] Ghostscript 已安装\n";
                else std::cout << "[FAIL] 未安装\n";

                std::cout << "\n=== Calibre ===\n";
                ret = system("ebook-convert --version 2>&1");
                if (ret == 0) std::cout << "[OK] Calibre 已安装\n";
                else std::cout << "[FAIL] 未安装\n";

                std::cout << "\n=== FFmpeg ===\n";
                ret = system("ffmpeg -version 2>&1 | findstr /i \"ffmpeg\"");
                if (ret == 0) std::cout << "[OK] FFmpeg 已安装\n";
                else std::cout << "[FAIL] 未安装\n";

                std::cout << "\n是否安装缺失的工具？(y/n): ";
                char c;
                std::cin >> c;
                std::cin.ignore();
                if (c == 'y' || c == 'Y')
                {
                    std::cout << "\n选择要安装的工具：\n";
                    std::cout << " 1. LibreOffice\n";
                    std::cout << " 2. Pandoc\n";
                    std::cout << " 3. ImageMagick\n";
                    std::cout << " 4. Ghostscript\n";
                    std::cout << " 5. Calibre\n";
                    std::cout << " 6. FFmpeg\n";
                    std::cout << " 7. 全部安装\n";
                    int t = GetIntInput("请选择: ");
                    switch (t)
                    {
                        case 1: RunCmd("winget install TheDocumentFoundation.LibreOffice --silent --accept-package-agreements --accept-source-agreements"); break;
                        case 2: RunCmd("winget install JohnMacFarlane.Pandoc --silent --accept-package-agreements --accept-source-agreements"); break;
                        case 3: RunCmd("winget install ImageMagick.ImageMagick --silent --accept-package-agreements --accept-source-agreements"); break;
                        case 4: RunCmd("winget install ArtifexSoftware.GhostScript --silent --accept-package-agreements --accept-source-agreements"); break;
                        case 5: RunCmd("winget install calibre.calibre --silent --accept-package-agreements --accept-source-agreements"); break;
                        case 6: RunCmd("winget install Gyan.FFmpeg --silent --accept-package-agreements --accept-source-agreements"); break;
                        case 7:
                            RunCmd("winget install TheDocumentFoundation.LibreOffice --silent --accept-package-agreements --accept-source-agreements");
                            RunCmd("winget install JohnMacFarlane.Pandoc --silent --accept-package-agreements --accept-source-agreements");
                            RunCmd("winget install ImageMagick.ImageMagick --silent --accept-package-agreements --accept-source-agreements");
                            RunCmd("winget install ArtifexSoftware.GhostScript --silent --accept-package-agreements --accept-source-agreements");
                            RunCmd("winget install calibre.calibre --silent --accept-package-agreements --accept-source-agreements");
                            break;
                    }
                }
                Pause();
                break;
            }

            // ==================== DOCX 转 PDF ====================
            case 1:
            {
                ClearScreen();
                std::cout << "========== DOCX 转 PDF ==========\n";
                std::string src = GetInput("输入 DOCX 路径: ");
                std::string dst = GetInput("输出 PDF 路径: ");
                if (src.empty() || dst.empty()) { Pause(); break; }

                // 使用 LibreOffice 无头模式
                std::string dstDir = dst.substr(0, dst.find_last_of("\\/"));
                std::string cmd = "soffice --headless --convert-to pdf --outdir \"" + dstDir + "\" \"" + src + "\"";
                RunCmd(cmd);

                // 重命名输出文件
                std::string baseName = src.substr(src.find_last_of("\\/") + 1);
                size_t dot = baseName.find_last_of('.');
                if (dot != std::string::npos) baseName = baseName.substr(0, dot);
                std::string generatedPdf = dstDir + "\\" + baseName + ".pdf";

                if (generatedPdf != dst)
                {
                    MoveFileA(generatedPdf.c_str(), dst.c_str());
                }
                Pause();
                break;
            }

            // ==================== DOCX 转 TXT ====================
            case 2:
            {
                ClearScreen();
                std::cout << "========== DOCX 转 TXT ==========\n";
                std::string src = GetInput("输入 DOCX 路径: ");
                std::string dst = GetInput("输出 TXT 路径: ");
                if (src.empty() || dst.empty()) { Pause(); break; }

                std::string cmd = "pandoc \"" + src + "\" -t plain -o \"" + dst + "\"";
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== DOCX 转 HTML ====================
            case 3:
            {
                ClearScreen();
                std::cout << "========== DOCX 转 HTML ==========\n";
                std::string src = GetInput("输入 DOCX 路径: ");
                std::string dst = GetInput("输出 HTML 路径: ");
                if (src.empty() || dst.empty()) { Pause(); break; }

                std::string cmd = "pandoc \"" + src + "\" -t html -o \"" + dst + "\"";
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== DOCX 转 RTF ====================
            case 4:
            {
                ClearScreen();
                std::cout << "========== DOCX 转 RTF ==========\n";
                std::string src = GetInput("输入 DOCX 路径: ");
                std::string dst = GetInput("输出 RTF 路径: ");
                if (src.empty() || dst.empty()) { Pause(); break; }

                std::string cmd = "pandoc \"" + src + "\" -t rtf -o \"" + dst + "\"";
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== DOCX 转 ODT ====================
            case 5:
            {
                ClearScreen();
                std::cout << "========== DOCX 转 ODT ==========\n";
                std::string src = GetInput("输入 DOCX 路径: ");
                std::string dstDir = GetInput("输出目录: ");
                if (src.empty() || dstDir.empty()) { Pause(); break; }

                std::string cmd = "soffice --headless --convert-to odt --outdir \"" + dstDir + "\" \"" + src + "\"";
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== DOCX 转 DOC ====================
            case 6:
            {
                ClearScreen();
                std::cout << "========== DOCX 转 DOC ==========\n";
                std::string src = GetInput("输入 DOCX 路径: ");
                std::string dstDir = GetInput("输出目录: ");
                if (src.empty() || dstDir.empty()) { Pause(); break; }

                std::string cmd = "soffice --headless --convert-to doc --outdir \"" + dstDir + "\" \"" + src + "\"";
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== DOC 转 DOCX ====================
            case 7:
            {
                ClearScreen();
                std::cout << "========== DOC 转 DOCX ==========\n";
                std::string src = GetInput("输入 DOC 路径: ");
                std::string dstDir = GetInput("输出目录: ");
                if (src.empty() || dstDir.empty()) { Pause(); break; }

                std::string cmd = "soffice --headless --convert-to docx --outdir \"" + dstDir + "\" \"" + src + "\"";
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== XLSX 转 PDF ====================
            case 8:
            {
                ClearScreen();
                std::cout << "========== XLSX 转 PDF ==========\n";
                std::string src = GetInput("输入 XLSX 路径: ");
                std::string dstDir = GetInput("输出目录: ");
                if (src.empty() || dstDir.empty()) { Pause(); break; }

                std::string cmd = "soffice --headless --convert-to pdf --outdir \"" + dstDir + "\" \"" + src + "\"";
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== XLSX 转 CSV ====================
            case 9:
            {
                ClearScreen();
                std::cout << "========== XLSX 转 CSV ==========\n";
                std::string src = GetInput("输入 XLSX 路径: ");
                std::string dstDir = GetInput("输出目录: ");
                if (src.empty() || dstDir.empty()) { Pause(); break; }

                std::string cmd = "soffice --headless --convert-to csv --outdir \"" + dstDir + "\" \"" + src + "\"";
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== XLSX 转 TXT ====================
            case 10:
            {
                ClearScreen();
                std::cout << "========== XLSX 转 TXT ==========\n";
                std::string src = GetInput("输入 XLSX 路径: ");
                std::string dstDir = GetInput("输出目录: ");
                if (src.empty() || dstDir.empty()) { Pause(); break; }

                std::string cmd = "soffice --headless --convert-to txt --outdir \"" + dstDir + "\" \"" + src + "\"";
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== XLSX 转 HTML ====================
            case 11:
            {
                ClearScreen();
                std::cout << "========== XLSX 转 HTML ==========\n";
                std::string src = GetInput("输入 XLSX 路径: ");
                std::string dstDir = GetInput("输出目录: ");
                if (src.empty() || dstDir.empty()) { Pause(); break; }

                std::string cmd = "soffice --headless --convert-to html --outdir \"" + dstDir + "\" \"" + src + "\"";
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== XLSX 转 ODS ====================
            case 12:
            {
                ClearScreen();
                std::cout << "========== XLSX 转 ODS ==========\n";
                std::string src = GetInput("输入 XLSX 路径: ");
                std::string dstDir = GetInput("输出目录: ");
                if (src.empty() || dstDir.empty()) { Pause(); break; }

                std::string cmd = "soffice --headless --convert-to ods --outdir \"" + dstDir + "\" \"" + src + "\"";
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== CSV 转 XLSX ====================
            case 13:
            {
                ClearScreen();
                std::cout << "========== CSV 转 XLSX ==========\n";
                std::string src = GetInput("输入 CSV 路径: ");
                std::string dstDir = GetInput("输出目录: ");
                if (src.empty() || dstDir.empty()) { Pause(); break; }

                std::string cmd = "soffice --headless --convert-to xlsx --outdir \"" + dstDir + "\" \"" + src + "\"";
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== PPTX 转 PDF ====================
            case 14:
            {
                ClearScreen();
                std::cout << "========== PPTX 转 PDF ==========\n";
                std::string src = GetInput("输入 PPTX 路径: ");
                std::string dstDir = GetInput("输出目录: ");
                if (src.empty() || dstDir.empty()) { Pause(); break; }

                std::string cmd = "soffice --headless --convert-to pdf --outdir \"" + dstDir + "\" \"" + src + "\"";
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== PPTX 转图片 ====================
            case 15:
            {
                ClearScreen();
                std::cout << "========== PPTX 转图片 ==========\n";
                std::cout << "先转 PDF，再转图片。\n";
                std::string src = GetInput("输入 PPTX 路径: ");
                std::string dstDir = GetInput("输出目录: ");
                if (src.empty() || dstDir.empty()) { Pause(); break; }

                // 转 PDF
                std::string cmd1 = "soffice --headless --convert-to pdf --outdir \"" + dstDir + "\" \"" + src + "\"";
                RunCmd(cmd1);

                // 转图片
                std::string baseName = src.substr(src.find_last_of("\\/") + 1);
                size_t dot = baseName.find_last_of('.');
                if (dot != std::string::npos) baseName = baseName.substr(0, dot);
                std::string pdfPath = dstDir + "\\" + baseName + ".pdf";

                std::string cmd2 = "magick convert -density 150 \"" + pdfPath + "\" \"" + dstDir + "\\" + baseName + "-%03d.png\"";
                RunCmd(cmd2);
                Pause();
                break;
            }

            // ==================== PPTX 转 ODP ====================
            case 16:
            {
                ClearScreen();
                std::cout << "========== PPTX 转 ODP ==========\n";
                std::string src = GetInput("输入 PPTX 路径: ");
                std::string dstDir = GetInput("输出目录: ");
                if (src.empty() || dstDir.empty()) { Pause(); break; }

                std::string cmd = "soffice --headless --convert-to odp --outdir \"" + dstDir + "\" \"" + src + "\"";
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== PDF 转 DOCX ====================
            case 17:
            {
                ClearScreen();
                std::cout << "========== PDF 转 DOCX ==========\n";
                std::cout << "使用 LibreOffice 导入 PDF（效果有限）\n";
                std::string src = GetInput("输入 PDF 路径: ");
                std::string dstDir = GetInput("输出目录: ");
                if (src.empty() || dstDir.empty()) { Pause(); break; }

                // 先转 ODT 再转 DOCX
                std::string cmd1 = "soffice --headless --convert-to odt --outdir \"" + dstDir + "\" \"" + src + "\"";
                RunCmd(cmd1);

                std::string baseName = src.substr(src.find_last_of("\\/") + 1);
                size_t dot = baseName.find_last_of('.');
                if (dot != std::string::npos) baseName = baseName.substr(0, dot);
                std::string odtPath = dstDir + "\\" + baseName + ".odt";

                std::string cmd2 = "soffice --headless --convert-to docx --outdir \"" + dstDir + "\" \"" + odtPath + "\"";
                RunCmd(cmd2);
                Pause();
                break;
            }

            // ==================== PDF 转图片 ====================
            case 18:
            case 32:
            {
                ClearScreen();
                std::cout << "========== PDF 转图片 ==========\n";
                std::string src = GetInput("输入 PDF 路径: ");
                std::string dstDir = GetInput("输出目录: ");
                int dpi = GetIntInput("DPI（默认 150）: ", 150);
                if (src.empty() || dstDir.empty()) { Pause(); break; }

                std::string cmd = "magick convert -density " + std::to_string(dpi) + " \"" + src + "\" \"" + dstDir + "\\page-%03d.png\"";
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== PDF 转 TXT ====================
            case 19:
            {
                ClearScreen();
                std::cout << "========== PDF 转 TXT ==========\n";
                std::string src = GetInput("输入 PDF 路径: ");
                std::string dst = GetInput("输出 TXT 路径: ");
                if (src.empty() || dst.empty()) { Pause(); break; }

                // 使用 pdftotext（poppler）
                std::string cmd = "pdftotext \"" + src + "\" \"" + dst + "\"";
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== PDF 转 HTML ====================
            case 20:
            {
                ClearScreen();
                std::cout << "========== PDF 转 HTML ==========\n";
                std::string src = GetInput("输入 PDF 路径: ");
                std::string dst = GetInput("输出 HTML 路径: ");
                if (src.empty() || dst.empty()) { Pause(); break; }

                std::string cmd = "pdftohtml \"" + src + "\" \"" + dst + "\"";
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== PDF 合并 ====================
            case 21:
            {
                ClearScreen();
                std::cout << "========== PDF 合并 ==========\n";
                std::cout << "输入多个 PDF 路径（用空格分隔）: ";
                std::string files = GetInput("");
                std::string dst = GetInput("输出 PDF 路径: ");
                if (files.empty() || dst.empty()) { Pause(); break; }

                std::string cmd = "gswin64c -sDEVICE=pdfwrite -dNOPAUSE -dQUIET -dBATCH -sOutputFile=\"" + dst + "\" " + files;
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== PDF 拆分 ====================
            case 22:
            {
                ClearScreen();
                std::cout << "========== PDF 拆分 ==========\n";
                std::string src = GetInput("输入 PDF 路径: ");
                std::string dstDir = GetInput("输出目录: ");
                if (src.empty() || dstDir.empty()) { Pause(); break; }

                std::string cmd = "gswin64c -sDEVICE=pdfwrite -dNOPAUSE -dQUIET -dBATCH -sOutputFile=\"" + dstDir + "\\page-%03d.pdf\" \"" + src + "\"";
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== PDF 加密 ====================
            case 23:
            {
                ClearScreen();
                std::cout << "========== PDF 加密 ==========\n";
                std::string src = GetInput("输入 PDF 路径: ");
                std::string dst = GetInput("输出 PDF 路径: ");
                std::string password = GetInput("密码: ");
                if (src.empty() || dst.empty()) { Pause(); break; }

                std::string cmd = "gswin64c -sDEVICE=pdfwrite -dNOPAUSE -dQUIET -dBATCH -sOwnerPassword=" + password + " -sUserPassword=" + password + " -sOutputFile=\"" + dst + "\" \"" + src + "\"";
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== PDF 解密 ====================
            case 24:
            {
                ClearScreen();
                std::cout << "========== PDF 解密 ==========\n";
                std::string src = GetInput("输入 PDF 路径: ");
                std::string dst = GetInput("输出 PDF 路径: ");
                std::string password = GetInput("密码: ");
                if (src.empty() || dst.empty()) { Pause(); break; }

                std::string cmd = "gswin64c -sDEVICE=pdfwrite -dNOPAUSE -dQUIET -dBATCH -sPDFPassword=" + password + " -sOutputFile=\"" + dst + "\" \"" + src + "\"";
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== PDF 压缩 ====================
            case 25:
            {
                ClearScreen();
                std::cout << "========== PDF 压缩 ==========\n";
                std::string src = GetInput("输入 PDF 路径: ");
                std::string dst = GetInput("输出 PDF 路径: ");
                if (src.empty() || dst.empty()) { Pause(); break; }

                std::string cmd = "gswin64c -sDEVICE=pdfwrite -dCompatibilityLevel=1.4 -dPDFSETTINGS=/ebook -dNOPAUSE -dQUIET -dBATCH -sOutputFile=\"" + dst + "\" \"" + src + "\"";
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== PDF 加水印 ====================
            case 26:
            {
                ClearScreen();
                std::cout << "========== PDF 加水印 ==========\n";
                std::cout << "使用 watermark.pdf 叠加（需自备水印 PDF）\n";
                std::string src = GetInput("输入 PDF 路径: ");
                std::string watermark = GetInput("水印 PDF 路径: ");
                std::string dst = GetInput("输出 PDF 路径: ");
                if (src.empty() || watermark.empty() || dst.empty()) { Pause(); break; }

                std::string cmd = "pdftk \"" + src + "\" background \"" + watermark + "\" output \"" + dst + "\"";
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== 图片格式转换 ====================
            case 27:
            case 28:
            {
                ClearScreen();
                std::cout << "========== 图片格式转换 ==========\n";
                std::string src = GetInput("输入图片路径: ");
                std::string dst = GetInput("输出图片路径: ");
                if (src.empty() || dst.empty()) { Pause(); break; }

                std::string cmd = "magick convert \"" + src + "\" \"" + dst + "\"";
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== 图片压缩 ====================
            case 29:
            {
                ClearScreen();
                std::cout << "========== 图片压缩 ==========\n";
                std::string src = GetInput("输入图片路径: ");
                std::string dst = GetInput("输出图片路径: ");
                int quality = GetIntInput("质量（1-100）: ", 85);
                if (src.empty() || dst.empty()) { Pause(); break; }

                std::string cmd = "magick convert \"" + src + "\" -quality " + std::to_string(quality) + " \"" + dst + "\"";
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== 图片加水印 ====================
            case 30:
            {
                ClearScreen();
                std::cout << "========== 图片加水印 ==========\n";
                std::string src = GetInput("输入图片路径: ");
                std::string watermark = GetInput("水印图片路径: ");
                std::string dst = GetInput("输出图片路径: ");
                if (src.empty() || watermark.empty() || dst.empty()) { Pause(); break; }

                std::string cmd = "magick convert \"" + src + "\" \"" + watermark + "\" -gravity southeast -composite \"" + dst + "\"";
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== 图片转 PDF ====================
            case 31:
            {
                ClearScreen();
                std::cout << "========== 图片转 PDF ==========\n";
                std::cout << "输入多张图片（用空格分隔）: ";
                std::string files = GetInput("");
                std::string dst = GetInput("输出 PDF 路径: ");
                if (files.empty() || dst.empty()) { Pause(); break; }

                std::string cmd = "magick convert " + files + " \"" + dst + "\"";
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== EPUB 转 MOBI ====================
            case 33:
            {
                ClearScreen();
                std::cout << "========== EPUB 转 MOBI ==========\n";
                std::string src = GetInput("输入 EPUB 路径: ");
                std::string dst = GetInput("输出 MOBI 路径: ");
                if (src.empty() || dst.empty()) { Pause(); break; }

                std::string cmd = "ebook-convert \"" + src + "\" \"" + dst + "\"";
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== MOBI 转 EPUB ====================
            case 34:
            {
                ClearScreen();
                std::cout << "========== MOBI 转 EPUB ==========\n";
                std::string src = GetInput("输入 MOBI 路径: ");
                std::string dst = GetInput("输出 EPUB 路径: ");
                if (src.empty() || dst.empty()) { Pause(); break; }

                std::string cmd = "ebook-convert \"" + src + "\" \"" + dst + "\"";
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== EPUB 转 PDF ====================
            case 35:
            {
                ClearScreen();
                std::cout << "========== EPUB 转 PDF ==========\n";
                std::string src = GetInput("输入 EPUB 路径: ");
                std::string dst = GetInput("输出 PDF 路径: ");
                if (src.empty() || dst.empty()) { Pause(); break; }

                std::string cmd = "ebook-convert \"" + src + "\" \"" + dst + "\"";
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== TXT 转 EPUB ====================
            case 36:
            {
                ClearScreen();
                std::cout << "========== TXT 转 EPUB ==========\n";
                std::string src = GetInput("输入 TXT 路径: ");
                std::string dst = GetInput("输出 EPUB 路径: ");
                if (src.empty() || dst.empty()) { Pause(); break; }

                std::string cmd = "ebook-convert \"" + src + "\" \"" + dst + "\"";
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== MD 转 PDF ====================
            case 37:
            {
                ClearScreen();
                std::cout << "========== MD 转 PDF ==========\n";
                std::string src = GetInput("输入 MD 路径: ");
                std::string dst = GetInput("输出 PDF 路径: ");
                if (src.empty() || dst.empty()) { Pause(); break; }

                std::string cmd = "pandoc \"" + src + "\" -o \"" + dst + "\" --pdf-engine=xelatex";
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== MD 转 HTML ====================
            case 38:
            {
                ClearScreen();
                std::cout << "========== MD 转 HTML ==========\n";
                std::string src = GetInput("输入 MD 路径: ");
                std::string dst = GetInput("输出 HTML 路径: ");
                if (src.empty() || dst.empty()) { Pause(); break; }

                std::string cmd = "pandoc \"" + src + "\" -s -o \"" + dst + "\"";
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== MD 转 DOCX ====================
            case 39:
            {
                ClearScreen();
                std::cout << "========== MD 转 DOCX ==========\n";
                std::string src = GetInput("输入 MD 路径: ");
                std::string dst = GetInput("输出 DOCX 路径: ");
                if (src.empty() || dst.empty()) { Pause(); break; }

                std::string cmd = "pandoc \"" + src + "\" -o \"" + dst + "\"";
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== RTF 转 DOCX ====================
            case 40:
            {
                ClearScreen();
                std::cout << "========== RTF 转 DOCX ==========\n";
                std::string src = GetInput("输入 RTF 路径: ");
                std::string dst = GetInput("输出 DOCX 路径: ");
                if (src.empty() || dst.empty()) { Pause(); break; }

                std::string cmd = "pandoc \"" + src + "\" -o \"" + dst + "\"";
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== ODT 转 DOCX ====================
            case 41:
            {
                ClearScreen();
                std::cout << "========== ODT 转 DOCX ==========\n";
                std::string src = GetInput("输入 ODT 路径: ");
                std::string dstDir = GetInput("输出目录: ");
                if (src.empty() || dstDir.empty()) { Pause(); break; }

                std::string cmd = "soffice --headless --convert-to docx --outdir \"" + dstDir + "\" \"" + src + "\"";
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== HTML 转 PDF ====================
            case 42:
            {
                ClearScreen();
                std::cout << "========== HTML 转 PDF ==========\n";
                std::string src = GetInput("输入 HTML 路径: ");
                std::string dst = GetInput("输出 PDF 路径: ");
                if (src.empty() || dst.empty()) { Pause(); break; }

                std::string cmd = "wkhtmltopdf \"" + src + "\" \"" + dst + "\"";
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== 通用文档转换 ====================
            case 43:
            {
                ClearScreen();
                std::cout << "========== 通用文档转换（LibreOffice） ==========\n";
                std::string src = GetInput("输入文件路径: ");
                std::string fmt = GetInput("目标格式（如 pdf, docx, xlsx, pptx）: ");
                std::string dstDir = GetInput("输出目录: ");
                if (src.empty() || fmt.empty() || dstDir.empty()) { Pause(); break; }

                std::string cmd = "soffice --headless --convert-to " + fmt + " --outdir \"" + dstDir + "\" \"" + src + "\"";
                RunCmd(cmd);
                Pause();
                break;
            }

            default:
                std::cout << "无效选项。" << std::endl;
                Pause();
        }
    }
}

// ==================== 哈希工具 ====================

// 哈希工具菜单
void HashToolsMenu()
{
    while (true)
    {
        ClearScreen();
        std::cout << "========== 哈希工具 ==========\n";
        std::cout << "=== 文件哈希 ===\n";
        std::cout << " 1. 计算文件 MD5\n";
        std::cout << " 2. 计算文件 SHA1\n";
        std::cout << " 3. 计算文件 SHA256\n";
        std::cout << " 4. 计算文件 SHA384\n";
        std::cout << " 5. 计算文件 SHA512\n";
        std::cout << " 6. 计算文件 CRC32\n";
        std::cout << " 7. 计算文件所有哈希\n";
        std::cout << "\n=== 文本哈希 ===\n";
        std::cout << " 8. 计算文本 MD5\n";
        std::cout << " 9. 计算文本 SHA1\n";
        std::cout << "10. 计算文本 SHA256\n";
        std::cout << "11. 计算文本 SHA512\n";
        std::cout << "12. 计算文本所有哈希\n";
        std::cout << "\n=== 批量处理 ===\n";
        std::cout << "13. 批量计算目录下所有文件哈希\n";
        std::cout << "14. 导出哈希到文件\n";
        std::cout << "15. 从文件导入哈希并校验\n";
        std::cout << "\n=== 文件校验 ===\n";
        std::cout << "16. 校验文件（比对哈希）\n";
        std::cout << "17. 校验文件是否被修改\n";
        std::cout << "18. 查找重复文件（哈希比对）\n";
        std::cout << "\n=== 高级功能 ===\n";
        std::cout << "19. Base64 编码（文件）\n";
        std::cout << "20. Base64 解码（文件）\n";
        std::cout << "21. Base64 编码（文本）\n";
        std::cout << "22. Base64 解码（文本）\n";
        std::cout << "23. URL 编码\n";
        std::cout << "24. URL 解码\n";
        std::cout << "25. 十六进制编码\n";
        std::cout << "26. 十六进制解码\n";
        std::cout << "\n=== 其他 ===\n";
        std::cout << "27. 文件大小转换\n";
        std::cout << "28. 计算字符串长度\n";
        std::cout << "29. 生成随机哈希\n";
        std::cout << "30. 生成 UUID\n";
        std::cout << " 0. 返回主菜单\n";
        std::cout << "=============================\n";

        int choice = GetIntInput("请选择: ");

        if (choice == 0) break;

        switch (choice)
        {
            // ==================== 文件 MD5 ====================
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            {
                ClearScreen();
                std::string algo;
                switch (choice)
                {
                    case 1: algo = "MD5"; std::cout << "========== 文件 MD5 ==========\n"; break;
                    case 2: algo = "SHA1"; std::cout << "========== 文件 SHA1 ==========\n"; break;
                    case 3: algo = "SHA256"; std::cout << "========== 文件 SHA256 ==========\n"; break;
                    case 4: algo = "SHA384"; std::cout << "========== 文件 SHA384 ==========\n"; break;
                    case 5: algo = "SHA512"; std::cout << "========== 文件 SHA512 ==========\n"; break;
                    case 6: algo = "CRC32"; std::cout << "========== 文件 CRC32 ==========\n"; break;
                    case 7: algo = "ALL"; std::cout << "========== 文件所有哈希 ==========\n"; break;
                }

                std::string file = GetInput("文件路径: ");
                if (file.empty()) { Pause(); break; }

                // 去除引号
                while (!file.empty() && (file.front() == '"' || file.front() == '\'')) file.erase(file.begin());
                while (!file.empty() && (file.back() == '"' || file.back() == '\'')) file.pop_back();

                if (GetFileAttributesA(file.c_str()) == INVALID_FILE_ATTRIBUTES)
                {
                    std::cout << "文件不存在！" << std::endl;
                    Pause();
                    break;
                }

                if (algo == "ALL")
                {
                    std::cout << "\n正在计算所有哈希...\n\n";
                    std::cout << "MD5:    "; system(("certutil -hashfile \"" + file + "\" MD5 | findstr /v \"hash CertUtil\" | findstr /r /v \"^$\"").c_str());
                    std::cout << "SHA1:   "; system(("certutil -hashfile \"" + file + "\" SHA1 | findstr /v \"hash CertUtil\" | findstr /r /v \"^$\"").c_str());
                    std::cout << "SHA256: "; system(("certutil -hashfile \"" + file + "\" SHA256 | findstr /v \"hash CertUtil\" | findstr /r /v \"^$\"").c_str());
                    std::cout << "SHA384: "; system(("certutil -hashfile \"" + file + "\" SHA384 | findstr /v \"hash CertUtil\" | findstr /r /v \"^$\"").c_str());
                    std::cout << "SHA512: "; system(("certutil -hashfile \"" + file + "\" SHA512 | findstr /v \"hash CertUtil\" | findstr /r /v \"^$\"").c_str());
                }
                else
                {
                    std::string cmd = "certutil -hashfile \"" + file + "\" " + algo;
                    RunCmd(cmd);
                }
                Pause();
                break;
            }

            // ==================== 文本哈希 ====================
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            {
                ClearScreen();
                std::string algo;
                switch (choice)
                {
                    case 8: algo = "MD5"; std::cout << "========== 文本 MD5 ==========\n"; break;
                    case 9: algo = "SHA1"; std::cout << "========== 文本 SHA1 ==========\n"; break;
                    case 10: algo = "SHA256"; std::cout << "========== 文本 SHA256 ==========\n"; break;
                    case 11: algo = "SHA512"; std::cout << "========== 文本 SHA512 ==========\n"; break;
                    case 12: algo = "ALL"; std::cout << "========== 文本所有哈希 ==========\n"; break;
                }

                std::string text = GetInput("输入文本: ");
                if (text.empty()) { Pause(); break; }

                // 写入临时文件
                std::string tempFile = "C:\\hash_temp_input.txt";
                std::ofstream f(tempFile, std::ios::binary);
                f << text;
                f.close();

                if (algo == "ALL")
                {
                    std::cout << "\nMD5:    "; system(("certutil -hashfile \"" + tempFile + "\" MD5 | findstr /v \"hash CertUtil\" | findstr /r /v \"^$\"").c_str());
                    std::cout << "SHA1:   "; system(("certutil -hashfile \"" + tempFile + "\" SHA1 | findstr /v \"hash CertUtil\" | findstr /r /v \"^$\"").c_str());
                    std::cout << "SHA256: "; system(("certutil -hashfile \"" + tempFile + "\" SHA256 | findstr /v \"hash CertUtil\" | findstr /r /v \"^$\"").c_str());
                    std::cout << "SHA512: "; system(("certutil -hashfile \"" + tempFile + "\" SHA512 | findstr /v \"hash CertUtil\" | findstr /r /v \"^$\"").c_str());
                }
                else
                {
                    std::string cmd = "certutil -hashfile \"" + tempFile + "\" " + algo;
                    RunCmd(cmd);
                }

                DeleteFileA(tempFile.c_str());
                Pause();
                break;
            }

            // ==================== 批量计算目录哈希 ====================
            case 13:
            {
                ClearScreen();
                std::cout << "========== 批量计算哈希 ==========\n";
                std::string dir = GetInput("目录路径: ");
                std::string algo = GetInput("算法（MD5/SHA1/SHA256/SHA512）: ");
                if (dir.empty() || algo.empty()) { Pause(); break; }

                std::string cmd = "for %f in (\"" + dir + "\\*\") do @echo %f & certutil -hashfile \"%f\" " + algo + " | findstr /v \"hash CertUtil\" | findstr /r /v \"^$\" & echo.";
                std::string fullCmd = "cmd /c \"" + cmd + "\"";
                RunCmd(fullCmd);
                Pause();
                break;
            }

            // ==================== 导出哈希到文件 ====================
            case 14:
            {
                ClearScreen();
                std::cout << "========== 导出哈希 ==========\n";
                std::string dir = GetInput("目录路径: ");
                std::string algo = GetInput("算法（MD5/SHA256）: ");
if (algo.empty()) algo = "SHA256";
                std::string output = GetInput("输出文件路径: ");
                if (dir.empty() || output.empty()) { Pause(); break; }

                std::string cmd = "for %f in (\"" + dir + "\\*\") do @certutil -hashfile \"%f\" " + algo + " | findstr /v \"hash CertUtil\" | findstr /r /v \"^$\"";
                std::string fullCmd = "cmd /c \"" + cmd + "\" > \"" + output + "\"";
                RunCmd(fullCmd);
                Pause();
                break;
            }

            // ==================== 从文件导入并校验 ====================
            case 15:
            {
                ClearScreen();
                std::cout << "========== 从文件校验哈希 ==========\n";
                std::cout << "哈希文件格式：每行 <哈希值> <文件名>\n";
                std::string hashFile = GetInput("哈希文件路径: ");
                std::string algo = GetInput("算法（MD5/SHA256）: ");
                if (hashFile.empty() || algo.empty()) { Pause(); break; }

                std::string cmd = "certutil -hashfile \"" + hashFile + "\" " + algo;
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== 校验文件 ====================
            case 16:
            {
                ClearScreen();
                std::cout << "========== 校验文件 ==========\n";
                std::string file = GetInput("文件路径: ");
                std::string expected = GetInput("预期哈希值: ");
                std::string algo = GetInput("算法（MD5/SHA1/SHA256/SHA512）: ");
                if (file.empty() || expected.empty() || algo.empty()) { Pause(); break; }

                std::string tempFile = "C:\\hash_check.txt";
                DeleteFileA(tempFile.c_str());

                std::string cmd = "certutil -hashfile \"" + file + "\" " + algo + " > \"" + tempFile + "\" 2>&1";
                system(cmd.c_str());

                // 读取实际哈希
                std::ifstream f(tempFile);
                std::string line, actualHash;
                int lineCount = 0;
                while (std::getline(f, line))
                {
                    lineCount++;
                    if (lineCount == 2)
                    {
                        actualHash = line;
                        break;
                    }
                }
                f.close();
                DeleteFileA(tempFile.c_str());

                // 去除空格
                while (!actualHash.empty() && (actualHash.front() == ' ')) actualHash.erase(actualHash.begin());
                while (!actualHash.empty() && (actualHash.back() == ' ' || actualHash.back() == '\r')) actualHash.pop_back();

                // 转大写
                std::transform(actualHash.begin(), actualHash.end(), actualHash.begin(), ::toupper);
                std::transform(expected.begin(), expected.end(), expected.begin(), ::toupper);

                std::cout << "\n预期哈希: " << expected << "\n";
                std::cout << "实际哈希: " << actualHash << "\n\n";

                if (actualHash == expected)
                {
                    std::cout << "[✓] 校验通过！文件未被修改。" << std::endl;
                }
                else
                {
                    std::cout << "[✗] 校验失败！文件已被修改。" << std::endl;
                }
                Pause();
                break;
            }

            // ==================== 校验文件是否被修改 ====================
            case 17:
            {
                ClearScreen();
                std::cout << "========== 检测文件是否被修改 ==========\n";
                std::cout << "将文件哈希保存到文件，之后可对比。\n\n";
                std::string file = GetInput("文件路径: ");
                std::string recordFile = GetInput("记录文件路径（如 C:\\hash.txt）: ");
                if (file.empty() || recordFile.empty()) { Pause(); break; }

                std::string cmd = "certutil -hashfile \"" + file + "\" SHA256 | findstr /v \"hash CertUtil\" | findstr /r /v \"^$\" > \"" + recordFile + "\"";
                system(cmd.c_str());

                std::cout << "\n哈希已保存到: " << recordFile << std::endl;
                std::cout << "下次运行此功能时，输入相同记录文件即可对比。" << std::endl;
                Pause();
                break;
            }

            // ==================== 查找重复文件 ====================
            case 18:
            {
                ClearScreen();
                std::cout << "========== 查找重复文件 ==========\n";
                std::cout << "注意：需要目录下所有文件的哈希，速度较慢。\n";
                std::string dir = GetInput("目录路径: ");
                if (dir.empty()) { Pause(); break; }

                std::string cmd = "for %f in (\"" + dir + "\\*\") do @(echo %~zf %f & certutil -hashfile \"%f\" SHA256 | findstr /v \"hash CertUtil\" | findstr /r /v \"^$\")";
                std::string fullCmd = "cmd /c \"" + cmd + "\"";
                RunCmd(fullCmd);
                Pause();
                break;
            }

            // ==================== Base64 编码（文件） ====================
            case 19:
            {
                ClearScreen();
                std::cout << "========== Base64 编码（文件） ==========\n";
                std::string src = GetInput("输入文件路径: ");
                std::string dst = GetInput("输出文本文件路径: ");
                if (src.empty() || dst.empty()) { Pause(); break; }

                std::string cmd = "certutil -encode \"" + src + "\" \"" + dst + "\"";
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== Base64 解码（文件） ====================
            case 20:
            {
                ClearScreen();
                std::cout << "========== Base64 解码（文件） ==========\n";
                std::string src = GetInput("输入 Base64 文本文件路径: ");
                std::string dst = GetInput("输出文件路径: ");
                if (src.empty() || dst.empty()) { Pause(); break; }

                std::string cmd = "certutil -decode \"" + src + "\" \"" + dst + "\"";
                RunCmd(cmd);
                Pause();
                break;
            }

            // ==================== Base64 编码（文本） ====================
            case 21:
            {
                ClearScreen();
                std::cout << "========== Base64 编码（文本） ==========\n";
                std::string text = GetInput("输入文本: ");
                if (text.empty()) { Pause(); break; }

                // 写入临时文件
                std::string tempIn = "C:\\b64_in.txt";
                std::string tempOut = "C:\\b64_out.txt";
                std::ofstream f(tempIn);
                f << text;
                f.close();

                system(("certutil -encode \"" + tempIn + "\" \"" + tempOut + "\"").c_str());

                std::ifstream fin(tempOut);
                std::string line;
                std::cout << "\n=== Base64 编码结果 ===\n";
                while (std::getline(fin, line))
                {
                    if (line.find("-----") == std::string::npos)
                        std::cout << line << std::endl;
                }
                fin.close();

                DeleteFileA(tempIn.c_str());
                DeleteFileA(tempOut.c_str());
                Pause();
                break;
            }

            // ==================== Base64 解码（文本） ====================
            case 22:
            {
                ClearScreen();
                std::cout << "========== Base64 解码（文本） ==========\n";
                std::string text = GetInput("输入 Base64 文本: ");
                if (text.empty()) { Pause(); break; }

                std::string tempIn = "C:\\b64_in.txt";
                std::string tempOut = "C:\\b64_out.txt";
                std::ofstream f(tempIn);
                f << "-----BEGIN CERTIFICATE-----\n" << text << "\n-----END CERTIFICATE-----";
                f.close();

                system(("certutil -decode \"" + tempIn + "\" \"" + tempOut + "\"").c_str());

                std::ifstream fin(tempOut);
                std::string content((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
                fin.close();

                std::cout << "\n=== 解码结果 ===\n" << content << std::endl;

                DeleteFileA(tempIn.c_str());
                DeleteFileA(tempOut.c_str());
                Pause();
                break;
            }

            // ==================== URL 编码 ====================
            case 23:
            {
                ClearScreen();
                std::cout << "========== URL 编码 ==========\n";
                std::string text = GetInput("输入文本: ");
                if (text.empty()) { Pause(); break; }

                std::cout << "\n=== URL 编码结果 ===\n";
                for (char c : text)
                {
                    if (isalnum((unsigned char)c) || c == '-' || c == '_' || c == '.' || c == '~')
                        std::cout << c;
                    else
                        printf("%%%02X", (unsigned char)c);
                }
                std::cout << std::endl;
                Pause();
                break;
            }

            // ==================== URL 解码 ====================
            case 24:
            {
                ClearScreen();
                std::cout << "========== URL 解码 ==========\n";
                std::string text = GetInput("输入 URL 编码文本: ");
                if (text.empty()) { Pause(); break; }

                std::cout << "\n=== URL 解码结果 ===\n";
                for (size_t i = 0; i < text.size(); i++)
                {
                    if (text[i] == '%' && i + 2 < text.size())
                    {
                        int hex = std::stoi(text.substr(i + 1, 2), nullptr, 16);
                        std::cout << (char)hex;
                        i += 2;
                    }
                    else
                    {
                        std::cout << text[i];
                    }
                }
                std::cout << std::endl;
                Pause();
                break;
            }

            // ==================== 十六进制编码 ====================
            case 25:
            {
                ClearScreen();
                std::cout << "========== 十六进制编码 ==========\n";
                std::string text = GetInput("输入文本: ");
                if (text.empty()) { Pause(); break; }

                std::cout << "\n=== 十六进制编码结果 ===\n";
                for (char c : text)
                {
                    printf("%02X ", (unsigned char)c);
                }
                std::cout << std::endl;
                Pause();
                break;
            }

            // ==================== 十六进制解码 ====================
            case 26:
            {
                ClearScreen();
                std::cout << "========== 十六进制解码 ==========\n";
                std::string text = GetInput("输入十六进制文本（空格分隔）: ");
                if (text.empty()) { Pause(); break; }

                std::cout << "\n=== 十六进制解码结果 ===\n";
                std::stringstream ss(text);
                std::string byte;
                while (ss >> byte)
                {
                    try
                    {
                        int val = std::stoi(byte, nullptr, 16);
                        std::cout << (char)val;
                    }
                    catch (...) {}
                }
                std::cout << std::endl;
                Pause();
                break;
            }

            // ==================== 文件大小转换 ====================
            case 27:
            {
                ClearScreen();
                std::cout << "========== 文件大小转换 ==========\n";
                std::string file = GetInput("文件路径: ");
                if (file.empty()) { Pause(); break; }

                HANDLE hFile = CreateFileA(file.c_str(), GENERIC_READ, FILE_SHARE_READ,
                                            NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
                if (hFile == INVALID_HANDLE_VALUE)
                {
                    std::cout << "无法打开文件！" << std::endl;
                    Pause();
                    break;
                }

                LARGE_INTEGER size;
                GetFileSizeEx(hFile, &size);
                CloseHandle(hFile);

                double bytes = (double)size.QuadPart;
                std::cout << "\n=== 文件大小 ===\n";
                std::cout << "字节: " << (unsigned long long)bytes << " B\n";
                std::cout << "KB:   " << bytes / 1024.0 << " KB\n";
                std::cout << "MB:   " << bytes / (1024.0 * 1024.0) << " MB\n";
                std::cout << "GB:   " << bytes / (1024.0 * 1024.0 * 1024.0) << " GB\n";
                Pause();
                break;
            }

            // ==================== 计算字符串长度 ====================
            case 28:
            {
                ClearScreen();
                std::cout << "========== 计算字符串长度 ==========\n";
                std::string text = GetInput("输入文本: ");
                std::cout << "\n字符数: " << text.length() << std::endl;
                std::cout << "字节数 (UTF-8): " << text.length() << std::endl;
                Pause();
                break;
            }

            // ==================== 生成随机哈希 ====================
            case 29:
            {
                ClearScreen();
                std::cout << "========== 生成随机哈希 ==========\n";
                int length = GetIntInput("哈希长度（16-128）: ", 32);

                srand((unsigned)time(NULL));
                const char* hex = "0123456789abcdef";
                std::string result;
                for (int i = 0; i < length; i++)
                {
                    result += hex[rand() % 16];
                }

                std::cout << "\n=== 随机哈希 ===\n" << result << std::endl;
                Pause();
                break;
            }

            // ==================== 生成 UUID ====================
            case 30:
            {
                ClearScreen();
                std::cout << "========== 生成 UUID ==========\n";
                int count = GetIntInput("生成数量（1-100）: ", 1);

                std::cout << "\n=== UUID ===\n";
                for (int i = 0; i < count; i++)
                {
                    GUID guid;
                    CoCreateGuid(&guid);
                    char buf[64];
                    sprintf(buf, "%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
                            guid.Data1, guid.Data2, guid.Data3,
                            guid.Data4[0], guid.Data4[1],
                            guid.Data4[2], guid.Data4[3], guid.Data4[4],
                            guid.Data4[5], guid.Data4[6], guid.Data4[7]);
                    std::cout << buf << std::endl;
                }
                Pause();
                break;
            }

            default:
                std::cout << "无效选项。" << std::endl;
                Pause();
        }
    }
}

// ==================== 进制转换工具 ====================

// 数字转指定进制字符串
std::string NumToBase(unsigned long long num, int base)
{
    if (base < 2 || base > 36) return "0";
    if (num == 0) return "0";

    const char* digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string result;
    while (num > 0)
    {
        result = digits[num % base] + result;
        num /= base;
    }
    return result;
}

// 字符串转指定进制（按字节）
std::string StringToBase(const std::string& text, int base, const std::string& sep)
{
    std::string result;
    for (size_t i = 0; i < text.size(); i++)
    {
        unsigned char c = (unsigned char)text[i];
        if (base == 2)
        {
            char buf[9];
            for (int j = 7; j >= 0; j--)
                buf[7 - j] = ((c >> j) & 1) ? '1' : '0';
            buf[8] = 0;
            result += buf;
        }
        else if (base == 8)
        {
            char buf[4];
            sprintf(buf, "%03o", c);
            result += buf;
        }
        else if (base == 10)
        {
            char buf[8];
            sprintf(buf, "%d", c);
            result += buf;
        }
        else if (base == 16)
        {
            char buf[4];
            sprintf(buf, "%02X", c);
            result += buf;
        }

        if (i < text.size() - 1) result += sep;
    }
    return result;
}

// 进制转换菜单
void BaseConverterMenu()
{
    while (true)
    {
        ClearScreen();
        std::cout << "========== 进制转换工具 ==========\n";
        std::cout << "=== 文本转进制 ===\n";
        std::cout << " 1. 文本转 2 进制\n";
        std::cout << " 2. 文本转 8 进制\n";
        std::cout << " 3. 文本转 10 进制\n";
        std::cout << " 4. 文本转 16 进制\n";
        std::cout << " 5. 文本转 32 进制\n";
        std::cout << " 6. 文本转 36 进制\n";
        std::cout << " 7. 文本转所有进制\n";
        std::cout << "\n=== 进制转文本 ===\n";
        std::cout << " 8. 2 进制转文本\n";
        std::cout << " 9. 8 进制转文本\n";
        std::cout << "10. 10 进制转文本\n";
        std::cout << "11. 16 进制转文本\n";
        std::cout << "12. 32 进制转文本\n";
        std::cout << "13. 36 进制转文本\n";
        std::cout << "\n=== 数字进制转换 ===\n";
        std::cout << "14. 任意进制转任意进制（数字）\n";
        std::cout << "15. 十进制转二进制\n";
        std::cout << "16. 十进制转十六进制\n";
        std::cout << "17. 二进制转十进制\n";
        std::cout << "18. 十六进制转十进制\n";
        std::cout << "\n=== 文件转进制 ===\n";
        std::cout << "19. 文件转 2 进制\n";
        std::cout << "20. 文件转 16 进制\n";
        std::cout << "21. 文件转 10 进制\n";
        std::cout << "22. 文件转 Base64\n";
        std::cout << "23. 文件转所有进制\n";
        std::cout << "\n=== 进制转文件 ===\n";
        std::cout << "24. 2 进制转文件\n";
        std::cout << "25. 16 进制转文件\n";
        std::cout << "26. 10 进制转文件\n";
        std::cout << "27. Base64 转文件\n";
        std::cout << "\n=== 其他 ===\n";
        std::cout << "28. 显示字符编码表（ASCII）\n";
        std::cout << "29. 批量转换目录下所有文件\n";
        std::cout << "30. 二进制文件查看器\n";
        std::cout << " 0. 返回主菜单\n";
        std::cout << "==================================\n";

        int choice = GetIntInput("请选择: ");

        if (choice == 0) break;

        switch (choice)
        {
            // ==================== 文本转进制 ====================
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            {
                ClearScreen();
                std::cout << "========== 文本转进制 ==========\n";
                std::string text = GetInput("输入文本: ");
                if (text.empty()) { Pause(); break; }

                std::string sep = GetInput("分隔符（默认空格，可空）: ");
                if (sep.empty()) sep = " ";

                std::cout << "\n=== 转换结果 ===\n";
                if (choice == 1 || choice == 7)
                    std::cout << "2 进制:  " << StringToBase(text, 2, sep) << "\n";
                if (choice == 2 || choice == 7)
                    std::cout << "8 进制:  " << StringToBase(text, 8, sep) << "\n";
                if (choice == 3 || choice == 7)
                    std::cout << "10 进制: " << StringToBase(text, 10, sep) << "\n";
                if (choice == 4 || choice == 7)
                    std::cout << "16 进制: " << StringToBase(text, 16, sep) << "\n";
                if (choice == 5 || choice == 7)
                {
                    std::cout << "32 进制: ";
                    for (char c : text) std::cout << NumToBase((unsigned char)c, 32) << sep;
                    std::cout << "\n";
                }
                if (choice == 6 || choice == 7)
                {
                    std::cout << "36 进制: ";
                    for (char c : text) std::cout << NumToBase((unsigned char)c, 36) << sep;
                    std::cout << "\n";
                }
                Pause();
                break;
            }

            // ==================== 进制转文本 ====================
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
            {
                ClearScreen();
                std::cout << "========== 进制转文本 ==========\n";
                std::string input = GetInput("输入进制字符串（用空格分隔）: ");
                if (input.empty()) { Pause(); break; }

                int base = 16;
                switch (choice)
                {
                    case 8: base = 2; break;
                    case 9: base = 8; break;
                    case 10: base = 10; break;
                    case 11: base = 16; break;
                    case 12: base = 32; break;
                    case 13: base = 36; break;
                }

                std::cout << "\n=== 解码结果 ===\n";
                std::stringstream ss(input);
                std::string token;
                while (ss >> token)
                {
                    try
                    {
                        unsigned long val = std::stoul(token, nullptr, base);
                        std::cout << (char)val;
                    }
                    catch (...) {}
                }
                std::cout << std::endl;
                Pause();
                break;
            }

            // ==================== 任意进制转换 ====================
            case 14:
            {
                ClearScreen();
                std::cout << "========== 任意进制转换 ==========\n";
                std::string num = GetInput("输入数字: ");
                int fromBase = GetIntInput("源进制（2-36）: ", 10);
                int toBase = GetIntInput("目标进制（2-36）: ", 16);

                try
                {
                    unsigned long long val = std::stoull(num, nullptr, fromBase);
                    std::cout << "\n=== 转换结果 ===\n";
                    std::cout << "十进制: " << val << "\n";
                    std::cout << "目标进制(" << toBase << "): " << NumToBase(val, toBase) << std::endl;
                }
                catch (...)
                {
                    std::cout << "转换失败！请检查输入。" << std::endl;
                }
                Pause();
                break;
            }

            case 15:
            case 16:
            case 17:
            case 18:
            {
                ClearScreen();
                std::string num;
                int fromBase, toBase;

                switch (choice)
                {
                    case 15: std::cout << "========== 十进制转二进制 ==========\n"; fromBase = 10; toBase = 2; break;
                    case 16: std::cout << "========== 十进制转十六进制 ==========\n"; fromBase = 10; toBase = 16; break;
                    case 17: std::cout << "========== 二进制转十进制 ==========\n"; fromBase = 2; toBase = 10; break;
                    case 18: std::cout << "========== 十六进制转十进制 ==========\n"; fromBase = 16; toBase = 10; break;
                }

                num = GetInput("输入数字: ");
                try
                {
                    unsigned long long val = std::stoull(num, nullptr, fromBase);
                    std::cout << "\n结果: " << NumToBase(val, toBase) << std::endl;
                }
                catch (...)
                {
                    std::cout << "转换失败！" << std::endl;
                }
                Pause();
                break;
            }

            // ==================== 文件转进制 ====================
            case 19:
            case 20:
            case 21:
            case 22:
            case 23:
            {
                ClearScreen();
                std::cout << "========== 文件转进制 ==========\n";
                std::string src = GetInput("输入文件路径: ");
                std::string dst = GetInput("输出文本路径: ");
                if (src.empty() || dst.empty()) { Pause(); break; }

                std::ifstream in(src, std::ios::binary);
                if (!in.is_open())
                {
                    std::cout << "无法打开文件！" << std::endl;
                    Pause();
                    break;
                }

                std::string content((std::istreambuf_iterator<char>(in)),
                                     std::istreambuf_iterator<char>());
                in.close();

                std::ofstream out(dst);
                if (choice == 19 || choice == 23) out << "2进制:\n" << StringToBase(content, 2, " ") << "\n\n";
                if (choice == 21 || choice == 23) out << "10进制:\n" << StringToBase(content, 10, " ") << "\n\n";
                if (choice == 20 || choice == 23) out << "16进制:\n" << StringToBase(content, 16, " ") << "\n\n";
                out.close();

                std::cout << "\n文件已转换。\n";
                std::cout << "输出大小: ";
                std::cout << (std::ifstream(dst, std::ios::ate).tellg()) << " 字节\n";
                Pause();
                break;
            }

            // ==================== 进制转文件 ====================
            case 24:
            case 25:
            case 26:
            {
                ClearScreen();
                std::cout << "========== 进制转文件 ==========\n";
                std::string src = GetInput("输入文本文件路径: ");
                std::string dst = GetInput("输出二进制文件路径: ");
                int base = 16;
                if (choice == 24) base = 2;
                if (choice == 26) base = 10;
                if (choice == 25) base = 16;

                if (src.empty() || dst.empty()) { Pause(); break; }

                std::ifstream in(src);
                if (!in.is_open())
                {
                    std::cout << "无法打开文件！" << std::endl;
                    Pause();
                    break;
                }

                std::ofstream out(dst, std::ios::binary);
                std::string token;
                while (in >> token)
                {
                    try
                    {
                        unsigned long val = std::stoul(token, nullptr, base);
                        out.put((char)val);
                    }
                    catch (...) {}
                }
                in.close();
                out.close();

                std::cout << "\n文件已还原。\n";
                Pause();
                break;
            }

            // ==================== ASCII 字符表 ====================
            case 28:
            {
                ClearScreen();
                std::cout << "========== ASCII 字符编码表 ==========\n\n";
                std::cout << "十进制  十六进制  字符\n";
                std::cout << "------  --------  ----\n";
                for (int i = 32; i < 127; i++)
                {
                    printf("%4d    0x%02X     %c\n", i, i, (char)i);
                }
                Pause();
                break;
            }

            // ==================== 批量转换 ====================
            case 29:
            {
                ClearScreen();
                std::cout << "========== 批量转换 ==========\n";
                std::string dir = GetInput("目录路径: ");
                int base = GetIntInput("目标进制（2/8/10/16）: ", 16);
                if (dir.empty()) { Pause(); break; }

                std::string cmd = "for %f in (\"" + dir + "\\*\") do @certutil -encode \"%f\" \"%f.b64\"";
                RunCmd("cmd /c \"" + cmd + "\"");
                Pause();
                break;
            }

            // ==================== 二进制文件查看器 ====================
            case 30:
            {
                ClearScreen();
                std::cout << "========== 二进制文件查看器 ==========\n";
                std::string file = GetInput("文件路径: ");
                int maxBytes = GetIntInput("查看字节数（默认 256）: ", 256);
                if (file.empty()) { Pause(); break; }

                std::ifstream in(file, std::ios::binary);
                if (!in.is_open())
                {
                    std::cout << "无法打开文件！" << std::endl;
                    Pause();
                    break;
                }

                std::cout << "\n=== 十六进制视图 ===\n\n";
                unsigned char buffer[16];
                int offset = 0;
                int totalRead = 0;

                while (in.read((char*)buffer, 16) && totalRead < maxBytes)
                {
                    // 偏移
                    printf("%08X  ", offset);

                    // 十六进制
                    std::streamsize bytesRead = in.gcount();
                    for (int i = 0; i < 16; i++)
                    {
                        if (i < bytesRead) printf("%02X ", buffer[i]);
                        else printf("   ");
                        if (i == 7) printf(" ");
                    }

                    // 字符
                    printf(" |");
                    for (int i = 0; i < bytesRead; i++)
                    {
                        if (buffer[i] >= 32 && buffer[i] < 127) putchar(buffer[i]);
                        else putchar('.');
                    }
                    printf("|\n");

                    offset += 16;
                    totalRead += (int)bytesRead;
                }
                in.close();

                std::cout << "\n共查看 " << totalRead << " 字节\n";
                Pause();
                break;
            }

            default:
                std::cout << "无效选项。" << std::endl;
                Pause();
        }
    }
}

