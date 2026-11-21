# 更新日志

## npm 1.17.2 — 2026-11-21

### WinRing0 驱动加载诊断增强

- 错误码 0（服务已启动但设备节点打不开）细分为专属场景：CreateFile 错误码随提示显示，指向杀软拦截/HVCI，并给出事件查看器排查路径。
- 各典型错误码（2/5/1058/1072/1073/1275）分别给出针对性说明。

### 插槽信息支持 NVMe（磁盘与存储插槽）

- NVMe/M.2 实际占用：按实际接入的 NVMe 盘数展示（型号+容量），并与 SMBIOS 报告的 M.2 插槽总数对照；SMBIOS 未标记占用时以实际接入为准（常见于笔记本 BIOS）。
- npm 版本 1.17.2。

## npm 1.17.1 — 2026-11-21

### DiskTool DLC 下载源调整

- DiskTool DLC 下载改为仅从 GitHub Release 获取（github.com/huvubb/disktool），移除个人镜像站点备用源。
- QQ 音乐转码器安装包下载源保持不变（主镜像 + web.archive.org 备源）。
- npm 版本 1.17.1。

## npm 1.17.0 — 2026-11-16

### 网卡驱动救援包扩到 9 品牌

- 新增：Realtek 有线（RTL810x/811x/8125 全系）、Realtek Wi-Fi（RTL8812/8821/8852 USB+PCIe 全系）、Realtek USB（RTL8153）、Broadcom NetXtreme 有线全系、ASIX USB（AX88179/178A/772D）。
- 内嵌包 25MB，覆盖 Intel/Realtek（有线+WiFi+USB）/Broadcom/ASIX/Qualcomm/MediaTek 全部主流有线、无线与 USB 网卡。

### C 盘清理 AppData 深度清理（新增 6 项）

- AppData 软件缓存（Chrome/Edge Cache、Firefox Profiles 缓存；只删缓存不动配置）。
- 应用崩溃转储（CrashDumps/WER ReportArchive/ReportQueue，实测 873MB）。
- Windows 更新下载缓存（SoftwareDistribution\Download，自动停启 wuauserv）。
- 传递优化缓存（Delivery Optimization，实测 5.19GB）。
- 系统日志与着色器缓存（CBS/DISM 日志、D3DSCache、NVIDIA/AMD DXCache）。
- 开发工具缓存（npm/pip/Hugging Face/NuGet/Yarn/pnpm）。
- 清理选择支持 all 全选与多选逗号组合。
- npm 版本 1.17.0。

## npm 1.16.1 — 2026-11-16

### Linux 格式化无 WSL 修复

- 明确提示挂物理盘必须 WSL2（WSL1 不支持 wsl --mount），引导 wsl --install -d Ubuntu。
- 修复 mkfs 工具链检测：wsl 无发行版时退出码仍为 0，改为输出标记文本校验（MKFS_TOOL_PRESENT），实测引导路径正确触发。

## npm 1.16.0 — 2026-11-16

### Linux 文件系统格式化（磁盘工具 → 15）

- 格式化分区新增 Linux 全系列：ext2/ext3/ext4、XFS、Btrfs、F2FS、exFAT（exfatprogs）、swap——经 WSL 发行版内 mkfs 对物理分区执行（wsl --mount --bare 挂盘 → mkfs → 卸载）。
- WSL 内目标设备按磁盘号映射 /dev/sdXN，执行前显示映射结果并二次确认。

## npm 1.15.3 — 2026-11-16

### 磁盘格式化文件系统扩展（磁盘工具 → 15）

- 格式化分区支持全部主流文件系统：NTFS、exFAT、FAT32、FAT16、FAT12、ReFS（diskpart 原生格式化）。
- 新增 Apple 文件系统预设：HFS+ 与 APFS——Windows 没有原生格式化器，本操作将分区类型 GUID 设为 Apple 标准（HFS+ 48465300-…/APFS 7C3457EF-…）并提示到 macOS 磁盘工具执行抹掉。
- 临时 diskpart 脚本改存 %TEMP%。

## npm 1.15.2 — 2026-11-16

### 内嵌 Intel Wi-Fi 驱动升级 24.80.0.2（修复 AX203 首装不工作）

