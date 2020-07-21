FLAGS := -O0 -march=native -g -Wall

all: smalldragon

smalldragon: build/transpiler.o build/java_code_gen.o build/util.o build/ast_reader.o
	gcc $(FLAGS) -o sd build/*.o

build/ast_reader.o: ../ast/ast_reader.c
	gcc $(FLAGS) -c ../ast/ast_reader.c
	mv ast_reader.o build/

build/util.o: ../util/util.c
	gcc $(FLAGS) -c ../util/util.c
	mv util.o build/

build/transpiler.o: main/transpiler.c
	gcc $(FLAGS) -c main/transpiler.c
	mv transpiler.o build/
