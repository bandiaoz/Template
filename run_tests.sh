#!/usr/bin/env bash
set -euo pipefail

# 切换到脚本所在目录
cd "$(dirname "$0")"

# 编译所有测试
clang++ -std=c++20 -O2 \
  -I. \
  test/local/*.cpp \
  third_party/catch/catch_amalgamated.cpp \
  -o build/all_tests

# 运行测试
echo "Compiled successfully: build/all_tests"

