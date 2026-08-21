// 多功能系统工具箱 v2.7
// 编译环境：Visual Studio 2019/2022 或 MinGW-w64，Windows 10/11
// 注意：请将本文件保存为 UTF-8 with BOM 格式，避免中文乱码
// 运行需要管理员权限，部分操作有风险，使用前请阅读免责声明

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <winsock2.h>       // 必须在 windows.h 之前
#include <windows.h>
#include <shellapi.h>
#include <conio.h>
#include <fstream>
#include <chrono>
#include <cmath>
#include <thread>
#include <atomic>
#include <gl/gl.h>

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

// 获取用户输入的整数
int GetIntInput(const std::string& prompt, int defaultVal = -1)
{
    std::string input = GetInput(prompt);
    if (input.empty() && defaultVal != -1)
        return defaultVal;
    try { return std::stoi(input); }
    catch (...) { return -1; }
}

// 功能声明
void CleanDiskC();
void ManageWindowsUpdate();
void DiskRepairMenu();
void USBRepairMenu();
void DetectFakeDisk();
void SystemFileRepair();
void InstallRuntimes();
void OpenSoftwareWebsite();
void ExtractZip();
void ManageHibernate();
void ManageVirtualMemory();
void StressTestMenu();
void NetworkStressTest();
void InterfaceStressTest();
void SecurityCheckMenu();
void CheckScheduledTasks();
void CheckProcesses();
void CheckRegistryAutoRuns();
void FileDigitalSignature();
void DismExtractWim();
void TrustedInstallerMenu();

