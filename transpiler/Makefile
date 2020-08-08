#it had -O0 before, but i dont think we need it
#unless debugging becomes hard when omitting it.
FLAGS := -march=native -g -Wall

CALL := gcc $(FLAGS)

all: smalldragon

smalldragon: build/transpiler.o build/java_code_gen.o build/util.o build/ast_reader.o build/c_code_gen.o build/code_gen_util.o
	$(CALL) -o sd build/*.o

build/ast_reader.o: ../ast/ast_reader.c
	$(CALL) -c ../ast/ast_reader.c
	mv *.o build/

build/util.o: ../util/util.c
	$(CALL) -c ../util/util.c
	mv *.o build/

build/transpiler.o: main/transpiler.c
	$(CALL) -c main/transpiler.c
	mv *.o build/

build/c_code_gen.o: main/c_code_gen.c
	$(CALL) -c main/c_code_gen.c
	mv *.o build/

build/code_gen_util.o: main/code_gen_util.c
	$(CALL) -c main/code_gen_util.c
	mv *.o build/
