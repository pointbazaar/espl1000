FLAGS := -O0 -march=native -g -Wall

all: smalldragon

smalldragon: build/transpiler.o build/java_code_gen.o build/util.o build/ast_reader.o
	gcc $(FLAGS) -o sd build/*.o

build/ast_reader.o: ../ast/ast_reader.c
	gcc $(FLAGS) -c ../ast/ast_reader.c

build/util.o: ../util/util.c
	gcc $(FLAGS) -c ../util/util.c

build/transpiler.o: transpiler.c
	gcc $(FLAGS) -c transpiler.c
