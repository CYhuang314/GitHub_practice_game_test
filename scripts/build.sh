#!/bin/sh
set -eu
project_root=$(CDPATH= cd "$(dirname "$0")/.." && pwd)
platform=$(uname -s)
case $platform in
    MINGW*|MSYS*|CYGWIN*) executable_suffix=.exe ;;
    *) executable_suffix= ;;
esac

compiler_help() {
    case $platform in
        Darwin) printf 'Install Apple Command Line Tools once: xcode-select --install\n' >&2 ;;
        Linux) printf 'Install a C compiler once. Ubuntu/Debian: sudo apt install build-essential\nFedora: sudo dnf install gcc\n' >&2 ;;
        *) printf 'Install GCC or Clang. On Windows, run.cmd also supports MSVC.\n' >&2 ;;
    esac
}

case ${1:-} in
    ''|--tests) ;;
    *) printf 'Usage: sh scripts/build.sh [--tests]\n' >&2; exit 2 ;;
esac
compiler=${CC:-}
if [ -z "$compiler" ]; then
    if command -v cc >/dev/null 2>&1; then compiler=cc
    elif command -v gcc >/dev/null 2>&1; then compiler=gcc
    elif command -v clang >/dev/null 2>&1; then compiler=clang
    else
        printf 'C compiler not found.\n' >&2
        compiler_help
        exit 1
    fi
fi
if ! command -v "$compiler" >/dev/null 2>&1; then
    printf 'Configured C compiler not found: %s\n' "$compiler" >&2
    compiler_help
    exit 1
fi
if ! "$compiler" --version >/dev/null 2>&1; then
    printf 'C compiler is unavailable: %s\n' "$compiler" >&2
    compiler_help
    exit 1
fi
printf 'Compiler: %s\n' "$compiler"
mkdir -p "$project_root/build"
if [ "${1:-}" = "--tests" ]; then
    "$compiler" -std=c11 -Wall -Wextra -Wpedantic -Werror -O2 \
        -I "$project_root/include" \
        "$project_root/src/game.c" "$project_root/src/levels.c" \
        "$project_root/src/progress.c" "$project_root/src/render.c" \
        "$project_root"/src/unlocks/*.c "$project_root/tests/test_game.c" \
        -o "$project_root/build/test-game$executable_suffix"
    cd "$project_root/build"
    "./test-game$executable_suffix"
else
    "$compiler" -std=c11 -Wall -Wextra -Wpedantic -Werror -O2 \
        -I "$project_root/include" \
        "$project_root"/src/*.c "$project_root"/src/unlocks/*.c \
        -o "$project_root/build/git-game$executable_suffix"
    printf 'Built: %s/build/git-game%s\n' "$project_root" "$executable_suffix"
fi