- 24.70.0.3 对 AX203（DEV_51F1/7740）支持不全，无驱动状态首装后网卡不工作。
- 从本机 DriverStore 提取 Intel 官方签名且实测驱动 AX203 成功的 24.80.0.2 替换，NetDrivers.7z 更新为 23MB。

## npm 1.14.0 — 2026-11-16

### 应急断网（网络工具 → 28/29）

- 一键断网：停止 DNS/DHCP/网络服务，禁用全部物理网卡、虚拟网卡与 VPN/TAP 适配器，丢弃当前所有 TCP/UDP/VPN 连接；输入 DISCONNECT 二次确认（防止远程会话自锁）；完成后报告剩余在线适配器。
- 一键恢复网络：重新启用全部适配器、恢复网络服务、重置 DNS/DHCP 并验证连通性。

### 网卡驱动救援包（驱动工具 → 24）

- 内嵌通用网卡驱动合集（推荐）：构建时嵌入 Intel/Realtek/Qualcomm/MediaTek 四品牌官方驱动（NetDrivers.7z，23MB，覆盖 2728 个硬件 ID：Intel Netwtw08 **24.80.0.2**——24.70 及以下对 AX203 支持不全，本版从已实测驱动 AX203 成功的本机 DriverStore 提取 Intel 官方签名版本；Realtek 8852BE、Qualcomm/Atheros 1611 个、MediaTek MT7920/7921），断网救急零下载零依赖本机。
- 断网安装自动识别 Net 类 INF（兼容 UTF-16 编码 INF，Class= 任意空格对齐），实测 5 个 INF 识别 5 个、安装 5 成功 0 失败（already exists 正确计为成功）。
- 硬件扫描修复：pnputil /scan-devices 被拒时回退 CM_Reenumerate_DevNode 全树重枚举，不再对 WAN Miniport 等系统设备报 Access denied。
- 安装完成后自动检测网卡设备状态：已工作/有问题/硬件不在位三种结论分别提示（含 M.2 插槽与 BIOS 检查建议）。
- 本机驱动导出为救援包：DISM 导出全部第三方驱动打包 Drivers.7z（补充备份，菜单 4）。
- 断网安装网卡驱动：解压救援包，自动识别 INF 的 Class=Net（网卡类）离线安装（无 Net 类时可选择全量安装），完成后扫描设备；网卡恢复在线后即可用 Windows Update 补齐全套驱动，回到在线模式。
- 在线查询驱动库：连接 Windows Update 实时查询本机可用的驱动更新数量与标题清单。

### WinRing0 驱动自动获取（主菜单 39 → 5）

- 本地没有 WinRing0x64.sys 时改为 curl 自动下载（LibreHardwareMonitor v0.9.2 官方仓库），SHA256 锁定校验后使用，不再要求手动下载。

### DLL 修复去名称化（主菜单 37 → 5）

- 新增「缺啥补啥」：对照修复库清单扫描系统缺失组件自动补齐；系统已有同名文件不替换。
- 「全量修复」同样改为仅补缺失，不覆盖任何已有 DLL。
- 新增「粘贴报错修复」：把弹窗报错文本整段粘贴进来，自动提取全部 *.dll 文件名并逐一匹配修复，无需记忆文件名。

### 其他

- Pause 后立即清屏：每个操作结束按键返回后界面即清空，输出不再向下无限延伸。
- NTDLL 菜单改用 GetModuleHandleW 获取（ntdll 进程内必然已映射），替代 LoadLibraryA。
- 实时面板网卡吞吐量去掉管道符进度条，仅显示收发速度。
- npm 版本 1.14.0。

## npm 1.13.0 — 2026-11-16


### AI 算力测试校验重做（主菜单 38）

- CPU FP32：校验改为清零矩阵后以相同循环重跑 1 轮，随机抽 8 个元素与双精度参考值对比（阈值 1e-4，实测误差 0.00%）；此前仅检查数值有限性，无法发现计算错误。
- CPU INT8：校验改为用参考公式独立重算全部 64M 项的精确校验和，与多线程实测值逐位对比；此前仅检查溢出。
- GPU Compute：校验改为利用迭代 a=a*b+c 的数学特性——z 分量（b<1）必须收敛到理论不动点 1250±1%；此前仅检查数值有限性。

