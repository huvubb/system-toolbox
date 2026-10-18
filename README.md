# 🧰 多功能系统工具箱

**简体中文** · [English](README.en.md)

[![NoLogin Verified](https://nologin.tools/badges/flat-blue.svg)](https://nologin.tools/badge/ "Verified by NoLoginTools.org")

**System Toolbox** —— 免费开源的 Windows 10/11 系统维护工具箱，集成后台任务队列、高速下载、音乐转码和磁盘工具。

[下载 v1.9 完整包](https://github.com/huvubb/system-toolbox/releases/download/v1.9/system-toolbox-windows.zip) · [更新说明与校验清单](https://github.com/huvubb/system-toolbox/releases/tag/v1.9) · [项目介绍](https://huvubb.github.io/system-toolbox/)

> 🇨🇳 **国内直链（免代理、更快）**：[在线介绍页](https://q23213ddssd.dpdns.org/system-toolbox/) · [ZIP 直接下载](https://q23213ddssd.dpdns.org/downloads/system-toolbox-windows.zip) · [SHA256 校验清单](https://q23213ddssd.dpdns.org/downloads/system-toolbox-SHA256SUMS.txt)（与 GitHub Release 同一文件，可用哈希核对）

Open-source Windows maintenance toolbox with background queues, resumable downloads, audio conversion and volume mounts. Built with C++17/20.

### v1.9 新增

- 后台队列：winget 等独立命令可后台运行，调整优先级、暂停和继续。
- 安装后刷新 PATH，覆盖 Python、Node.js/npm、Java、Go、Rust 等常见工具。
- 集成 fastdl、多种音乐文件转 MP3、DiskTool 可下载 DLC，以及镜像和卷挂载。
- 根据 EXE 的 SHA256 检查更新；修复指定磁盘的开机检查设置影响其他磁盘的问题。

音乐格式随厂商版本变化，部分文件需要密钥，不能保证所有平台的全部文件可转换。详见下方使用说明。

[![C++](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)
[![Windows](https://img.shields.io/badge/Windows-10%2F11-0078D6.svg)](https://www.microsoft.com/windows)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![Build](https://img.shields.io/badge/Build-SHA256-orange.svg)](https://github.com/huvubb/system-toolbox/releases)

---

## 📌 简介

本工具箱面向 **Windows 高级用户、系统管理员与开发者**，集中提供系统维护、安装、下载、音视频处理与磁盘管理入口。下载、软件安装和在线修复需要联网；系统操作请先了解选项影响。

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

下载 [v1.9 完整 ZIP](https://github.com/huvubb/system-toolbox/releases/download/v1.9/system-toolbox-windows.zip)，解压后运行 `system-toolbox.exe`。请保留同目录的 `music-converter.exe` 和 `fastdl.exe`。音乐转码另需 FFmpeg，可在工具箱内安装。

发行页附有 `SHA256SUMS.txt`，可用 PowerShell 的 `Get-FileHash .\system-toolbox.exe -Algorithm SHA256` 对照主程序哈希。

> **注意**：运行需要管理员权限，部分功能（如 TrustedInstaller）需要同目录下放置 [PsExec64.exe](https://live.sysinternals.com/PsExec64.exe)

### 方式二：通过 npm 安装

Windows x64、Node.js 18+：执行 `npm install system-toolbox`，然后用 `npx system-toolbox` 启动。需要全局命令时使用 `npm install -g system-toolbox`，之后可直接运行 `system-toolbox`。从 npm 包 1.9.1 起，三个 EXE 随包提供，安装时无需另从 GitHub 或镜像下载；系统维护在启动工具箱并确认管理员权限后执行。

### 方式三：从源码编译

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

### 音乐文件转 MP3

在「音视频工具」中选择 29，可转换单个文件或批量转换目录：支持网易云 NCM、QQ 音乐 QMC/MFLAC/MGG、酷狗 KGM/KGMA/VPR、酷我 KWM 等格式。部分 QQ 音乐文件需提供对应 EKey 或密钥库；酷狗 KGG v5 等未支持的格式会提示失败。不同版本的加密格式不保证全部兼容。转码需要 FFmpeg，已存在的输出会使用新文件名。

完整 Windows 构建包包含 `music-converter.exe`，需与 `system-toolbox.exe` 放在同一目录。源码构建时，安装 Python 3.10+ 后执行 `python -m pip install -r music-converter/requirements.txt`；CMake 会将转换脚本复制到程序目录。该组件来源及许可证见 [music-converter/UPSTREAM.md](music-converter/UPSTREAM.md)。

工具箱通过 winget 安装软件成功后会刷新当前进程的 PATH；安装 Python 时启用安装器的 PATH 选项，安装 Node.js 时会将 npm 全局目录加入用户 PATH。对 Java、Go、Rust、Git、CMake、Make、MSYS2、FFmpeg、文档转换工具等也会检查已知安装目录，仅将实际存在的命令目录补入用户 PATH；其他安装目录使用安装器注册的 PATH。Visual Studio 编译器仍需开发者终端初始化 SDK 环境。

编译器须已加入 PATH。切换编译器时使用新的构建目录。MSVC 可执行文件位于 `build/Release/`，MinGW 位于 `build/`。MinGW 构建静态链接运行库，方便复制到其他 Windows 电脑。

回归测试覆盖整数输入、SHA256、中文路径、更新状态、文件进制及 Base64 往返转换、无效输入保留目标文件、十六进制查看边界。测试不执行磁盘修复、注册表修改或硬件压力测试。GitHub Actions 会使用 MSVC 和 MinGW 分别构建并运行同一套测试。

主 CPP 现已包含队列实现，不再需要额外的 `task-queue.h`。支持 C++17 和 C++20；可用 `-DCMAKE_CXX_STANDARD=20` 配置 CMake。文件路径统一转换为 UTF-8 字节字符串，兼容 C++20 的 `char8_t`。

音乐转码会先请求 FFmpeg 自动选择硬件加速，失败后回退软件解码；常见音频解码器通常仍在 CPU 上运行，不能据此声称已使用 GPU。

### 后台队列和版本检查

主菜单 31 可开启后台提交；后续独立系统命令、普通 FFmpeg 操作、多平台音乐转换会进入队列，菜单可继续操作。默认并发 2，支持 1–8 个并发任务、低/普通/高优先级、暂停/继续及独立日志。等待任务按优先级和提交顺序调度，运行任务及其子进程同步控制。依赖中间文件的智能流水线、内置计算、交互式命令仍在前台运行。后台任务没有交互输入，请只提交无需输入的操作；退出工具箱会结束未完成任务，暂停安装或系统维护可能持续占用资源。

程序标识自动来自本地 EXE 的 SHA256，无需在源码中维护版本号。更新检查获取 GitHub 最新发布：若附件含 `SHA256SUMS.txt`，先比较其中的程序哈希；旧发布没有清单时，下载 ZIP 后计算其中唯一的 `system-toolbox.exe` 哈希。相同则无需更新，不同则显示发布说明并询问是否替换；哈希只能判断文件是否相同，无法判断新旧。清单与下载内容不符时拒绝安装。启动记录也改为保存程序哈希，旧版数字记录会自动迁移。

### fastdl 高速下载

已集成 `D:\fastdl` 的源码，来源见 [fastdl-integration/UPSTREAM.md](fastdl-integration/UPSTREAM.md)。主菜单 32 提供文件下载及完整 fastdl 菜单（含 ed2k）；网络工具的下载、PsExec 下载和更新附件下载也使用 fastdl。普通文件下载可按主菜单 31 的设置进入后台队列，更新下载保持前台等待，以完成哈希校验。下载支持多线程和断点数据，失败不会覆盖已有目标文件。

完整包包含 `fastdl.exe`，放在工具箱旁；源码运行需安装 `fastdl-integration/requirements.txt`。构建流程自动生成 `system-toolbox-windows.zip` 和 `SHA256SUMS.txt`，发布时将两者附到同一 GitHub Release 即可。更新安装会同步复制附件中的音乐转换器和 fastdl，保留旧文件备份；有后台任务时先完成任务再安装更新。

### DiskTool DLC、挂载与磁盘检查

主菜单 **35** 可将 Windows 安装/WinPE ISO 制作为磁盘启动介质：清除所选物理磁盘，创建最大 30GB 的 MBR/FAT32 活动分区，复制安装文件与 EFI 启动文件；超过 4GB 的 `sources/install.wim` 自动分卷。可选 UEFI+BIOS 模式会使用通过微软签名校验的 `bootsect.exe` 写入目标卷启动代码。需要管理员权限，目标盘容量须为 4GB–2TB；ISO 必须包含 Windows/WinPE 启动文件，本功能不支持 Linux ISO 或光盘刻录。大于 30GB 的剩余磁盘空间保持未分配。

制作启动盘会清除目标盘所有分区和数据，不能自动回滚。程序显示编号、型号、序列号、容量与分区，并要求输入包含磁盘唯一标识的确认文本；拦截系统、启动、分页文件、ISO、工具箱和临时脚本所在盘，执行前再次校验磁盘身份。制作流程固定在前台，勿在写入期间拔盘或关闭程序；首次使用前请备份。完成后应安全弹出并通过固件启动菜单验证，UEFI/BIOS 是否可启动仍取决于镜像架构与目标固件。

主菜单 33 可从用户提供的 [DiskTool 下载地址](https://q23213ddssd.dpdns.org/downloads/disktool.exe) 安装 DLC，也可导入 `D:\disktool\build\disktool.exe`。安装和启动时核验已记录构建的 SHA256；校验失败保留原 DLC。EXE 以子进程共享控制台运行，退出后返回主菜单。DLC 存放在 `%LOCALAPPDATA%\SystemToolbox\dlc`，无需放在主程序旁；涉及额外恢复工具或数据库的功能仍需 DiskTool 的配套资源。

主菜单 34 支持列出卷和挂载点、挂载/卸载 ISO/VHD/VHDX、将卷挂载到空目录或空闲盘符、移除单个挂载点。镜像默认只读，VHD/VHDX 可选择读写；不允许移除系统卷或工具箱所在卷的根挂载点。

主菜单 30 仅控制指定卷的开机自动检查：合并既有排除项，恢复时只移除所选卷的排除项，保留其他卷及第三方启动项，并先保存原配置。不会终止全部 chkdsk 进程、清空 BootExecute、修改 WinRE 或全局 NTFS 自愈设置。旧版清空的启动检查配置可通过菜单 30 → 4 显式恢复 Windows 默认入口，同时仅排除所选卷；原值无法推断，非空的未知配置会拒绝覆盖。

运行记录和配置备份存放在 `%LOCALAPPDATA%\SystemToolbox`。安装目录仅需 EXE、主 CPP 和 Markdown 文档；许可证文件使用 `.md` 扩展名。
