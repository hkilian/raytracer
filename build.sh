#!/bin/bash

em++ ./main.cpp -s USE_SDL=2 -std=c++11 -o ./emscripten/main.bc
em++ ./TextureImage.cpp -s USE_SDL=2 -std=c++11 -o ./emscripten/TextureImage.bc
em++ ./Vec3.cpp -s USE_SDL=2 -std=c++11 -o ./emscripten/Vec3.bc
em++ ./emscripten/main.bc ./emscripten/TextureImage.bc ./emscripten/Vec3.bc -s USE_SDL=2 --shell-file ./emscripten/source/shell.html -o ./emscripten/out.html