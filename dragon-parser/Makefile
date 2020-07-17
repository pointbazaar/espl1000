FLAGS := -O0 -march=native -g -Wall

all: dragon-parser

clean: 
	rm *.o

dragon-parser: commandline parsing typenodes statements util tests
	gcc $(FLAGS) -o dragon-parser *.o

util: ../util/*.c
	gcc $(FLAGS) -c ../util/*.c

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
	