LRESULT CALLBACK GPUStressWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_CLOSE || msg == WM_DESTROY)
        PostQuitMessage(0);
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int main()
{
    SetUtf8Console();

    std::cout << "========================================\n";
    std::cout << "        多功能系统工具箱 - 免责声明\n";
    std::cout << "========================================\n";
    std::cout << "1. 本工具涉及系统底层操作，部分功能具有风险，可能导致数据丢失或系统损坏。\n";
    std::cout << "2. 使用前请务必备份重要数据，特别是磁盘修复、虚拟内存修改等操作。\n";
    std::cout << "3. 请确保您理解每一步操作的含义，并自行承担所有后果。\n";
    std::cout << "4. 作者不对因使用本工具造成的任何损失负责。\n";
    std::cout << "========================================\n";
    std::cout << "是否同意以上条款并继续？(y/n): ";
    char agree;
    std::cin >> agree;
    std::cin.ignore();
    if (agree != 'y' && agree != 'Y')
    {
        std::cout << "您已拒绝，程序退出。" << std::endl;
        return 0;
    }

    if (!IsRunAsAdmin())
    {
        std::cout << "\n当前未以管理员身份运行，部分功能可能受限。\n";
        std::cout << "是否尝试以管理员身份重新启动？(y/n): ";
        char c;
        std::cin >> c;
        std::cin.ignore();
        if (c == 'y' || c == 'Y')
        {
            if (ElevateSelf())
            {
                std::cout << "已启动管理员实例，当前窗口即将关闭。" << std::endl;
                return 0;
            }
            else
                std::cout << "提升失败，请手动右键以管理员身份运行。" << std::endl;
        }
    }

    int choice;
    while (true)
    {
        ClearScreen();
        std::cout << "========================================\n";
        std::cout << "        多功能系统工具箱 v2.7\n";                                                                                                                                                                    std::cout << "========================================\n";
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
        std::cout << "13. DISM 解压 WIM\n";
        std::cout << "14. TrustedInstaller 特权操作\n";
        std::cout << " 0. 退出程序\n";
        std::cout << "========================================\n";
        std::cout << "请输入选项: ";

        std::cin >> choice;
        std::cin.ignore();

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
        case 13: DismExtractWim(); break;
        case 14: TrustedInstallerMenu(); break;
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
void CleanDiskC()
{
    ClearScreen();
    std::cout << "========== C盘清理 ==========\n";
    std::cout << "将执行以下操作：\n";
    std::cout << " - 清理用户临时文件夹\n";
    std::cout << " - 清理 Windows 临时文件夹\n";
    std::cout << " - 清空回收站\n";
    std::cout << " - 运行磁盘清理工具 (cleanmgr)\n";
    std::cout << "==============================\n";
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

    std::cout << "\n开始清理...\n";
    RunCmd("del /q /f /s \"%TEMP%\\*\" >nul 2>&1", false);
    RunCmd("del /q /f /s \"C:\\Windows\\Temp\\*\" >nul 2>&1", false);
    RunCmd("powershell -Command \"Clear-RecycleBin -Force -ErrorAction SilentlyContinue\"", false);
    RunCmd("cleanmgr /sagerun:1", true);
    std::cout << "C盘清理完成。" << std::endl;
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

// ==================== 5. 运行库安装 ====================
void InstallRuntimes()
{
    ClearScreen();
    std::cout << "========== 运行库安装 ==========\n";
    std::cout << "将使用 winget 安装常用运行库，请确保已安装 App Installer。\n";
    std::cout << "可选安装项：\n";
    std::cout << " 1. .NET 8 桌面运行时\n";
    std::cout << " 2. .NET 6 桌面运行时\n";
    std::cout << " 3. Visual C++ 2015-2022 运行库 (x64)\n";
    std::cout << " 4. 全部安装\n";
    std::cout << " 5. 返回\n";
    std::cout << "=================================\n";
    int choice = GetIntInput("请选择: ");

    if (choice >= 1 && choice <= 4)
    {
        if (system("winget --version >nul 2>&1") != 0)
        { std::cout << "未检测到 winget，请安装 App Installer 或手动下载运行库。\n"; Pause(); return; }
    }

    auto installPackage = [](const std::string& packageId)
    {
        std::string cmd = "winget install " + packageId + " --silent --accept-package-agreements --accept-source-agreements";
        RunCmd(cmd);
    };

    if (choice == 1) installPackage("Microsoft.DotNet.DesktopRuntime.8");
    else if (choice == 2) installPackage("Microsoft.DotNet.DesktopRuntime.6");
    else if (choice == 3) installPackage("Microsoft.VCRedist.2015+.x64");
    else if (choice == 4)
    {
        installPackage("Microsoft.DotNet.DesktopRuntime.8");
        installPackage("Microsoft.DotNet.DesktopRuntime.6");
        installPackage("Microsoft.VCRedist.2015+.x64");
    }
    else std::cout << "操作取消。" << std::endl;
    Pause();
}

// ==================== 6. 打开常用软件官网 ====================
void OpenSoftwareWebsite()
{
    ClearScreen();
    std::cout << "========== 常用软件官网 ==========\n";
    struct Software { const char* name; const char* url; };
    std::vector<Software> softList =
    {
        { "微信", "https://weixin.qq.com/" },
        { "QQ", "https://im.qq.com/" },
        { "钉钉", "https://www.dingtalk.com/" },
        { "谷歌浏览器", "https://www.google.com/chrome/" },
        { "火狐浏览器", "https://www.mozilla.org/firefox/" },
        { "Edge浏览器", "https://www.microsoft.com/edge" },
        { "VLC播放器", "https://www.videolan.org/vlc/" },
        { "7-Zip", "https://www.7-zip.org/" },
        { "Notepad++", "https://notepad-plus-plus.org/" },
        { "Everything", "https://www.voidtools.com/" },
        { "Steam", "https://store.steampowered.com/" },
        { "Epic Games", "https://www.epicgames.com/" },
        { "WPS Office", "https://www.wps.cn/" },
        { "Adobe Reader", "https://get.adobe.com/reader/" },
        { "Python", "https://www.python.org/" },
        { "Java", "https://www.java.com/" },
        { "Visual Studio Code", "https://code.visualstudio.com/" },
        { "Git", "https://git-scm.com/" },
    };

    for (size_t i = 0; i < softList.size(); ++i)
    {
        std::cout << " " << (i + 1) << ". " << softList[i].name << "\n";
    }
    std::cout << " 0. 返回\n";
    std::cout << "==================================\n";
    int choice = GetIntInput("请选择软件编号: ");

    if (choice > 0 && choice <= static_cast<int>(softList.size()))
    {
        ShellExecuteA(NULL, "open", softList[choice - 1].url, NULL, NULL, SW_SHOWNORMAL);
        std::cout << "已在默认浏览器中打开。" << std::endl;
    }
    else if (choice != 0)
    {
        std::cout << "无效选项。" << std::endl;
    }
    Pause();
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
        std::cout << " 4. GPU 压力测试\n";
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
        // ========== 4. GPU 压力测试 ==========
        else if (choice == 4)
        {
            ClearScreen();
            std::cout << "========== GPU 压力测试 ==========\n";
            std::cout << "即将创建一个 OpenGL 渲染窗口，请关闭窗口或按 ESC 结束测试。\n";
            std::cout << "若无法创建窗口，请检查显卡驱动。\n";
            std::cout << "按任意键开始测试..." << std::endl;
            _getch();

            // 注册窗口类
            WNDCLASSW wc = {0};
            wc.lpfnWndProc = GPUStressWndProc;
            wc.hInstance = GetModuleHandle(NULL);
            wc.lpszClassName = L"GPUStressWindow";
            wc.hCursor = LoadCursor(NULL, IDC_ARROW);
            if (!RegisterClassW(&wc))
            {
                std::cout << "窗口类注册失败！" << std::endl;
                Pause();
                continue;
            }

            HWND hwnd = CreateWindowW(L"GPUStressWindow", L"GPU Stress Test", WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, wc.hInstance, NULL);
            if (!hwnd)
            {
                std::cout << "窗口创建失败！" << std::endl;
                Pause();
                continue;
            }

            HDC hdc = GetDC(hwnd);
            PIXELFORMATDESCRIPTOR pfd = { sizeof(pfd), 1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA, 32, 0,0,0,0,0,0,0,0,0,0,0,0,0, 32,0,0, PFD_MAIN_PLANE, 0,0,0,0 };
            int pf = ChoosePixelFormat(hdc, &pfd);
            if (!pf || !SetPixelFormat(hdc, pf, &pfd))
            {
                std::cout << "像素格式设置失败！" << std::endl;
                ReleaseDC(hwnd, hdc);
                DestroyWindow(hwnd);
                Pause();
                continue;
            }

            HGLRC hglrc = wglCreateContext(hdc);
            if (!hglrc)
            {
                std::cout << "OpenGL 上下文创建失败！" << std::endl;
                ReleaseDC(hwnd, hdc);
                DestroyWindow(hwnd);
                Pause();
                continue;
            }
            wglMakeCurrent(hdc, hglrc);
            ShowWindow(hwnd, SW_SHOW);
            UpdateWindow(hwnd);

            MSG msg;
            bool running = true;
            auto start = std::chrono::steady_clock::now();
            while (running)
            {
                while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
                {
                    if (msg.message == WM_QUIT) { running = false; break; }
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
                if (!running) break;

                glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
                glBegin(GL_TRIANGLES);
                for (int i = 0; i < 2000; ++i)
                {
                    float x = (i % 40) / 40.0f * 2.0f - 1.0f;
                    float y = (i / 40) / 50.0f * 2.0f - 1.0f;
                    glColor3f((i % 256) / 255.0f, (i * 3 % 256) / 255.0f, (i * 7 % 256) / 255.0f);
                    glVertex2f(x, y);
                    glVertex2f(x + 0.04f, y);
                    glVertex2f(x + 0.02f, y + 0.08f);
                }
                glEnd();
                SwapBuffers(hdc);

                if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) running = false;
            }
            auto end = std::chrono::steady_clock::now();
            double elapsed = std::chrono::duration<double>(end - start).count();

            wglMakeCurrent(NULL, NULL);
            wglDeleteContext(hglrc);
            ReleaseDC(hwnd, hdc);
            DestroyWindow(hwnd);

            std::cout << "GPU 压力测试结束，持续时间: " << elapsed << " 秒。" << std::endl;
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
    std::cout << "本功能使用 PowerShell 的 Set-AuthenticodeSignature 为文件签名。\n";
    std::cout << "支持使用 PFX 证书文件或系统证书存储中的代码签名证书。\n";
    std::cout << "注意：签名需要有效的代码签名证书，否则签名无效。\n";
    std::cout << "=====================================\n";

    std::string filePath = GetInput("请输入要签名的文件完整路径: ");
    if (filePath.empty()) { std::cout << "路径不能为空！" << std::endl; Pause(); return; }

    DWORD attr = GetFileAttributesA(filePath.c_str());
    if (attr == INVALID_FILE_ATTRIBUTES) { std::cout << "文件不存在！" << std::endl; Pause(); return; }

    std::cout << "\n请选择证书来源：\n";
    std::cout << " 1. 使用 PFX 证书文件\n";
    std::cout << " 2. 使用系统证书存储中的代码签名证书\n";
    std::cout << " 3. 返回\n";
    int choice = GetIntInput("请选择: ");

    if (choice == 1)
    {
        std::string pfxPath = GetInput("请输入 PFX 证书文件路径: ");
        if (pfxPath.empty()) { std::cout << "路径不能为空！" << std::endl; Pause(); return; }
        std::string password = GetInput("请输入证书密码（没有密码直接回车）: ");

        std::string psCmd = "powershell -Command \""
            "$cert = New-Object System.Security.Cryptography.X509Certificates.X509Certificate2('" + pfxPath + "', '" + password + "');"
            "Set-AuthenticodeSignature -FilePath '" + filePath + "' -Certificate $cert -HashAlgorithm SHA256;"
            "Write-Host '签名完成，请验证。'"
            "\"";
        RunCmd(psCmd);
    }
    else if (choice == 2)
    {
        std::string certSubject = GetInput("请输入证书主题名（如 'My Code Signing Cert'）: ");
        if (certSubject.empty()) { std::cout << "主题名不能为空！" << std::endl; Pause(); return; }

        std::string psCmd = "powershell -Command \""
            "$cert = Get-ChildItem Cert:\\CurrentUser\\My -CodeSigningCert | Where-Object { $_.Subject -like '*" + certSubject + "*' } | Select-Object -First 1;"
            "if ($cert) { Set-AuthenticodeSignature -FilePath '" + filePath + "' -Certificate $cert -HashAlgorithm SHA256; Write-Host '签名完成，请验证。' }"
            "else { Write-Host '未找到匹配的证书！' }"
            "\"";
        RunCmd(psCmd);
    }
    else
        std::cout << "操作取消。" << std::endl;
    Pause();
}

// ==================== 13. DISM 解压 WIM ====================
void DismExtractWim()
{
    ClearScreen();
    std::cout << "========== DISM 解压 WIM ==========\n";
    std::string wimPath = GetInput("请输入 WIM 文件完整路径: ");
    if (wimPath.empty()) { std::cout << "路径不能为空！" << std::endl; Pause(); return; }
    std::string destDir = GetInput("请输入解压目标文件夹路径: ");
    if (destDir.empty()) { std::cout << "目标路径不能为空！" << std::endl; Pause(); return; }

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
    std::string cmd = "\"" + psexec + "\" -accepteula -s " + command + " 2>&1";
    
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
    
    if (buffer.str().empty())
    {
        return "命令执行完成，无输出";
    }
    
    return buffer.str();
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
    
    // 使用 CreateProcess 直接启动，绕过 system()
    std::string cmdLine = "\"" + psexec + "\" -accepteula -s -i \"" + cleanPath + "\"";
    if (!args.empty())
    {
        cmdLine += " " + args;
    }
    
    std::cout << "执行命令: " << cmdLine << std::endl;
    
    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    
    BOOL result = CreateProcessA(
        NULL,
        (LPSTR)cmdLine.c_str(),
        NULL, NULL, FALSE,
        0, NULL, NULL,
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

// 以 TrustedInstaller 权限运行 PowerShell 命令
bool RunPowerShellAsTrustedInstaller(const std::string& psCommand)
{
    if (!CheckPsExec()) return false;
    
    std::string psexec = GetPsExecPath();
    std::string cmd = "\"" + psexec + "\" -accepteula -s powershell -Command \"" + psCommand + "\"";
    
    std::cout << "执行命令: " << cmd << std::endl;
    int ret = system(cmd.c_str());
    return ret == 0;
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

// 以 TrustedInstaller 权限删除目录
bool RemoveDirectoryAsTrustedInstaller(const std::string& dirPath)
{
    if (!CheckPsExec()) return false;
    
    std::string psexec = GetPsExecPath();
    std::string cmd = "\"" + psexec + "\" -accepteula -s cmd /c \"rmdir /s /q \\\"" + dirPath + "\\\"\"";
    
    std::cout << "执行命令: " << cmd << std::endl;
    int ret = system(cmd.c_str());
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
                if (RunPowerShellAsTrustedInstaller(psCmd))
                    std::cout << "PowerShell 命令执行成功！" << std::endl;
                else
                    std::cout << "PowerShell 命令执行失败！" << std::endl;
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