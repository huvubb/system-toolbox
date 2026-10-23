#!/usr/bin/env node
'use strict';
const fs = require('node:fs');
const path = require('node:path');
const { spawn, spawnSync } = require('node:child_process');

if (process.argv.includes('--version') || process.argv.includes('-v')) {
  console.log(require('./package.json').version);
  process.exit(0);
}
if (process.argv.includes('--help') || process.argv.includes('-h')) {
  console.log('System Toolbox (Windows x64)\nLocal install: npm install system-toolbox\nRun: npx system-toolbox\nGlobal install: npm install -g system-toolbox\nRun globally: system-toolbox\nThe toolbox requests administrator rights when launched.');
  process.exit(0);
}
const binDir = path.join(__dirname, 'bin');
const exe = path.join(binDir, 'system-toolbox.exe');
for (const name of ['system-toolbox.exe']) {
  if (!fs.existsSync(path.join(binDir, name))) {
    console.error(`[system-toolbox] Missing ${name}. Reinstall with: npm install system-toolbox@latest`);
    process.exit(1);
  }
}
const powershell = path.join(process.env.SystemRoot || 'C:\\Windows', 'System32', 'WindowsPowerShell', 'v1.0', 'powershell.exe');
const admin = spawnSync(powershell, ['-NoProfile', '-NonInteractive', '-Command',
  '$p=[Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent(); if($p.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)){exit 0}else{exit 1}'
], { stdio: 'ignore' });
if (admin.error) {
  console.error('[system-toolbox] Cannot check administrator rights: ' + admin.error.message);
  process.exit(1);
}
if (admin.status === 0) {
  const child = spawn(exe, process.argv.slice(2), { stdio: 'inherit', cwd: binDir });
  child.on('error', error => { console.error(error.message); process.exitCode = 1; });
  child.on('exit', (code, signal) => { process.exitCode = signal ? 1 : (code ?? 1); });
} else {
  const quote = value => "'" + value.replace(/'/g, "''") + "'";
  const result = spawnSync(powershell, ['-NoProfile', '-NonInteractive', '-Command',
    `$ErrorActionPreference='Stop'; try { Start-Process -FilePath ${quote(exe)} -Verb RunAs -WorkingDirectory ${quote(binDir)} -ErrorAction Stop | Out-Null } catch { Write-Error $_; exit 1 }`
  ], { stdio: 'inherit' });
  if (result.error || result.status !== 0) {
    console.error('[system-toolbox] Administrator launch failed or was cancelled.');
    process.exitCode = 1;
  }
}
