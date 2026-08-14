#!/usr/bin/env bash

set -euo pipefail

repo_root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
build_jobs=${BUILD_JOBS:-2}
remove_build_root=false

if [[ -z ${BUILD_ROOT:-} ]]; then
  BUILD_ROOT=$(mktemp -d)
  remove_build_root=true
else
  mkdir -p -- "$BUILD_ROOT"
fi

cleanup() {
  if [[ $remove_build_root == true ]]; then
    rm -rf -- "$BUILD_ROOT"
  fi
}
trap cleanup EXIT

build_cmake() {
  local name=$1
  local source=$2
  local build="$BUILD_ROOT/$name"

  echo "==> $name"
  cmake -S "$repo_root/$source" -B "$build" -DCMAKE_BUILD_TYPE=Release
  cmake --build "$build" --parallel "$build_jobs"
  if [[ -f "$build/CTestTestfile.cmake" ]]; then
    ctest --test-dir "$build" --output-on-failure
  fi
}

echo "==> lab1-1"
echo "SKIP: the historical P5 bootstrap toolchain is not bundled as an executable environment"

build_cmake lab1-2 lab1-2
build_cmake lab1-3 lab1-3
build_cmake lab1-4 lab1-4
build_cmake lab1-5 lab1-5/src
build_cmake lab2-3 lab2-3/src
build_cmake lab2-4 lab2-4/src
build_cmake lab3-1-generator lab3-1/apps/generator/src
build_cmake lab3-1-calculator lab3-1/apps/calculator/src
build_cmake lab3-2 lab3-2/src

echo "==> lab2-1"
(
  cd "$repo_root/lab2-1"
  go build ./...
  go test ./...
)

echo "==> Python labs"
export PYTHONPYCACHEPREFIX="$BUILD_ROOT/pycache"
python3 -m py_compile \
  "$repo_root/lab2-2/sample/main.py" \
  "$repo_root/lab2-2/sample/parser_edsl.py" \
  "$repo_root/lab3-3/sample/main.py" \
  "$repo_root/lab3-3/sample/parser_edsl.py"
python3 "$repo_root/lab2-2/sample/main.py" \
  "$repo_root/lab2-2/examples/program.txt" >/dev/null
python3 "$repo_root/lab3-3/sample/main.py" \
  "$repo_root/lab3-3/input/example.txt" >/dev/null

echo "All available checks passed."
