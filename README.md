# 多功能系统工具箱

Windows 系统维护工具，包含以下功能：

## 功能列表
- C盘清理
- Windows 更新管理
- 硬盘修复（含U盘修复、扩容盘检测）
- 系统文件修复（sfc + DISM）
- 运行库安装（.NET / VC++）
- 常用软件官网
- ZIP 解压
- 休眠管理
- 虚拟内存管理
- 硬件压力测试（硬盘/内存/CPU/GPU/网络/接口）
- 安全检测（计划任务/进程/注册表）
- 文件数字签名
- DISM 解压 WIM
- TrustedInstaller 特权操作（需 PsExec64.exe）

- ## vx:forever870422
- ## 邮箱：810372789@qq.com
- ## 希望大家踊跃贡献自己的那一份力量！
- 
## 编译

### MinGW-w64
```bash
g++ -o system-toolbox.exe demo.cpp -std=c++11 -fexec-charset=UTF-8 -finput-charset=UTF-8 -lws2_32 -lshell32 -ladvapi32 -lopengl32 -lgdi32 -luser32 -pthread

