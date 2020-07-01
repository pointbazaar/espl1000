#!/bin/bash

#run this script with root privileges
rm main
rm main-debug
rm test

make main
make main-debug
make test

#copy our executable
cp main /bin/dragon-lexer
cp main-debug /bin/dragon-lexer-debug
