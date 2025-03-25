#!/bin/sh

mkdir -p web-build
cd web-build
emcmake cmake ..
make -j12
