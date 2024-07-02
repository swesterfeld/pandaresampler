#!/bin/bash
set -Eeuo pipefail

test_run()
{
  DIR="build_$1"
  ninja -C"$DIR" install
  ldconfig

  $CXX -o testmultidelay tests/testmultidelay.cc `pkg-config --cflags --libs pandaresampler`
  ./testmultidelay >/dev/null
  $CXX -o testmultidelay tests/testmultidelay.cc `pkg-config --cflags pandaresampler` -DPANDA_RESAMPLER_HEADER_ONLY
  ./testmultidelay >/dev/null

  ninja -C"$DIR" uninstall
}

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
test_run release
build tests -Ddevel=true -Db_sanitize=address -Db_sanititze=undefined -Ddebug_cxx=true

# Tests clang
export CC=clang CXX=clang++

build clang_release
test_run clang_release
build clang_tests -Ddevel=true -Db_sanitize=address -Db_sanititze=undefined -Ddefault_library=static -Ddebug_cxx=true
