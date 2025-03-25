#!/bin/sh

# Build for WASM inside a docker container.
# This is for convenience.
docker run \
  --rm \
  -v $(pwd):/zengine \
  -w /zengine \
  --entrypoint /bin/sh \
  emscripten/emsdk:4.0.4-arm64 \
  ./scripts/emscripten/build.sh
