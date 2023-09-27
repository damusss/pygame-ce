#!/bin/bash
set -e -x

cd $(dirname `readlink -f "$0"`)

SDL2="SDL2-2.26.5"

curl -sL --retry 10 https://www.libsdl.org/release/${SDL2}.tar.gz > ${SDL2}.tar.gz
sha512sum -c sdl2.sha512

tar xzf ${SDL2}.tar.gz

if [[ "$MAC_ARCH" == "arm64" ]]; then
    # Build SDL with ARM optimisations on M1 macs
    export M1_MAC_EXTRA_FLAGS="--enable-arm-simd --enable-arm-neon"
fi

cd $SDL2
./configure --disable-video-vulkan $ARCHS_CONFIG_FLAG $M1_MAC_EXTRA_FLAGS
make
make install

if [[ "$OSTYPE" == "darwin"* ]]; then
    # Install to mac deps cache dir as well
    make install DESTDIR=${MACDEP_CACHE_PREFIX_PATH}
fi