### DLL 修复库内嵌（主菜单 37 → 5）

- DirectXRepair.7z（图吧 DirectX_Repair 数据包，100 个 DLL，约 90MB）在构建时以 ld -b binary 直接嵌入 EXE；运行时外部 .7z 缺失时自动从内嵌数据释放到数据目录后解压修复，无需单独下载（实测释放+按名修复全链路通过）。
- 构建命令更新：需先生成 dxdata.o（ld -b binary DirectXRepair.7z）并加 -DSTB_EMBED_DXREPAIR 链接；不带该宏构建时自动回退为外部 .7z 模式。
- 发行 ZIP 精简为仅含 EXE、CPP 与 README.md。

## npm 1.12.0 — 2026-11-15


### DLL 修复库（主菜单 37 → 5，图吧 DirectX_Repair 数据包兼容）

- 新增 DLL 修复库一键修复：把图吧工具箱 DirectX_Repair 的 Data 目录（A/B/C/L，100 个 DLL，约 119MB）打包为 DirectXRepair.7z（约 90MB），工具箱自动解压到数据目录并支持按文件名修复与全量修复。
- 安装时读取 PE 头自动判断 32/64 位并分别放入 SysWOW64 / System32；已有文件自动备份为 .stb-bak。
- 压缩包需放置在 EXE 同目录或 drivers 目录；发布页提供 DirectXRepair.7z 独立附件；解压需 7z.exe（自动搜索 PATH、Program Files、EXE 目录）。

### DirectX 12 测试（主菜单 22 → 6）

- 新增 D3D12 渲染压测：设备特性等级检测（11_0/11_1/12_0/12_1）、D3D12 命令队列 + FLIP_DISCARD 交换链 + fence 同步的清屏渲染循环，实时 FPS（标题栏），ESC 退出。

### 驱动工具扩展（主菜单 23 → 22/23）

- 注入驱动到离线系统目录（DISM /Image /Add-Driver /Recurse）：适用于部署的系统目录、挂载的 install.wim/boot.wim 与 WinPE，可选强制注入未签名驱动。
- 一键更新缺失驱动：通过微软官方 Windows Update COM 接口按设备硬件 ID 自动匹配、下载并安装全部缺失驱动（WU 服务被禁用时自动提示恢复），完成后 pnputil 扫描设备变更。

## npm 1.11.0 — 2026-11-15


### 网卡吞吐量与镜像源（主菜单 39）

- 实时面板新增网卡吞吐量：GetIfTable2 差值采样，仅显示硬件网卡（过滤 Npcap/QoS/WFP 伪网卡），每网卡显示 ↓接收/↑发送速度，并用管道符进度条相对渲染；速度自适应 B/s→GB/s。
- winget 安装首次执行前静默切换到中科大 winget-source 镜像（winget source remove/add，失败不阻断）；npm install 一律追加 npmmirror 镜像。说明：清华 TUNA 未提供 winget/npm 镜像（实测 404），故选用等效国内官方镜像。
- 构建新增 iphlpapi 链接；MinGW 需 _WIN32_WINNT=0x0A00 且先包含 ws2ipdef.h（netioapi.h 的 _WS2IPDEF_ 守卫），已在源码处理。

### 测试启动提示

- GPU 测试（DirectX 11/Compute）、RAM 带宽测试在初始化/分配阶段补充即时进度提示（设备创建、着色器编译、内存分配清零），消除启动时 2-3 秒的无反馈期。

## npm 1.10.9 — 2026-11-15


### 依赖项下载改用 CURL 并跳过 SSL/TLS 校验

- TrustedInstaller 依赖（PsExec64，live.sysinternals.com）、QQ音乐转码器安装包（主镜像与 web.archive.org 备源）、DiskTool DLC（GitHub Release 与国内镜像）改为 CURL 拉取，强制 `--insecure --ssl-no-revoke` 跳过 SSL/TLS 证书校验与吊销检查，规避镜像站点证书链不完整导致的下载失败。
- 内容完整性不受影响：QQ音乐安装包仍做 SHA256 校验，PsExec64 仍做微软签名校验，DiskTool DLC 仍核验记录的构建哈希。
- 工具箱自更新下载不受影响（保持 FastDL/curl 常规校验与哈希清单）。
- FFmpeg 等经 winget 安装的软件不适用 CURL 参数，行为不变。

