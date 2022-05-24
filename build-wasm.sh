#!/usr/bin/env sh
emcc \
    -g -gsource-map --source-map-base=/ \
    -s WASM=1 \
    -s USE_SDL=2 \
    -sDISABLE_EXCEPTION_CATCHING=0 \
    --preload-file PIXELS.DEF \
    -I ./include \
    source/input.cpp \
    source/global.cpp \
    source/fast3d.cpp \
    source/text3d.cpp \
    source/cryxtels.cpp \
    -o cryxtels.html
