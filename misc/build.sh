#!/bin/bash
set -Eeuo pipefail

build()
{
  if [ -f "./configure" ]; then
    make uninstall
    make distclean
  fi
  echo "###############################################################################"
  echo "# BUILD TESTS :"
  echo "#   CC=$CC CXX=$CXX "
  echo "#   ./autogen.sh $@"
  echo "###############################################################################"
  $CXX --version | sed '/^[[:space:]]*$/d;s/^/#   /'
  echo "###############################################################################"
  ./autogen.sh "$@"
  make -j `nproc` V=1
  if ! make -j `nproc` check; then
    for LOG in $(find tests -iname '*.log')
    do
      echo "===== $LOG ====="
      cat $LOG
    done
    exit 1
  fi
  make install
}

# Tests using gcc
export CC=gcc CXX=g++

build --with-tests --enable-asan --enable-debug-cxx

build --with-tests
make -j `nproc` distcheck

build

# Tests clang
export CC=clang CXX=clang++

build --with-tests

build
