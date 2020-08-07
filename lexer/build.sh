#!/bin/bash

#run this script with root privileges
rm main main-debug test

mkdir -p build/release
mkdir -p build/debug

make main
make main-debug
make test

#copy our executable
cp main /bin/dragon-lexer
cp main-debug /bin/dragon-lexer-debug
