import assert from 'node:assert/strict';
import fs from 'node:fs';
import os from 'node:os';
import path from 'node:path';
import { fileURLToPath } from 'node:url';
import { spawnSync } from 'node:child_process';

// This test needs Node only in CI/development. Playing the game does not.
const projectRoot = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const windows = process.platform === 'win32';
const scratch = fs.mkdtempSync(path.join(os.tmpdir(), 'gpg-launcher-'));
const fixture = path.join(scratch, 'project with spaces');
const outside = path.join(scratch, 'outside project');
const env = { ...process.env };
fs.mkdirSync(outside);
fs.cpSync(projectRoot, fixture, {
    recursive: true,
    filter: source => !['.git', 'build', '.git-game-progress', '.instructor'].includes(
        path.relative(projectRoot, source).split(path.sep)[0])
});

const powershell = windows
    ? path.join(env.SystemRoot || 'C:\\Windows', 'System32', 'WindowsPowerShell', 'v1.0', 'powershell.exe')
    : null;
const comspec = env.ComSpec || 'cmd.exe';
const launchers = windows ? ['cmd', 'powershell'] : ['sh'];
let observedWindowsCompiler;

function invoke(launcher, args, { input = '', environment = env } = {}) {
    let command, commandArgs, verbatim = false;
    if (launcher === 'powershell') {
        command = powershell;
        commandArgs = ['-NoProfile', '-ExecutionPolicy', 'Bypass', '-File', path.join(fixture, 'run.ps1'), ...args];
    } else if (launcher === 'cmd') {
        command = comspec;
        // All shell fragments here are fixed test inputs, not user-provided text.
        commandArgs = ['/d', '/s', '/c', `""${path.join(fixture, 'run.cmd')}" ${args.join(' ')}"`];
        verbatim = true;
    } else {
        command = 'sh';
        commandArgs = [path.join(fixture, 'run.sh'), ...args];
    }
    const result = spawnSync(command, commandArgs, {
        cwd: outside, env: environment, input, encoding: 'utf8',
        windowsVerbatimArguments: verbatim, timeout: 120000, maxBuffer: 2 * 1024 * 1024
    });
    if (result.error) throw result.error;
    result.output = result.stdout + result.stderr;
    return result;
}

function checkLaunchers() {
    for (const launcher of launchers) {
        let result = invoke(launcher, ['--no-save', '--list']);
        assert.equal(result.status, 0, result.output);
        if (windows && !observedWindowsCompiler) {
            observedWindowsCompiler = result.output.match(/^Compiler: (.+)\r?$/m)?.[1].trim();
            assert.ok(observedWindowsCompiler, 'Launcher must report the selected compiler');
        }
        assert.match(result.stdout, /GIT PATCH PUSHER/);
        assert.match(result.stdout, /Tutorial stamps: 0\/2/);
        assert.equal((result.stdout.match(/\[OPEN\]/g) || []).length, 2);
        console.log(`PASS ${launcher}: compile and launch from another directory; project path contains spaces.`);

        result = invoke(launcher, ['--no-save'], { input: '1\nd\nn\nssdd\nm\nq\n' });
        assert.equal(result.status, 0, result.output);
        assert.equal((result.stdout.match(/PATCHES DELIVERED!/g) || []).length, 2, result.output);
        assert.equal(fs.existsSync(path.join(fixture, '.git-game-progress')), false);
        console.log(`PASS ${launcher}: interactive input, both tutorials, argument forwarding, no-save.`);

        result = invoke(launcher, ['--level', '99']);
        assert.equal(result.status, 2, result.output);
        console.log(`PASS ${launcher}: preserves game exit code 2.`);

        result = invoke(launcher, ['--check-unlock', '3']);
        const binary = path.join(fixture, 'build', windows ? 'git-game.exe' : 'git-game');
        const directProbe = spawnSync(binary, ['--check-unlock', '3'], { encoding: 'utf8', timeout: 10000 });
        assert.ok([0, 1].includes(directProbe.status), directProbe.stderr);
        assert.equal(result.status, directProbe.status, result.output);
        assert.match(result.stdout, /PASS: UNLOCKED|FAIL: STILL LOCKED/);
        console.log(`PASS ${launcher}: preserves unlock-check status for both starter and repaired code.`);

        result = invoke(launcher, ['--no-save', '--list'], {
            environment: { ...env, CC: 'gpg-intentionally-missing-c-compiler' }
        });
        assert.equal(result.status, 1, result.output);
        assert.match(result.output, /compiler not found/i);
        assert.doesNotMatch(result.output, /GIT PATCH PUSHER/);
        console.log(`PASS ${launcher}: missing compiler stops with an actionable message.`);
    }

    const launcher = launchers[0];
    const saved = invoke(launcher, [], { input: '1\nd\nq\n' });
    assert.equal(saved.status, 0, saved.output);
    assert.equal(fs.existsSync(path.join(fixture, '.git-game-progress')), true);
    assert.equal(fs.existsSync(path.join(outside, '.git-game-progress')), false);
    const restored = invoke(launcher, ['--list']);
    assert.equal(restored.status, 0, restored.output);
    assert.match(restored.stdout, /best: 1 steps/);
    console.log('PASS progress is saved in the project, not in the caller directory, and reloads.');

    // CC may be a full executable path containing spaces; do not split it.
    let compiler = env.CC;
    if (windows) {
        const realCompiler = observedWindowsCompiler;
        assert.ok(realCompiler, 'No compiler found for explicit CC test');
        const compilerDirectory = path.join(scratch, 'compiler with spaces');
        fs.mkdirSync(compilerDirectory);
        const driver = /^(cl|clang-cl)(\.exe)?$/i.test(path.basename(realCompiler)) ? 'cl' : 'gcc';
        compiler = path.join(compilerDirectory, `${driver}.cmd`);
        fs.writeFileSync(compiler, `@echo off\r\n"${realCompiler}" %*\r\nexit /b %errorlevel%\r\n`);
    } else {
        const found = spawnSync('sh', ['-c', 'command -v "${CC:-cc}"'], { env, encoding: 'utf8' });
        assert.equal(found.status, 0, found.stderr);
        const compilerDirectory = path.join(scratch, 'compiler with spaces');
        fs.mkdirSync(compilerDirectory);
        compiler = path.join(compilerDirectory, 'cc');
        fs.symlinkSync(found.stdout.trim(), compiler);
    }
    const overridden = invoke(launcher, ['--no-save', '--list'], {
        environment: { ...env, CC: compiler }
    });
    assert.equal(overridden.status, 0, overridden.output);
    assert.match(overridden.stdout, /GIT PATCH PUSHER/);
    console.log('PASS CC supports a full compiler path containing spaces.');
}

try {
    checkLaunchers();
    console.log(`Launcher checks passed on ${process.platform}/${process.arch}.`);
} finally {
    // Delete only the unique temporary fixture created by this test.
    const resolved = path.resolve(scratch);
    assert.equal(path.dirname(resolved), path.resolve(os.tmpdir()));
    assert.ok(path.basename(resolved).startsWith('gpg-launcher-'));
    fs.rmSync(resolved, { recursive: true, force: true });
}