## npm 1.10.8 — 2026-11-15


### CPU/显卡/风扇监测扩展（主菜单 39 → 5，自研引擎）

- 新增「CPU 温度与 EC 风扇」：WinRing0 内核驱动（用户手动放置于 EXE 目录或 drivers 目录，发行包不捆绑；驱动已被微软列入易受攻击驱动名单，HVCI 开启或杀软拦截时无法加载，使用与否由用户判断）。
- CPU 温度走 MSR DTS：Intel 每核 0x19C + TjMax 0x1A2 切核读取取最热核心（实测 i5-13420H 正常）；AMD 经 SMN 0x59800/PCI 配置空间。
- EC 风扇：自动探测标准端口 62/66（命令/内存模式）与 ACPI 2.0 端口 380/384，提供 00-FF 寄存器转储与相邻字节对风扇转速候选（实验性）；未暴露 EC 的机器给出说明。
- NVIDIA 显卡新增风扇百分比（NvAPI GetCoolerSettings）；AMD 显卡温度与风扇 RPM（ADL，atiadlxx.dll/atiadlxy.dll 动态加载）；Intel 核显温度走 ACPI 热区并明确说明。
- 实时面板新增 CPU 温度行（检测到驱动文件时自动启用）。
- 驱动服务加载处理禁用/待删除竞态（自动恢复启动类型、停止+删除+重建）。

## npm 1.10.7 — 2026-11-15


### 硬件实时监测（主菜单 39，自研引擎）

- 实时监测面板：curl 式清屏重绘，刷新间隔 1-10 秒可调，ESC 退出；CPU 负载（GetSystemTimes 差值）、内存占用、ACPI 热区温度（原生 IOCTL 热区接口，不可用时回退 WMI）、NVIDIA 显卡温度（NvAPI 动态加载 nvapi64.dll）、硬盘温度（NVMe 直读 SMART/Health 日志、SATA 读 SMART 属性 194/190）。
- 内存插槽信息：SMBIOS 直读占用/总数与主板最大容量（GetSystemFirmwareTable 不可用如 Win11 部分机型回退 WMI），逐条列出插槽位置/容量/频率/厂商/型号，空插槽单独标注。
- 硬盘与存储插槽：CreateFile + IOCTL 枚举物理磁盘与光驱（StorageDeviceDescriptor 总线类型），按 IDE/SATA/USB/NVMe 分类；CD-ROM 显示盘符；SMBIOS Type 9 插槽按 NGFF/M.2、mSATA、SATA、IDE 归类并统计占用/总数，未报告类型显示 0/0。
- RAM 基准测试：多线程读取/写入/复制带宽（GB/s），3 轮取最佳。
- 构建新增 setupapi 链接；CMake 与 MinGW 命令同步。

## npm 1.10.6 — 2026-11-15


### Windows 更新管理修复（主菜单 2）

- 禁用逻辑补全：新增禁用「Windows 更新医疗服务」（WaaSMedicSvc）——此前更新禁用后会被该服务自动改回去（复活的元凶）；该服务受系统保护，通过注册表方式禁用，重启后彻底生效。
- 新增禁用 WindowsUpdate/UpdateOrchestrator 相关计划任务（不存在的任务自动忽略）。
- 服务与策略操作增加实际状态校验（通过 SCM API 与注册表读取），完成后给出准确结论，不再因「服务已停止」「值不存在」等正常状态误报失败。
- 恢复逻辑：服务改回系统默认的手动启动（手动+触发器），医疗服务恢复手动；策略值从未设置时跳过删除；恢复后校验并提示服务由系统按需拉起。

## npm 1.10.5 — 2026-11-14


### AI 算力测试测量时长修复

- CPU FP32/INT8 改为自适应时长测量：每项重复计算累计约 2 秒后按总吞吐计算成绩，避免矩阵过小、零点几秒即结束导致的失真；结果显示轮数与累计耗时。
- GPU 计算着色器压测自动校准：先测基准耗时，自动提高迭代次数使每轮约 1.5 秒（上限 5000 万次迭代），成绩按实际迭代次数计算并显示规模。
- 修复 FP32 测试数据无符号下溢：size_t 直接参与减法在小于阈值时回绕为约 1.8e17 的超大值，多轮累加后校验失败；现先转 int 再计算，校验恢复通过。
- 修复多核加速比计算：固定测量时长后改用多核/单核 GFLOPS 比值。
- INT8 校验和改为原子累加，消除多线程数据竞争。

