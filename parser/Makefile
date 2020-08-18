#it had -O0 before
FLAGS := -march=native -g -Wall
CALL := gcc $(FLAGS)

all: dragon-parser

dragon-parser: commandline parsing typenodes statements build/util.o build/ASTWriter.o tests
	mkdir -p build/
	mv *.o build/
	$(CALL) -o dragon-parser build/*.o

build/util.o: 
	$(CALL) -c ../util/util.c

build/ASTWriter.o: 
	$(CALL) -c ../ast/ASTWriter.c

commandline: src/main/commandline/*.c ../ast/*.c
	$(CALL) -c src/main/commandline/*.c ../ast/*.c

parsing: src/main/parsing/*.c
	$(CALL) -c src/main/parsing/*.c

typenodes: src/main/parsing/typenodes/*.c
	$(CALL) -c src/main/parsing/typenodes/*.c

statements: src/main/parsing/statements/*.c
	$(CALL) -c src/main/parsing/statements/*.c

tests:	src/test/ParserTest.c src/test/astnodes/*.c src/test/commandline/*.c
	$(CALL) -c src/test/ParserTest.c src/test/astnodes/*.c src/test/commandline/*.c
	
