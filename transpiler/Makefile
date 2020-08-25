#it had -O0 before, but i dont think we need it
#unless debugging becomes hard when omitting it.
FLAGS := -march=native -g -Wall

CALL := gcc $(FLAGS)

OBJS := build/transpiler.o build/util.o build/ast_reader.o \
		build/c_code_gen.o build/code_gen_util.o build/test.o \
		build/teststatuscode.o \
		build/test_other.o \
		build/test_op.o \
		build/flags.o

all: smalldragon test

clean: 
	rm -f build/*.o

test: sd
	./sd -test

smalldragon: dir $(OBJS)
	$(CALL) -o sd build/*.o
	
dir:
	mkdir -p build

build/flags.o: main/flags.c
	$(CALL) -c main/flags.c -o $@

build/ast_reader.o: ../ast/ast_reader.c
	$(CALL) -c ../ast/ast_reader.c -o $@

build/util.o: ../util/util.c
	$(CALL) -c ../util/util.c -o $@

build/transpiler.o: main/transpiler.c main/transpiler.h main/c_code_gen.h main/flags.h test/test.h
	$(CALL) -c main/transpiler.c -o $@

build/c_code_gen.o: main/c_code_gen.c main/code_gen_util.h main/flags.h
	$(CALL) -c main/c_code_gen.c -o $@

build/code_gen_util.o: main/code_gen_util.c main/code_gen_util.h
	$(CALL) -c main/code_gen_util.c -o $@

build/test.o: test/test.c test/test.h main/transpiler.h main/flags.h test/teststatuscode.h 
	$(CALL) -c test/test.c -o $@

build/teststatuscode.o: test/teststatuscode.h test/teststatuscode.c main/flags.h main/transpiler.h ../util/util.c ../util/util.h
	$(CALL) -c test/teststatuscode.c -o $@
	
build/test_other.o: test/teststatuscode.h test/teststatuscode.c test/suite/test_other.h test/suite/test_other.c
	$(CALL) -c test/suite/test_other.c -o $@
	
build/test_op.o: test/teststatuscode.h test/teststatuscode.c test/suite/test_op.h test/suite/test_op.c
	$(CALL) -c test/suite/test_op.c -o $@
