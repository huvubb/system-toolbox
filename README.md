# 🧰 多功能系统工具箱

**System Toolbox** —— 免费、开源、全功能的 Windows 系统维护工具

[![C++](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)
[![Windows](https://img.shields.io/badge/Windows-10%2F11-0078D6.svg)](https://www.microsoft.com/windows)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![Version](https://img.shields.io/badge/Version-1.0.1-orange.svg)](https://github.com/huvubb/system-toolbox/releases)

---

## 📌 简介

专治各种收费系统维护工具割韭菜。

本工具箱面向 **Windows 高级用户、系统管理员与开发者**，提供近 **20 个大类** 的系统维护功能，全部免费、无广告、无需联网（除部分下载功能外）。

---

## ✨ 功能一览

| 编号 | 功能模块 | 说明 |
|------|----------|------|
| 1 | C盘清理 | 清理临时文件、回收站，调用磁盘清理工具 |
| 2 | Windows 更新管理 | 一键禁用/启用系统更新 |
| 3 | 硬盘修复 | chkdsk 修复、U盘专修、扩容盘检测 |
| 4 | 系统文件修复 | sfc /scannow + DISM 在线修复 |
| 5 | 运行库安装 | 通过 winget 安装 .NET / VC++ 运行库 |
| 6 | 常用软件官网 | 内置 200+ 软件官网，支持搜索与分页 |
| 7 | ZIP 解压 | 通过 PowerShell 解压 ZIP 压缩包 |
| 8 | 休眠管理 | 开启/关闭休眠与快速启动 |
| 9 | 虚拟内存管理 | 自动或自定义设置分页文件 |
| 10 | 硬件压力测试 | 硬盘 / 内存 / CPU / GPU / 网络 / 接口 |
| 11 | 安全检测 | 计划任务 / 进程 / 注册表自启动探测 |
| 12 | 文件数字签名 | 生成自签名证书并对文件签名 |
| 13 | DISM WIM 工具 | 解压 / 打包 / 挂载 / 索引管理 |
| 14 | TrustedInstaller 特权 | 通过 PsExec 以 SYSTEM 权限执行操作 |
| 15 | 系统时间同步 | 强制同步 time.windows.com |
| 16 | 存储单位换算 | Byte ~ YB 全单位精确换算 |
| 17 | NTDLL 底层 API | 系统版本 / 进程 / 时间查询及文件操作 |
| 18 | 镜像应用 | ESD/WIM 互转、ISO 信息查看、镜像部署 |
| 19 | NPM 工具安装 | 一键安装 AI 工具 / 构建工具 / 框架 / 数据库客户端 |

---

## 🚀 快速开始

### 方式一：直接下载 Release（推荐）

前往 [Releases](https://github.com/huvubb/system-toolbox/releases) 下载最新 `system-toolbox.exe`

> **注意**：运行需要管理员权限，部分功能（如 TrustedInstaller）需要同目录下放置 [PsExec64.exe](https://live.sysinternals.com/PsExec64.exe)

### 方式二：从源码编译

#### 编译环境
- Visual Studio 2019/2022 或 MinGW-w64
- Windows 10/11 SDK

#### MinGW-w64 编译命令
```bash
g++ -o system-toolbox.exe system-toolbox.cpp -std=c++17 -O2 -fexec-charset=UTF-8 -finput-charset=UTF-8 -lws2_32 -lshell32 -ladvapi32 -lole32 -lcrypt32 -lopengl32 -ld3d11 -ldxgi -lgdi32 -luser32 -pthread -static
```

在项目目录内运行，输出文件使用相对路径；部分 MinGW 链接器不支持包含中文的绝对输出路径。

#### 使用 CMake 构建并测试

需要 CMake 3.16 或更新版本。Visual Studio 开发者终端中执行：

```powershell
cmake -S . -B build
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

MinGW-w64 配合 Ninja：

```powershell
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
```

编译器须已加入 PATH。切换编译器时使用新的构建目录。MSVC 可执行文件位于 `build/Release/`，MinGW 位于 `build/`。MinGW 构建静态链接运行库，方便复制到其他 Windows 电脑。

回归测试覆盖整数输入、SHA256、中文路径、更新状态、文件进制及 Base64 往返转换、无效输入保留目标文件、十六进制查看边界。测试不执行磁盘修复、注册表修改或硬件压力测试。GitHub Actions 会使用 MSVC 和 MinGW 分别构建并运行同一套测试。
