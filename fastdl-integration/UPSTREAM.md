Bundled from the user's `D:\fastdl` repository at commit
`e8abedb3fff73808be414c36752e689652f2f4b8`. Copyright 2026 huvubb,
MIT license (see LICENSE). The fastdl package is unchanged. The entry point
adds `toolbox-download URL DESTINATION` for exact output names, output locking,
preserving existing targets on failure, and retained resume data.
The adapter binds loopback downloads to the loopback interface so fastdl's default physical-interface selection does not prevent local transfers.
