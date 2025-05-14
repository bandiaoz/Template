#!/usr/bin/env bash
set -euo pipefail

# 切换到脚本所在目录
cd "$(dirname "$0")"

mkdir -p build

# 支持单测：如果提供一个参数，则只编译/运行该测试
if [ $# -eq 1 ]; then
  name="$1"
  # 查找指定测试文件
  files=()
  while IFS= read -r -d '' file; do
    files+=("$file")
  done < <(find test/local -type f -name "${name}.cpp" -print0)
  if [ ${#files[@]} -eq 0 ]; then
    echo "No test file found for name: $name"
    exit 1
  fi
else
  # 查找 test/local 下的所有测试文件
  files=()
  while IFS= read -r -d '' file; do
    files+=("$file")
  done < <(find test/local -type f -name '*.cpp' -print0)
fi

# 编译测试文件
for testfile in "${files[@]}"; do
  base=$(basename "$testfile" .cpp)
  clang++ -std=c++20 -O2 \
    -I. \
    "$testfile" \
    third_party/catch/catch_amalgamated.cpp \
    -o build/"$base"
done

# 运行测试
for testfile in "${files[@]}"; do
  base=$(basename "$testfile" .cpp)
  exe="build/$base"
  echo "Running $exe"
  "$exe" --reporter compact
done
