FLAGS := -O0 -march=native -g
COMPILER := gcc

#this is a very crude makefile.

all: dragon-parser

clean: 
	rm *.o

dragon-parser: commandline parsing typenodes statements tests
	$(COMPILER) $(FLAGS) -o dragon-parser *.o

commandline: src/main/commandline/*.c
	$(COMPILER) $(FLAGS) -c src/main/commandline/*.c

parsing: src/main/parsing/*.c
	$(COMPILER) $(FLAGS) -c src/main/parsing/*.c

typenodes: src/main/parsing/typenodes/*.c
	$(COMPILER) $(FLAGS) -c src/main/parsing/typenodes/*.c

statements: src/main/parsing/statements/*.c
	$(COMPILER) $(FLAGS) -c src/main/parsing/statements/*.c

tests:	src/test/ParserTest.c src/test/astnodes/*.c
	$(COMPILER) $(FLAGS) -c src/test/ParserTest.c src/test/astnodes/*.c
	
