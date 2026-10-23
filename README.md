# 🧰 System Toolbox

Windows 10/11 系统维护工具箱

System Toolbox 是一款面向 Windows 10/11 的开源系统维护工具箱，整合系统清理、软件安装、下载管理、磁盘维护、修复工具、音频处理和后台任务控制等功能，帮助开发者和高级用户在一个入口中完成常见维护任务。

[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-Windows%2010%2F11-0078D6.svg)](https://www.microsoft.com/windows)
[![C++](https://img.shields.io/badge/C%2B%2B-17%2F20-blue.svg)](https://isocpp.org/)

> 适合：开发者、系统管理员、高级 Windows 用户、需要统一管理系统维护工具的人。

## 核心能力

- C 盘清理与系统垃圾处理
- Windows 更新控制
- 硬盘检查、修复与分区管理
- SFC / DISM 系统修复
- 常用软件安装与运行库管理
- ZIP / ISO / VHD / 镜像相关处理
- 音乐批量转 MP3
- fastdl 高速下载
- 后台任务队列，减少长任务打断
- DiskTool DLC 集成与启动盘制作

## 为什么值得用

很多 Windows 系统工具看起来是零散的小功能，但真正日常维护时，问题往往是：

- 工具分散在不同地方
- 操作步骤复杂
- 容易忘记执行前提条件
- 同一类功能需要反复切换

System Toolbox 把这些常见动作统一到一个入口中，减少重复操作，提高效率。

## 快速开始

### 直接运行 Release

下载完整包后，解压并运行：

```powershell
system-toolbox.exe
```

> 部分功能需要管理员权限，建议以管理员身份运行。

### 通过 npm 安装

```bash
npm install system-toolbox
npx system-toolbox
```

全局安装：

```bash
npm install -g system-toolbox
system-toolbox
```

### 源码编译

环境要求：

- Windows 10 / 11
- C++17 / C++20
- Visual Studio 2019/2022 或 MinGW-w64
- Windows SDK

示例编译：

```bash
g++ -o system-toolbox.exe system-toolbox.cpp -std=c++17 -O2 -fexec-charset=UTF-8 -finput-charset=UTF-8 -lws2_32 -lshell32 -ladvapi32 -lole32 -lcrypt32 -lopengl32 -ld3d11 -ldxgi -lgdi32 -luser32 -lcomctl32
```

也支持使用 CMake：

```powershell
cmake -S . -B build
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

## 主要目录

```text
.
├── src/                  # 主程序源代码
├── music-converter/      # 音乐转换模块
├── fastdl-integration/   # 下载器集成代码
├── npm-package/          # npm 包封装
├── README.md             # 项目说明
├── LICENSE               # MIT 许可证
├── CMakeLists.txt        # 构建脚本
└── ...
```

## 适用场景

- 需要清理 Windows 系统垃圾
- 需要统一管理安装与修复工具
- 需要批量转换音乐文件
- 需要处理 ISO / VHD / 磁盘相关工作
- 需要在一个入口内执行多类系统维护任务

## 免责声明

本项目为开源工具，用于系统维护、诊断与自用场景。

部分功能会修改系统状态，例如：

- 磁盘操作
- 启动盘制作
- 镜像挂载
- Windows 修复
- 更新控制

使用前请确认你了解风险，并在熟悉环境下操作。

## 贡献

欢迎提交 Issue、PR 或建议，尤其欢迎：

- 功能改进
- 兼容性修复
- 文档优化
- 翻译补充

## 联系方式

- GitHub: https://github.com/huvubb/system-toolbox
- Email: 810372789@qq.com

## 许可证

MIT License
