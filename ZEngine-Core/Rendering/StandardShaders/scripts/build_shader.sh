#!/bin/bash

../../../build/3rdparty/bgfx.cmake/shaderc \
  -f ${1}.frag.sc \
  --type fragment \
  --platform osx \
  --profile metal \
  --varying-def varying.def.sc \
  --bin2c ${2}_FRAGMENT_SHADER \
  -i ./ \
  -o ./${1}.frag.h

../../../build/3rdparty/bgfx.cmake/shaderc \
  -f ${1}.vert.sc \
  --type vertex \
  --platform osx \
  --profile metal \
  --varying-def varying.def.sc \
  --bin2c ${2}_VERTEX_SHADER \
  -i ./ \
  -o ./${1}.vert.h
