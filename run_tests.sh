#!/usr/bin/env bash
set -euo pipefail

# 切换到脚本所在目录
cd "$(dirname "$0")"

mkdir -p build

# 编译每个测试文件为单独可执行（递归查找 .cpp）
find test/local -type f -name '*.cpp' -print0 | while IFS= read -r -d '' testfile; do
  base=$(basename "$testfile" .cpp)
  clang++ -std=c++20 -O2 \
    -I. \
    "$testfile" \
    third_party/catch/catch_amalgamated.cpp \
    -o build/"$base"
done

# 运行所有测试
for exe in build/*; do
  echo "Running $exe"
  "$exe" --reporter compact
done
