#!/bin/bash

# if $1 is "clean", remove build directory
if [ "$1" == "clean" ]; then
    if [ -d "build" ]; then
        rm -rf build
    fi
    exit 0
fi

# if build directory does not exist, create it
if [ ! -d "build" ]; then
    mkdir build
fi

cd build
cmake -DCMAKE_BUILD_TYPE=Release -DWXT_GENERATE_MSIX:BOOL=OFF -G "Ninja" ../../../
cmake --build . --target all
cmake --build . --target wxTools-deb
tree deb/wxTools
cmake --build . --target wxTools-appimage
tree appimage/wxTools