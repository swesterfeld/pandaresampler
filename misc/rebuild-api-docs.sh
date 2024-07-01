#!/bin/bash

VERSION="$1"
cd "${MESON_SOURCE_ROOT}"
doxygen Doxyfile
rsync -av html/ stefan@space.twc.de:public_html/pandaresampler/api-$VERSION
