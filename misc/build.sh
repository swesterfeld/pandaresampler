#!/bin/bash
set -Eeuo pipefail

build()
{
  DIR="build_$1"
  shift
  if [ -d "$DIR" ]; then
    rm -rf "$DIR"
  fi
  echo "###############################################################################"
  echo "# BUILD TESTS :"
  echo "#   CC=$CC CXX=$CXX DIR=$DIR"
  echo "###############################################################################"
  $CXX --version | sed '/^[[:space:]]*$/d;s/^/#   /'
  echo "###############################################################################"
  meson setup "$DIR" "$@"
  ninja -C"$DIR"
  ninja -C"$DIR" test
}

# Tests using gcc
export CC=gcc CXX=g++

build release
build tests -Ddevel=true -Db_sanitize=address

# Tests clang
export CC=clang CXX=clang++

build clang_release
build clang_tests -Ddevel=true -Db_sanitize=address -Ddefault_library=static
