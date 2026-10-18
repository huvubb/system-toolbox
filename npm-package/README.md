# system-toolbox

免费开源的 Windows 10/11 x64 系统维护工具箱。原生 EXE 随 npm 包一起提供，安装时只访问 npm 软件源，无需另外下载 GitHub 附件或国内镜像，也不运行安装脚本。

```sh
npm install system-toolbox
npx system-toolbox
```

希望在任意目录直接启动时，使用全局安装：

```sh
npm install -g system-toolbox
system-toolbox
```

查看安装版本或帮助不会启动系统维护程序：

```sh
npx system-toolbox --version
npx system-toolbox --help
```

启动工具箱时会请求 Windows 管理员权限。包含后台队列、fastdl 下载器、音乐转换组件、可选 DiskTool DLC 和菜单 35 的 Windows ISO 启动盘制作。音乐转码需要另外安装 FFmpeg；部分厂商格式需要密钥或尚不兼容。制作启动盘会清除目标盘，操作前请备份。

Requires Node.js 18+ and Windows x64. Native executables are bundled in the npm tarball; installation does not download additional binaries or execute postinstall scripts. A local install provides `npx system-toolbox`; a global install provides `system-toolbox` on npm's global command path.

Windows 的 npm 命令入口在含 `&` 的目录中可能失败；请使用普通目录，或直接运行 `node "node_modules/system-toolbox/cli.js"`。

## Maintainer packaging

Copy the current tested Windows build's three EXEs, README and four component license/notice Markdown files into `bin/`. Generate `bin/SHA256SUMS.txt` for those eight files, copy the root MIT LICENSE here, then run `npm test` and `npm pack`. The prepack check refuses missing, unexpected or modified files in that manifest. Inspect and install the resulting tarball in a temporary directory before publishing. Generated binaries and tarballs stay out of Git.

Source and feedback: https://github.com/huvubb/system-toolbox

Contact: **810372789@qq.com**
