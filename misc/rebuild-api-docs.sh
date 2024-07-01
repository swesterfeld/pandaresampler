#!/bin/bash

set -e

VERSION="$1"
doxygen "${MESON_SOURCE_ROOT}/Doxyfile"
rsync -av html/ stefan@space.twc.de:public_html/pandaresampler/api-$VERSION
