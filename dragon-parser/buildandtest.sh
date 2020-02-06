#!/bin/bash

# do not use .hpp's in g++ call

g++ --std=c++17 -o dragon-parser \
src/main/commandline/BaseToken.cpp \
src/main/commandline/TokenList.cpp \
src/main/commandline/TokenReader.cpp \
src/main/parsing/CharConstNode.cpp \
src/main/parsing/IntConstNode.cpp \
src/main/parsing/BoolConstNode.cpp \
src/main/parsing/IdentifierNode.cpp \
src/main/parsing/OperatorNode.cpp \
src/main/parsing/TermNode.cpp \
src/main/parsing/ExpressionNode.cpp \
src/main/parsing/DeclaredArgumentNode.cpp \
src/main/parsing/VariableNode.cpp \
src/main/parsing/SimpleVariableNode.cpp \
src/main/parsing/statements/MethodCallNode.cpp \
src/main/parsing/typenodes/*.cpp \
#src/main/parsing/statements/*.hpp 


#copy the executable to /usr/bin
#cp dragon-parser /usr/bin/

#dragon-parser -test