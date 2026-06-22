#!/usr/bin/env bash
set -euo pipefail

# 切换到脚本所在目录
cd "$(dirname "$0")"

mkdir -p build

CXX=${CXX:-clang++}
CXXFLAGS=(-std=c++20 -O2 -I.)
CATCH_SRC=third_party/catch/catch_amalgamated.cpp
CATCH_OBJ=build/catch_amalgamated.o
CATCH_DEP=build/catch_amalgamated.d

needs_rebuild() {
  local target="$1"
  local depfile="$2"

  if [ ! -e "$target" ] || [ ! -f "$depfile" ] || [ "$0" -nt "$target" ]; then
    return 0
  fi

  local deps
  deps=$(tr '\\\n' '  ' < "$depfile" | sed -e 's/^[^:]*: //')
  for dep in $deps; do
    if [[ "$dep" == *: ]]; then
      continue
    fi
    if [ -f "$dep" ] && [ "$dep" -nt "$target" ]; then
      return 0
    fi
  done
  return 1
}

if needs_rebuild "$CATCH_OBJ" "$CATCH_DEP"; then
  echo "Building $CATCH_OBJ"
  "$CXX" "${CXXFLAGS[@]}" -MMD -MP -MF "$CATCH_DEP" -c "$CATCH_SRC" -o "$CATCH_OBJ"
fi

# 支持局部测试：传入一个或多个测试名时，只编译/运行这些测试
if [ $# -gt 0 ]; then
  files=()
  for name in "$@"; do
    while IFS= read -r -d '' file; do
      files+=("$file")
    done < <(find test/local -type f -name "${name}.cpp" -print0)
  done
  if [ ${#files[@]} -eq 0 ]; then
    echo "No test file found for: $*"
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
  exe="build/$base"
  depfile="build/$base.d"
  if needs_rebuild "$exe" "$depfile" || [ "$CATCH_OBJ" -nt "$exe" ]; then
    echo "Building $exe"
    "$CXX" "${CXXFLAGS[@]}" -MMD -MP -MF "$depfile" "$testfile" "$CATCH_OBJ" -o "$exe"
  else
    echo "Up to date $exe"
  fi
done

# 运行测试
for testfile in "${files[@]}"; do
  base=$(basename "$testfile" .cpp)
  exe="build/$base"
  echo "Running $exe"
  "$exe" --reporter compact
done