## npm 1.10.4 — 2026-11-14


### 文件数字签名（主菜单 12）

- 签名后自动把证书安装到本机「受信任的根证书颁发机构」和「受信任的发布者」，签名直接被系统识别为可信，不再提示未知发布者。
- 证书同时导出为 .cer 文件，便于在其他电脑导入；临时脚本改存 %TEMP%，不再写到 C 盘根目录。
- 修复中文路径下文件无法识别的问题（改用宽字符 API），脚本改为带 BOM 的 UTF-8，PowerShell 5.1 可正确读取中文路径。

### 驱动工具人性化（主菜单 23）

- 新增「驱动体检」：列出状态异常的在用设备，并给出中文处理建议。
- 新增「给设备装驱动」：列出缺少驱动的设备供选择，读取设备硬件 ID，在指定文件夹（含子目录）中自动匹配 INF 并安装，全程无需知道 INF 名称；兼容 UTF-16 编码的 INF。
- 新增「一键备份所有驱动」：默认保存到工具箱数据目录 drivers-backup，带时间戳，并说明还原方法。
- 新增「电脑硬件概览」：型号、系统、处理器、内存、显卡/声卡/网卡/硬盘一览。
- 新增「从文件夹批量安装驱动」；原有查询/备份/签名/冲突检测保留并重新编号。

## npm 1.10.3 — 2026-11-13


### AI 算力测试（主菜单 38）

- CPU：FP32 矩阵乘法测量单核/多核 GFLOPS 与并行加速比，INT8 乘加测整数推理速率，结果做有限值校验。
- GPU：DirectX 11 计算着色器执行 FMA 压测，输出单精度算力（GFLOPS），预热后 3 轮取平均与最佳，并回读校验计算结果。
- 内存带宽：多线程写入/读取/复制速度（GB/s），3 轮取最佳。
- 全部本机运行，离线模式可用；综合跑分依次完成以上三项。

### 硬件压力测试完整实现（主菜单 10）

- 新增 GPU 压力测试入口：DirectX 11 / OpenGL / 多窗口。DirectX 11 测试现真正编译并绑定顶点/像素着色器（运行时加载系统 d3dcompiler_47.dll），配合顶点旋转动画形成实际渲染负载，关闭垂直同步并在标题栏实时显示 FPS；修复旧版着色器从未编译绑定导致 GPU 空载的问题。
- 内存测试：7 种位图案（全 0/全 1/交替 1010/0101/棋盘 A5/5A/随机数据）全量逐字节校验，失败时报告图案与首个错误字节偏移；带宽测试测量多线程写入/读取/复制速度。

### 更新提示可关闭

- 更新内容页新增“是否不再显示更新提示？”；选择是后，更新完成直接进入主菜单。主菜单 25（软件更新）显示提示状态并可重新开启。
- 设置保存在 %LOCALAPPDATA%\SystemToolbox\settings.txt（SkipUpdateNotice）。

## npm 1.10.2 — 2026-11-13


### 启动盘制作（主菜单 35）

