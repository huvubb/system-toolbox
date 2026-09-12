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
g++ -o system-toolbox.exe system-toolbox.cpp -std=c++17 -fexec-charset=UTF-8 -finput-charset=UTF-8 -lws2_32 -lshell32 -ladvapi32 -lopengl32 -lgdi32 -luser32 -pthread
