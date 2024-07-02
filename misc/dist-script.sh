#!/bin/sh

set -e

meson setup $MESON_SOURCE_ROOT builddir-dist-script -Ddevel=true
meson compile -C builddir-dist-script
meson test -C builddir-dist-script
rm -rf builddir-dist-script