- 新增 Windows To Go 模式：整盘应用 `install.wim/install.esd`，不再受 30GB 启动分区上限限制，64GB 等大容量磁盘可整盘使用。UEFI 采用 GPT（300MB EFI 系统分区 + NTFS 系统分区），UEFI+传统 BIOS 采用 MBR 单 NTFS 活动分区。
- 两种模式均可选移除 TPM 2.0 / 安全启动 / 内存检查并跳过微软联网账户强制要求：普通安装盘写介质根目录 `unattend.xml`，WinToGo 写离线注册表与 `Windows\Panther` 应答文件。
- 映像应用改用系统内置 `dism.exe /Apply-Image`（失败回退 DISM PowerShell cmdlet），应用后校验 `ntoskrnl.exe`、`winload.efi`、注册表配置单元及 BIOS/UEFI 启动文件。
- 映像列表优先读取真实 `ImageIndex`，缺失时按枚举顺序自动编号；显示序号与实际应用索引分离，也支持直接输入真实索引。
- 系统架构按 ISO 内 `efi\boot\boot*.efi` 文件名推断（`bootx64`/`bootaa64`/`bootia32`），再回退 WIM 元数据与本机架构；容量判断使用所选映像体积加余量，元数据缺失时按保守值估算。
- EFI 系统分区不分配盘符：分区与卷一律通过 `Get-Partition`/`Get-Volume` 对象定位，EFI 文件以卷 GUID 路径（`\\?\Volume{...}\`）读写；仅在调用 `bcdboot /s` 时临时挂载访问路径并立即撤销。
- UEFI 模式向 ESP 写入 `\EFI\Boot\bootx64.efi`（ARM64 为 `bootaa64.efi`）回退路径，便于 Intel Mac 的 Option 启动菜单识别；MBR 模式改用 `bcdboot /f ALL` 生成 BIOS/UEFI 双引导，失败自动回退纯 BIOS。
- 按离线系统 `Select\Current` 定位控制集，将实际存在的 USB/存储控制器驱动设为引导启动，并标记 `PortableOperatingSystem` 与 `SanPolicy`，提升换机启动成功率（该步骤失败仅警告）。
- 修复 GPT→MBR 转换（`Set-Disk` 不接受 `RAW`，改用 `diskpart clean`；样式已符合时跳过 `Initialize-Disk`）、离线注册表残留挂载锁住 hive 文件、以及 `bootsect.exe` 签名校验在 Security 模块不可用时回退读取 PE 内嵌证书。

### 卸载与维护

- 首次启动自动注册 HKCU 卸载表项，系统「应用和功能」可直接卸载；主菜单 91 提供卸载流程，需输入 UNINSTALL 确认，依次清理注册项、`%LOCALAPPDATA%\SystemToolbox` 数据目录与程序自身。
- 诊断报告新增磁盘空间、开机启动项、网络配置三项；后台命令补充权限不足、找不到程序、用户取消的友好错误提示。

### 构建与兼容性

- 修复 MSVC C2026：内嵌 PowerShell 脚本按函数边界拆为三段相邻字符串拼接，每段低于 16KB，CI 的 MSVC 与 MinGW 构建均恢复通过。
- 控制台输入链路：禁用 stdin 缓冲、增加输入结束保护、移除菜单路径上会抢占管道的 cmd 子进程；支持 `STB_MEDIA_*` 环境变量预置脚本交互选项（正常菜单交互不变）。

### 同期未发布项（2026-10-23，已并入 1.10.2）

- 发行压缩包仅包含 cpp、exe、md；移除 FastDL 主包内文件，CI 附件不再同时重复上传散装程序。校验清单作为压缩包外的独立附件。

- FastDL 调整为可选 DLC，增加本地文件导入与 SHA256 校验；缺失时使用 curl 下载。
- curl 下载成功后替换目标文件，失败时保留原目标；当前回退以前台执行。
- DiskTool 菜单退出后返回工具箱主菜单。
- 常用软件官网菜单严格检查编号，并报告浏览器启动失败；修复搜索字符大小写转换问题。
- 已通过 C++17 构建与两项 npm 启动器测试；尚未完成完整回归、发行包清理和渠道发布。

## npm 1.9.1 — 2026-10-18

- npm 包直接携带 Windows x64 主程序、音乐转换器及 FastDL，移除安装时从外部站点下载程序的依赖。
- 增加打包前 SHA256 校验、启动器帮助及版本输出，并处理管理员启动。
- 支持本地安装后使用 npx system-toolbox，或全局安装后使用 system-toolbox。
- 已从 npm registry 实际安装并核验文件。

## npm 1.9.0 / GitHub v1.9 — 2026-10-18

- 发布 Windows 工具箱及 npm 安装入口。
- npm 1.9.0 安装时依赖外部下载；网络失败问题由 1.9.1 修复。
- winget huvubb.SystemToolbox 1.9 通过 PR #454989 提交；提交不等于审核上线，状态以该 PR 为准。

## DiskTool DLC v1.0.0 — 2026-10-18

- 独立 GitHub 仓库 huvubb/disktool 发布 Windows x64 静态程序和 SHA256SUMS。
- 工具箱源码优先从 GitHub 发行下载，原站点作为备用。
