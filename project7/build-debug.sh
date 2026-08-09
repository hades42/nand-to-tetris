#!/bin/sh

set -eu

project_dir=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
build_dir="$project_dir/build"
compiler=${CC:-clang}

mkdir -p "$build_dir"

"$compiler" \
    -std=c11 \
    -g \
    -O0 \
    -Wall \
    -Wextra \
    -Wpedantic \
    "$project_dir"/*.c \
    -o "$build_dir/VMTranslator"

printf 'Built %s\n' "$build_dir/VMTranslator"
