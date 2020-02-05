#!/bin/bash

g++ --std=c++17 -o dragon-parser \
src/main/commandline/*.cpp \
src/main/commandline/*.hpp \
src/main/parsing/*.hpp \
src/main/parsing/*.cpp \
src/main/parsing/statements/*.cpp \
src/main/parsing/statements/*.hpp \
src/main/parsing/typenodes/*.cpp \
src/main/parsing/typenodes/*.hpp

#copy the executable to /usr/bin
#cp dragon-parser /usr/bin/

#dragon-parser -test