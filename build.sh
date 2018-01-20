#!/bin/bash

em++ ./main.cpp -s USE_SDL=2 -o ./emscripten/main.bc
em++ ./TextureImage.cpp -s USE_SDL=2 -o ./emscripten/TextureImage.bc
em++ ./emscripten/main.bc ./emscripten/TextureImage.bc -s USE_SDL=2 -o ./emscripten/out.html