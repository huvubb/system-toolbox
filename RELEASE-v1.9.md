# v1.9 · 后台队列、下载与磁盘工具更新

请下载 **system-toolbox-windows.zip**，解压后运行 `system-toolbox.exe`，保留同目录的 `music-converter.exe` 和 `fastdl.exe`。`SHA256SUMS.txt` 包含各文件 SHA256，便于核验。

## 新增与修复

- 后台任务队列：默认并发 2，可设 1–8，支持优先级、暂停、继续和独立日志。winget 等独立命令可后台运行。
- 安装后刷新 PATH，检查 Python、Node.js/npm、Java、Go、Rust、Git、CMake、FFmpeg 等常见命令目录。
- 集成 fastdl 多线程与断点下载，失败保留原目标文件。
- 音乐转换支持 NCM、QMC/MFLAC/MGG、KGM/KGMA/VPR、KWM 等常见版本。
- 根据本地 EXE 的 SHA256 检查云端更新，校验后替换并保留备份。
- DiskTool 按需下载、导入和启动，安装与运行时校验已记录构建哈希。
- 增加 ISO/VHD/VHDX、卷目录与盘符挂载管理。
- 修复指定卷开机检查影响其他卷：保留其他卷配置，不再全局终止磁盘检查进程或清空启动检查项。
- 修复 C++20 `char8_t` 路径编译问题；主 CPP 包含队列实现。

## 使用边界

- 音乐转换需要 FFmpeg；部分 QQ 音乐文件需要 EKey，KGG v5 等格式尚未支持，无法保证所有平台全部文件兼容。硬件解码尝试失败会回退软件解码，常见音频通常仍使用 CPU。
- 后台任务无需交互输入；依赖中间结果的流水线仍在前台。退出工具箱会结束未完成任务。
- DiskTool 的恢复工具与数据库可能需要额外配套资源；PsExec 不随包分发。
- 系统维护需要管理员权限，磁盘写入与修复前应备份数据。

源码、详细说明与问题反馈：https://github.com/huvubb/system-toolbox

联系邮箱：810372789@qq.com
