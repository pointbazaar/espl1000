#it had -O0 before, but i dont think we need it
#unless debugging becomes hard when omitting it.

INCLUDE := -Imain -I..

FLAGS := -march=native -g --std=c11 -Wall $(INCLUDE)

CALL := gcc $(FLAGS)

CALLTEST := $(CALL) -Itest

AST_OBJS := ../ast/build/ast_reader.o \
		../ast/build/free_ast.o \
		../ast/build/str_ast.o \
		../ast/build/serialize.o 

TRANSPILER_OBJS := build/transpiler.o \
	build/c_code_gen.o \
	build/structs_code_gen.o \
	build/code_gen_util.o \
	build/test.o \
	build/test_statuscode.o \
	build/test_other.o \
	build/test_op.o \
	build/test_assign.o \
	build/test_stmt.o \
	build/flags.o \
	build/localvar_symtable.o \
	build/subr_symtable.o \
	build/struct_symtable.o \
	build/typeinference.o \
	build/help.o \
	build/gen_c_types.o \
	build/type_str.o

OBJS := $(AST_OBJS) $(TRANSPILER_OBJS);

all: all_no_valgrind valgrind

all_no_valgrind: smalldragon test

clean: 
	rm -f build/*.o

test: sd
	./sd -test

VALGRIND_OPS := --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 --log-file="/dev/null"

valgrind: sd
	valgrind $(VALGRIND_OPS) ./sd -clean ../examples/other/Everything.dg

smalldragon: dir $(OBJS)
	$(CALL) -o sd $(OBJS)
	
dir:
	mkdir -p build

build/flags.o: main/util/flags.c
	$(CALL) -c main/util/flags.c -o $@

build/ast_reader.o: ../ast/ast_reader.c
	$(CALL) -c ../ast/ast_reader.c -o $@

build/free_ast.o: ../ast/free_ast.c ../ast/free_ast.h
	$(CALL) -c ../ast/free_ast.c -o $@

build/serialize.o: ../ast/serialize.c ../ast/serialize.h
	$(CALL) -c ../ast/serialize.c -o $@

build/util.o: ../util/util.c
	$(CALL) -c ../util/util.c -o $@

build/transpiler.o: main/transpiler.c main/transpiler.h main/code_gen/c_code_gen.h main/util/flags.h test/test.h main/util/help.h 
	$(CALL) -c main/transpiler.c -o $@

build/c_code_gen.o: main/code_gen/c_code_gen.c main/util/ctx.h \
		main/code_gen/code_gen_util.h main/util/flags.h main/tables/localvar_symtable.h \
		main/code_gen/structs_code_gen.h
	$(CALL) -c main/code_gen/c_code_gen.c -o $@

build/code_gen_util.o: main/code_gen/code_gen_util.c main/code_gen/code_gen_util.h main/util/ctx.h
	$(CALL) -c main/code_gen/code_gen_util.c -o $@

build/structs_code_gen.o: main/code_gen/structs_code_gen.c main/code_gen/structs_code_gen.h main/util/ctx.h main/util/flags.h
	$(CALL) -c main/code_gen/structs_code_gen.c -o $@

build/test.o: test/test.c test/test.h \
		main/transpiler.h main/util/flags.h \
		test/util/test_statuscode.h \
		test/code_gen/test_op.h \
		test/code_gen/test_other.h \
		test/code_gen/test_assign.h \
		test/typeinference/test_typeinference.h
	$(CALLTEST) -c test/test.c -o $@

build/test_statuscode.o: test/util/test_statuscode.h test/util/test_statuscode.c main/util/flags.h main/transpiler.h
	$(CALLTEST) -c test/util/test_statuscode.c -o $@
	
build/test_assign.o: test/util/test_statuscode.h test/util/test_statuscode.c test/code_gen/test_assign.h test/code_gen/test_assign.c
	$(CALLTEST) -c test/code_gen/test_assign.c -o $@

build/test_other.o: test/util/test_statuscode.h test/util/test_statuscode.c test/code_gen/test_other.h test/code_gen/test_other.c
	$(CALLTEST) -c test/code_gen/test_other.c -o $@
	
build/test_op.o: test/util/test_statuscode.h test/util/test_statuscode.c test/code_gen/test_op.h test/code_gen/test_op.c
	$(CALLTEST) -c test/code_gen/test_op.c -o $@
	
build/test_stmt.o: test/util/test_statuscode.h test/util/test_statuscode.c test/code_gen/test_stmt.h test/code_gen/test_stmt.c
	$(CALLTEST) -c test/code_gen/test_stmt.c -o $@

SYMTABLES_H := main/tables/symtable.h main/tables/subr_symtable.h \
			main/tables/localvar_symtable.h main/tables/struct_symtable.h

build/typeinference.o: $(SYMTABLES_H) main/typeinference/typeinference.c main/typeinference/typeinference.h 
	$(CALL) -c main/typeinference/typeinference.c -o $@
	
build/type_str.o: $(SYMTABLES_H) main/typeinference/type_str.c main/typeinference/type_str.h
	$(CALL) -c main/typeinference/type_str.c -o $@

build/help.o: main/util/help.c main/util/help.h
	$(CALL) -c main/util/help.c -o $@

build/gen_c_types.o: main/code_gen/gen_c_types.h main/code_gen/gen_c_types.c 
	$(CALL) -c main/code_gen/gen_c_types.c -o $@

# ----------- the symbol tables --------------

build/subr_symtable.o: main/tables/subr_symtable.c main/tables/subr_symtable.h
	$(CALL) -c main/tables/subr_symtable.c -o $@

build/localvar_symtable.o: main/tables/localvar_symtable.c main/tables/localvar_symtable.h
	$(CALL) -c main/tables/localvar_symtable.c -o $@

build/struct_symtable.o: main/tables/struct_symtable.c main/tables/struct_symtable.h
	$(CALL) -c main/tables/struct_symtable.c -o $@
