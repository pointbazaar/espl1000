FLAGS := -O0 -march=native -g -Wall

all: smalldragon

smalldragon: build/transpiler.o build/java_code_gen.o build/util.o build/ast_reader.o
	gcc $(FLAGS) -o sd build/*.o

build/ast_reader.o:
	gcc $(FLAGS) -c ../ast/ast_reader.c

build/util.o:
	gcc $(FLAGS) -c ../util/util.c

build/java_code_gen.o:
	gcc $(FLAGS) -c java_code_gen.c

build/transpiler.o:
	gcc $(FLAGS) -c transpiler.c
