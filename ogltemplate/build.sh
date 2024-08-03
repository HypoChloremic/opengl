#!/bin/bash

CC=g++
CFLAGS=`pkg-config --cflags glew`
LDFLAGS=`pkg-config --libs glew`
LDFLAGS="$LDFLAGS -lglut"

$CC main.cpp $CFLAGS $LDFLAGS -o progr
