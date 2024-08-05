#!/bin/bash

CC=g++
CFLAGS=`pkg-config --cflags glew glfw3`
LDFLAGS=`pkg-config --libs glew glfw3`
LDFLAGS="$LDFLAGS -lglut -Wall"

# Include directory for header files
INCLUDE_DIR="-I./include"

mkdir -p build

$CC $INCLUDE_DIR ./src/main.cpp ./src/controls.cpp ./src/buffering.cpp ./src/file_handling.cpp ./src/cleaning.cpp ./common/loadShader.cpp $CFLAGS $LDFLAGS -o ./build/progr
