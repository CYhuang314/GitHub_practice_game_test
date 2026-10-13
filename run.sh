#!/bin/sh
set -eu
project_root=$(CDPATH= cd "$(dirname "$0")" && pwd)
sh "$project_root/scripts/build.sh"
cd "$project_root"
case $(uname -s) in
    MINGW*|MSYS*|CYGWIN*) exec ./build/git-game.exe "$@" ;;
    *) exec ./build/git-game "$@" ;;
esac
