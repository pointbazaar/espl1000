FLAGS := -O0 -march=native -g --std=c++17

#this is a very crude makefile.

all: dragon-parser

dragon-parser: commandline parsing typenodes statements
	g++ $(FLAGS) -o dragon-parser *.o

commandline: src/main/commandline/ASTWriter.cpp src/main/commandline/BaseToken.cpp src/main/commandline/Parser.cpp src/main/commandline/TokenList.cpp src/main/commandline/TokenReader.cpp
	g++ $(FLAGS) -c src/main/commandline/*.cpp

parsing: src/main/parsing/*.cpp
	g++ $(FLAGS) -c src/main/parsing/*.cpp

typenodes: src/main/parsing/typenodes/*.cpp
	g++ $(FLAGS) -c src/main/parsing/typenodes/*.cpp

statements: src/main/parsing/statements/*.cpp
	g++ $(FLAGS) -c src/main/parsing/statements/*.cpp
