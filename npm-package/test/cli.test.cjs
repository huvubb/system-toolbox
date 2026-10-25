const test = require('node:test');
const assert = require('node:assert/strict');
const fs = require('node:fs');
const path = require('node:path');
const vm = require('node:vm');
const { spawnSync } = require('node:child_process');
const cli = path.join(__dirname, '..', 'cli.js');

test('help and version work without starting the maintenance application', () => {
  for (const [flag, expected] of [['--version', require('../package.json').version], ['--help', 'npx system-toolbox']]) {
    const result = spawnSync(process.execPath, [cli, flag], { encoding: 'utf8' });
    assert.equal(result.status, 0);
    assert.ok(result.stdout.includes(expected));
  }
});

test('elevation quotes apostrophes in installation paths', () => {
  const calls = [];
  const context = {
    __dirname: "C:\\工具 O'Brien & folder\\system-toolbox",
    process: { argv: ['node', 'cli.js'], env: { SystemRoot: 'C:\\Windows' } }, console,
    require(name) {
      if (name === 'node:fs') return { existsSync: () => true };
      if (name === 'node:path') return path.win32;
      if (name === 'node:child_process') return {
        spawnSync(program, args) { calls.push({ program, args }); return { status: calls.length === 1 ? 1 : 0 }; },
        spawn() { throw new Error('Maintenance application must not run in this test'); }
      };
      throw new Error('Unexpected import: ' + name);
    }
  };
  vm.runInNewContext(fs.readFileSync(cli, 'utf8'), context);
  assert.equal(calls.length, 2);
  assert.ok(calls[1].args.at(-1).includes("O''Brien & folder"));
  assert.equal(context.process.exitCode, undefined);
});
