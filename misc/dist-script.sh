#!/bin/sh

set -e

meson setup builddir -Ddevel=true
meson compile -C builddir
meson test -C builddir
