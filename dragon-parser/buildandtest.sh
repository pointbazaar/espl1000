#!/bin/bash

# do not use .hpp's in g++ call

g++ -O0 --std=c++17 -o dragon-parser \
src/main/commandline/Parser.cpp \
src/main/commandline/BaseToken.cpp \
src/main/commandline/TokenList.cpp \
src/main/commandline/TokenReader.cpp \
src/main/parsing/*.cpp \
src/main/parsing/statements/*cpp \
src/main/parsing/typenodes/*.cpp \


#copy the executable to /usr/bin
cp dragon-parser /usr/bin/

#dragon-parser -test