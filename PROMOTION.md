# v1.9 分享文案

## 中文社区

标题：开源 Windows 系统工具箱 v1.9：后台安装、多队列、下载和磁盘工具

我的 Windows 系统工具箱更新到 v1.9 了，免费开源。这次加入了后台任务队列：winget 等独立命令可以后台执行，支持并发、优先级、暂停和继续；安装常用开发工具后会刷新 PATH。

另外集成了 fastdl 下载器、多种音乐文件转 MP3、可下载的 DiskTool DLC，以及镜像和卷挂载。修复了指定卷的开机磁盘检查设置影响其他卷的问题，更新检查改为比较程序 SHA256。

下载完整 ZIP 解压运行，保留两个辅助 EXE；音乐转换另需 FFmpeg。部分厂商格式需要密钥或暂不兼容，磁盘修复前请备份。

项目：https://github.com/huvubb/system-toolbox

下载：https://github.com/huvubb/system-toolbox/releases/tag/v1.9

欢迎反馈真实使用问题，也欢迎提交改进。

## English announcement

System Toolbox v1.9 is out: an open-source Windows 10/11 maintenance toolbox with background task queues, winget installation and PATH refresh, fastdl downloads, audio conversion, optional DiskTool integration and volume/image mounts. Update checks compare executable SHA256 hashes.

Download the complete ZIP and keep the helper executables together. Audio conversion requires FFmpeg; some proprietary formats require keys or are unsupported. Back up important data before disk operations.

Source and feedback: https://github.com/huvubb/system-toolbox

Release: https://github.com/huvubb/system-toolbox/releases/tag/v1.9

## 发布记录与后续渠道

- GitHub Release：发布 v1.9，提供完整 ZIP 与哈希清单。
- GitHub 仓库：补充中英文简介、功能关键词与下载入口。
- GitHub Pages：提供公开产品介绍与下载页。
- 第三方社区：以上文案可用于允许作者分享项目的版块；发布前核对该版块规则，披露作者身份，避免重复发帖和无关评论。

传播效果可通过 GitHub 仓库 Insights 的流量、来源和 Release 下载量观察；下载次数不等同于独立用户数。
