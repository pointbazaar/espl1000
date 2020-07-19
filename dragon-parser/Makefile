FLAGS := -O0 -march=native -g -Wall

all: dragon-parser

dragon-parser: commandline parsing typenodes statements build/util.o tests
	mv *.o build/
	gcc $(FLAGS) -o dragon-parser build/*.o

build/util.o: ../util/util.c ../util/util.h
	gcc $(FLAGS) -c ../util/util.c

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
	
