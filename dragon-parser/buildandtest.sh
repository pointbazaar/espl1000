#!/bin/bash

g++ --std=c++17 -o dragon-parser \
src/main/commandline/*.cpp \
src/main/commandline/*.hpp \
src/main/compiler/lexing/*.cpp \
src/main/compiler/lexing/*.hpp \
src/main/compiler/parsing/*.hpp \
src/main/compiler/parsing/astnodes/*.hpp \
src/main/compiler/parsing/astnodes/nonterminal/*.cpp \
src/main/compiler/parsing/astnodes/nonterminal/*.hpp \
src/main/compiler/parsing/astnodes/terminal/*.cpp \
src/main/compiler/parsing/astnodes/terminal/*.hpp \
src/main/compiler/parsing/astnodes/typenodes/*.cpp \
src/main/compiler/parsing/astnodes/typenodes/*.hpp

#copy the executable to /usr/bin
cp dragon-parser /usr/bin/

dragon-parser -test