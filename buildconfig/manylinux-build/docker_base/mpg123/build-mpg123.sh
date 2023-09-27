#!/bin/bash
set -e -x

cd $(dirname `readlink -f "$0"`)

MPG123="mpg123-1.32.1"

curl -sL --retry 10 https://downloads.sourceforge.net/sourceforge/mpg123/${MPG123}.tar.bz2 > ${MPG123}.tar.bz2
sha512sum -c mpg123.sha512

bzip2 -d ${MPG123}.tar.bz2
tar xf ${MPG123}.tar
cd $MPG123

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    # use sdl2, and only sdl2 backend
    export MPG123_EXTRA_ARGS="--with-audio=sdl --with-default-audio=sdl"
fi

./configure $ARCHS_CONFIG_FLAG --enable-int-quality --disable-debug $MPG123_EXTRA_ARGS
make
make install

if [[ "$OSTYPE" == "darwin"* ]]; then
    # Install to mac deps cache dir as well
    make install DESTDIR=${MACDEP_CACHE_PREFIX_PATH}
fi

cd ..
