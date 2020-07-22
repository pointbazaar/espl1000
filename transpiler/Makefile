FLAGS := -O0 -march=native -g -Wall

all: smalldragon

smalldragon: build/transpiler.o build/java_code_gen.o build/util.o build/ast_reader.o build/c_code_gen.o
	gcc $(FLAGS) -o sd build/*.o

build/ast_reader.o: ../ast/ast_reader.c
	gcc $(FLAGS) -c ../ast/ast_reader.c
	mv *.o build/

build/util.o: ../util/util.c
	gcc $(FLAGS) -c ../util/util.c
	mv *.o build/

build/transpiler.o: main/transpiler.c
	gcc $(FLAGS) -c main/transpiler.c
	mv *.o build/

build/c_code_gen.o: main/c_code_gen.c
	gcc $(FLAGS) -c main/c_code_gen.c
	mv *.o build/
