#!/bin/bash

CC=g++
CFLAGS=`pkg-config --cflags glew glfw3`
LDFLAGS=`pkg-config --libs glew glfw3`
LDFLAGS="$LDFLAGS -lglut"

$CC main.cpp ./common/loadShader.cpp $CFLAGS $LDFLAGS -o progr
