#!/bin/sh

cmake --build build
if [ "$?" -eq "0" ]; then
    ./build/perception
fi
