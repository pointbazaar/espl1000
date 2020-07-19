FLAGS := -O0 -march=native -g -Wall

all: dragon-parser

dragon-parser: commandline parsing typenodes statements build/util.o build/ASTWriter.o tests
	mv *.o build/
	gcc $(FLAGS) -o dragon-parser build/*.o

build/util.o: 
	gcc $(FLAGS) -c ../util/util.c

build/ASTWriter.o: 
	gcc $(FLAGS) -c ../ast/ASTWriter.c

commandline: src/main/commandline/*.c ../ast/*.c
	gcc $(FLAGS) -c src/main/commandline/*.c ../ast/*.c

parsing: src/main/parsing/*.c
	gcc $(FLAGS) -c src/main/parsing/*.c

typenodes: src/main/parsing/typenodes/*.c
	gcc $(FLAGS) -c src/main/parsing/typenodes/*.c

statements: src/main/parsing/statements/*.c
	gcc $(FLAGS) -c src/main/parsing/statements/*.c

tests:	src/test/ParserTest.c src/test/astnodes/*.c src/test/commandline/*.c
	gcc $(FLAGS) -c src/test/ParserTest.c src/test/astnodes/*.c src/test/commandline/*.c
	
