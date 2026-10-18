'use strict';
const fs = require('node:fs');
const path = require('node:path');
const crypto = require('node:crypto');
const names = ['system-toolbox.exe', 'music-converter.exe', 'fastdl.exe', 'README.md',
  'MUSIC-CONVERTER-LICENSE.md', 'MUSIC-CONVERTER-NOTICE.md', 'FASTDL-LICENSE.md', 'FASTDL-NOTICE.md'];
const directory = path.join(__dirname, 'bin');
const lines = fs.readFileSync(path.join(directory, 'SHA256SUMS.txt'), 'utf8').trim().split(/\r?\n/);
if (lines.length !== names.length) throw new Error('Unexpected native manifest file count');
const hashes = new Map();
for (const line of lines) {
  const match = /^([a-f0-9]{64}) {2}([^\\/]+)$/i.exec(line);
  if (!match || !names.includes(match[2]) || hashes.has(match[2])) throw new Error('Invalid native manifest');
  hashes.set(match[2], match[1].toLowerCase());
}
for (const name of names) {
  const actual = crypto.createHash('sha256').update(fs.readFileSync(path.join(directory, name))).digest('hex');
  if (actual !== hashes.get(name)) throw new Error('Native file hash mismatch: ' + name);
}
console.log('Verified all native binaries and license files.');
