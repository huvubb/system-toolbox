# 🧰 System Toolbox

[简体中文](README.md) · **English**

[![NoLogin Verified](https://nologin.tools/badges/flat-blue.svg)](https://nologin.tools/badge/ "Verified by NoLoginTools.org")
[![C++](https://img.shields.io/badge/C++-17%2F20-blue.svg)](https://isocpp.org/)
[![Windows](https://img.shields.io/badge/Windows-10%2F11-0078D6.svg)](https://www.microsoft.com/windows)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![Release](https://img.shields.io/badge/Release-v1.9-orange.svg)](https://github.com/huvubb/system-toolbox/releases)

**System Toolbox** is a free, open-source maintenance toolbox for **Windows 10/11** that consolidates system maintenance, software installation, fast downloads, audio conversion, and disk management into a single console. It is written in C++17/20, released under the MIT license, and is fully portable — extract and run.

[⬇️ Download v1.9 (complete ZIP)](https://github.com/huvubb/system-toolbox/releases/download/v1.9/system-toolbox-windows.zip) · [Release notes & checksums](https://github.com/huvubb/system-toolbox/releases/tag/v1.9) · [Website](https://huvubb.github.io/system-toolbox/)

---

## ✨ What's new in v1.9

- **Background task queue** — independent commands such as `winget` installs run in the background, with adjustable concurrency (1–8), priority, pause/resume, and per-task logs, so you can keep working while tasks run.
- **PATH refresh after installs** — picks up Python, Node.js/npm, Java, Go, Rust, Git, CMake, FFmpeg and other common tools without restarting the console.
- Integrated **fastdl** multi-threaded resumable downloads, **audio-to-MP3 conversion**, an installable **DiskTool** add-on, and image/volume mounting.
- Update checks now compare the executable's **SHA-256 hash**; a checksum mismatch aborts the update.
- Fixed a bug where configuring automatic disk check for one volume could affect other volumes.

---

## 📌 Why System Toolbox

Built for **Windows power users, system administrators, and developers** who want one fast, keyboard-driven entry point instead of hunting through a dozen settings pages and utilities.

- **One console, many tools** — cleaning, repair, runtimes, downloads, audio and disk tools in one place.
- **Background queues** — kick off installs and jobs and keep using the menu.
- **Portable & transparent** — no installer, no telemetry; the complete source is one auditable C++ codebase.
- **Integrity first** — releases ship with a `SHA256SUMS.txt` and updates are hash-verified before replacement.

---

## 🧩 Feature overview

| # | Module | Description |
|---|--------|-------------|
| 1 | C-drive cleanup | Remove temp files, empty the Recycle Bin, invoke the built-in Disk Cleanup tool |
| 2 | Windows Update control | Disable / enable Windows Update in one step |
| 3 | Disk repair | `chkdsk` repair, USB-drive repair, fake-capacity (expanded) drive detection |
| 4 | System file repair | `sfc /scannow` + online DISM repair |
| 5 | Runtime installation | Install .NET / VC++ runtimes via winget |
| 6 | Software directory | Built-in searchable, paginated list of 200+ official software websites |
| 7 | ZIP extraction | Extract ZIP archives via PowerShell |
| 8 | Hibernation control | Toggle hibernation and Fast Startup |
| 9 | Virtual memory | Automatic or custom page-file configuration |
| 10 | Hardware stress tests | Disk / memory / CPU / GPU / network / interface |
| 11 | Security inspection | Scheduled tasks, processes, and registry auto-start detection |
| 12 | Digital signatures | Generate a self-signed certificate and sign files |
| 13 | DISM / WIM tools | Extract / capture / mount / index management |
| 14 | TrustedInstaller privileges | Run operations as SYSTEM via PsExec |
| 15 | Time sync | Force sync with time.windows.com |
| 16 | Storage unit converter | Precise conversion from Byte to YB |
| 17 | Low-level NTDLL API | OS version / process / time queries and file operations |
| 18 | Imaging | ESD/WIM conversion, ISO inspection, image deployment |
| 19 | NPM tool installer | One-command install of AI tools, build tools, frameworks, DB clients |

Plus: a **background task queue** (menu 31), **fastdl downloads** (menu 32), the **DiskTool add-on** (menu 33), and **image/volume mounting** (menu 34).

---

## 🚀 Quick start

### Option 1 — Download a release (recommended)

1. Download the [v1.9 complete ZIP](https://github.com/huvubb/system-toolbox/releases/download/v1.9/system-toolbox-windows.zip).
2. Extract it and run `system-toolbox.exe`. Keep `music-converter.exe` and `fastdl.exe` in the same folder.
3. Audio conversion additionally needs FFmpeg, which you can install from inside the toolbox.

Each release includes `SHA256SUMS.txt`. Verify the main executable with:

```powershell
Get-FileHash .\system-toolbox.exe -Algorithm SHA256
```

> **Note:** running the toolbox requires administrator privileges. Some features (e.g. TrustedInstaller) need [PsExec64.exe](https://live.sysinternals.com/PsExec64.exe) placed next to the executable.

### Option 2 — Build from source

Requirements: Visual Studio 2019/2022 (or MinGW-w64) and the Windows 10/11 SDK.

```bash
g++ -o system-toolbox.exe system-toolbox.cpp -std=c++17 -O2 -fexec-charset=UTF-8 -finput-charset=UTF-8 -lws2_32 -lshell32 -ladvapi32 -lole32 -lcrypt32 -lopengl32 -ld3d11 -ldxgi -lgdi32 -luser32 -pthread -static
```

Or with CMake:

```powershell
cmake -S . -B build
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

CI builds and runs the same test suite with both MSVC and MinGW. The MinGW build statically links the runtime for easy copying between machines.

---

## ⚙️ Background queue

Menu 31 enables background submission. Independent system commands, ordinary FFmpeg jobs, and multi-format audio conversions then enter a queue while the menu stays usable. Defaults to 2 concurrent tasks (configurable 1–8), with low/normal/high priority, pause/resume, and separate logs. Queued tasks are scheduled by priority and submission order; running tasks and their child processes are controlled together. Interactive commands and pipelines that depend on intermediate files still run in the foreground. Quitting the toolbox ends unfinished tasks.

## ⚡ fastdl downloads

Menu 32 offers file downloads and the full fastdl menu (including ed2k). Network downloads, PsExec downloads, and update downloads all use fastdl. Downloads are multi-threaded and resumable, and a failure never overwrites an existing target file. The complete package ships `fastdl.exe` next to the toolbox.

## 🎵 Audio conversion

Menu 29 (under Audio/Video tools) converts a single file or a whole directory to MP3, supporting common Chinese streaming-service containers such as NetEase NCM, QQ Music QMC/MFLAC/MGG, KuGou KGM/KGMA/VPR, and KuWo KWM. Some QQ Music files require the corresponding EKey/key library, and a few formats (e.g. KGG v5) are not supported. FFmpeg is required. Hardware acceleration is attempted first and falls back to software decoding.

> Please convert only audio you have legitimately acquired, for personal backup and compatibility. Encrypted container formats vary by vendor and version, so not every file is guaranteed to convert.

## 💽 DiskTool add-on, mounting & disk check

Menu **35** creates bootable media from a Windows Setup/WinPE ISO. It erases the selected physical disk, creates an active MBR/FAT32 partition of up to 30 GiB, copies setup and EFI boot files, and splits `sources/install.wim` when it exceeds FAT32's file limit. Optional BIOS support uses Microsoft-signed `bootsect.exe`. Target disks must be between 4 GiB and 2 TiB. Remaining disk space stays unallocated. This feature does not burn optical discs or write Linux ISOs.

The tool refuses system, boot, page-file, ISO, application and temporary-script disks, displays disk identity and partitions, and requires an exact confirmation containing the disk's unique ID. It rechecks identity immediately before erasing and runs in the foreground after background tasks finish. Back up first: erasing cannot be rolled back. Actual boot compatibility depends on the ISO architecture and target firmware.

Menu 33 installs the DiskTool add-on from a user-provided URL or a local build, verifying its recorded SHA-256 both at install and at launch. Add-ons live in `%LOCALAPPDATA%\SystemToolbox\dlc`. Menu 34 lists volumes and mount points, mounts/unmounts ISO/VHD/VHDX, and mounts volumes to empty folders or free drive letters (images default to read-only). Menu 30 controls the automatic-boot-check setting for a selected volume only, preserving other volumes and third-party boot entries, and backs up the original configuration first.

Runtime records and configuration backups are stored in `%LOCALAPPDATA%\SystemToolbox`.

---

## 🛡️ Safety

- Disk writes and repairs can affect data — **back up important files first**.
- System maintenance features require administrator privileges; understand an option before running it.
- Downloads, software installation, and online repair require an internet connection.
- Upstream components and their licenses are documented in `music-converter/UPSTREAM.md` and `fastdl-integration/UPSTREAM.md`.

## 🤝 Contributing

Feedback and real-world bug reports are welcome. Please open an [issue](https://github.com/huvubb/system-toolbox/issues) with steps to reproduce, or start a [discussion](https://github.com/huvubb/system-toolbox/discussions). The regression tests cover integer input, SHA-256, Unicode paths, update status, unit/base64 round-trips, and hex-view boundaries; they deliberately do not run disk repair, registry changes, or hardware stress tests.

## 📄 License

Released under the [MIT License](LICENSE). Bundled third-party components (`fastdl`, `music-converter`, PsExec) retain their respective upstream licenses.

Contact: **810372789@qq.com**
