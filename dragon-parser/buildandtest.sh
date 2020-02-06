#!/bin/bash

g++ --std=c++17 -o dragon-parser \
src/main/commandline/BaseToken.cpp \
src/main/commandline/BaseToken.hpp \
src/main/commandline/Parser.cpp \
src/main/commandline/Parser.hpp \
src/main/commandline/TokenKeys.hpp \
src/main/commandline/TokenList.cpp \
src/main/commandline/TokenList.hpp \
src/main/commandline/TokenReader.cpp \
src/main/commandline/TokenReader.hpp \
#src/main/parsing/*.hpp \
#src/main/parsing/*.cpp \
#src/main/parsing/statements/*.cpp \
#src/main/parsing/statements/*.hpp \
#src/main/parsing/typenodes/*.cpp \
#src/main/parsing/typenodes/*.hpp

#copy the executable to /usr/bin
#cp dragon-parser /usr/bin/

#dragon-parser -test