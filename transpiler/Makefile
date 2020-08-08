#it had -O0 before, but i dont think we need it
#unless debugging becomes hard when omitting it.
FLAGS := -march=native -g -Wall

CALL := gcc $(FLAGS)

OBJS := build/transpiler.o build/util.o build/ast_reader.o build/c_code_gen.o build/code_gen_util.o

all: smalldragon

smalldragon: $(OBJS)
	$(CALL) -o sd build/*.o

build/ast_reader.o: ../ast/ast_reader.c
	$(CALL) -c ../ast/ast_reader.c -o $@

build/util.o: ../util/util.c
	$(CALL) -c ../util/util.c -o $@

build/transpiler.o: main/transpiler.c main/transpiler.h main/c_code_gen.h
	$(CALL) -c main/transpiler.c -o $@

build/c_code_gen.o: main/c_code_gen.c main/code_gen_util.h
	$(CALL) -c main/c_code_gen.c -o $@

build/code_gen_util.o: main/code_gen_util.c main/code_gen_util.h
	$(CALL) -c main/code_gen_util.c -o $@
