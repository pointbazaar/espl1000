#it had -O0 before, but i dont think we need it
#unless debugging becomes hard when omitting it.
FLAGS := -march=native -g --std=c11 -Wall -Werror -Dctime="\"$$(date)\"" -Dgccversion="\"$$(gcc -dumpfullversion)\""

CALL := gcc-10 $(FLAGS)

OBJS := build/transpiler.o \
	build/util.o \
	build/ast_reader.o \
	build/c_code_gen.o \
	build/structs_code_gen.o \
	build/code_gen_util.o \
	build/test.o \
	build/teststatuscode.o \
	build/test_other.o \
	build/test_op.o \
	build/test_other.o \
	build/test_assign.o \
	build/flags.o \
	build/localvarsymtable.o \
	build/subrsymtable.o \
	build/structsymtable.o \
	build/typeinference.o \
	build/help.o \
	build/free_ast.o \
	build/gen_c_types.o

all: smalldragon test valgrind

clean: 
	rm -f build/*.o

test: sd
	./sd -test

VALGRIND_OPS := --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 --log-file="/dev/null"

valgrind: sd
	valgrind $(VALGRIND_OPS) ./sd -clean ../examples/Everything.dg

smalldragon: dir $(OBJS)
	$(CALL) -o sd build/*.o
	
dir:
	mkdir -p build

build/flags.o: main/flags.c
	$(CALL) -c main/flags.c -o $@

build/ast_reader.o: ../ast/ast_reader.c
	$(CALL) -c ../ast/ast_reader.c -o $@

build/free_ast.o: ../ast/free_ast.c ../ast/free_ast.h
	$(CALL) -c ../ast/free_ast.c -o $@

build/util.o: ../util/util.c
	$(CALL) -c ../util/util.c -o $@

build/transpiler.o: main/transpiler.c main/transpiler.h main/code_gen/c_code_gen.h main/flags.h test/test.h main/help.h 
	$(CALL) -c main/transpiler.c -o $@

build/c_code_gen.o: main/code_gen/c_code_gen.c main/ctx.h \
		main/code_gen/code_gen_util.h main/flags.h main/tables/localvarsymtable.h \
		main/code_gen/structs_code_gen.h
	$(CALL) -c main/code_gen/c_code_gen.c -o $@

build/code_gen_util.o: main/code_gen/code_gen_util.c main/code_gen/code_gen_util.h main/ctx.h
	$(CALL) -c main/code_gen/code_gen_util.c -o $@

build/structs_code_gen.o: main/code_gen/structs_code_gen.c main/code_gen/structs_code_gen.h main/ctx.h main/flags.h
	$(CALL) -c main/code_gen/structs_code_gen.c -o $@

build/test.o: test/test.c test/test.h \
		main/transpiler.h main/flags.h \
		test/teststatuscode.h \
		test/suite/test_op.h \
		test/suite/test_other.h \
		test/suite/test_assign.h \
		test/suite/test_typeinference.h
	$(CALL) -c test/test.c -o $@

build/teststatuscode.o: test/teststatuscode.h test/teststatuscode.c main/flags.h main/transpiler.h ../util/util.c ../util/util.h
	$(CALL) -c test/teststatuscode.c -o $@
	
build/test_assign.o: test/teststatuscode.h test/teststatuscode.c test/suite/test_assign.h test/suite/test_assign.c
	$(CALL) -c test/suite/test_assign.c -o $@

build/test_other.o: test/teststatuscode.h test/teststatuscode.c test/suite/test_other.h test/suite/test_other.c
	$(CALL) -c test/suite/test_other.c -o $@
	
build/test_op.o: test/teststatuscode.h test/teststatuscode.c test/suite/test_op.h test/suite/test_op.c
	$(CALL) -c test/suite/test_op.c -o $@

build/typeinference.o: main/typeinference.c main/typeinference.h \
			main/tables/symtable.h main/tables/subrsymtable.h \
			main/tables/localvarsymtable.h main/tables/structsymtable.h
	$(CALL) -c main/typeinference.c -o $@

build/help.o: main/help.c main/help.h
	$(CALL) -c main/help.c -o $@

build/gen_c_types.o: main/code_gen/gen_c_types.h main/code_gen/gen_c_types.c 
	$(CALL) -c main/code_gen/gen_c_types.c -o $@

# ----------- the symbol tables --------------

build/subrsymtable.o: main/tables/subrsymtable.c main/tables/subrsymtable.h
	$(CALL) -c main/tables/subrsymtable.c -o $@

build/localvarsymtable.o: main/tables/localvarsymtable.c main/tables/localvarsymtable.h
	$(CALL) -c main/tables/localvarsymtable.c -o $@

build/structsymtable.o: main/tables/structsymtable.c main/tables/structsymtable.h
	$(CALL) -c main/tables/structsymtable.c -o $@
