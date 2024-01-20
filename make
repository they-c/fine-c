#!/bin/sh
cd $(dirname -- "$0")
mkdir -p build
cd build
gcc -I../include -o fine-c ../source/lexer.c ../source/main.c

#Tests
cp ../tests/test.c test.c
echo test.c:
./fine-c <test.c
