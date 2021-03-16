#it had -O0 before, but i dont think we need it
#unless debugging becomes hard when omitting it.

INCLUDE := -Imain -I..

FLAGS := -march=native -g --std=c11 -Wall -Werror $(INCLUDE)

CALL := gcc $(FLAGS)

CALLTEST := $(CALL) -Itest

AST_OBJS := ../ast/build/ast_reader.o \
		../ast/build/free_ast.o \
		../ast/build/str_ast.o \
		../ast/build/serialize.o \
		../ast/build/visitor.o

TRANSPILER_OBJS := build/transpiler.o \
	build/invoke.o \
	build/flags.o \
	build/validate_flags.o \
	build/help.o \
	build/fileutils.o 
	
OBJS_CODEGEN := build/c_code_gen.o \
	build/structs_code_gen.o \
	build/code_gen_util.o \
	build/c_code_gen_const.o \
	build/c_code_gen_expr.o \
	build/c_code_gen_stmts.o \
	build/c_code_gen_struct.o \
	build/c_code_gen_subr.o \
	build/c_code_gen_types.o \
	build/c_code_gen_var.o \
	build/gen_c_types.o 
	
OBJS_ANALYZER := build/lv_analyzer.o \
	build/fn_analyzer.o \
	build/dead_analyzer.o \
	build/halt_analyzer.o
	
OBJS_TYPEINFERENCE := build/typeinfer.o \
	build/typeinfer_expr.o \
	build/typeinfer_simplevar.o \
	build/typeinfer_term.o \
	build/typeinfer_var.o \
	build/typeinfer_methodcall.o \
	build/type_str.o
	
OBJS_TEST := build/test.o \
	build/test_statuscode.o \
	build/test_other.o \
	build/test_op.o \
	build/test_assign.o \
	build/test_stmt.o \
	build/test_typeinference.o
	
OBJS_TABLE := ../tables/build/lvst.o \
	../tables/build/sst.o \
	../tables/build/sst_prefill.o \
	../tables/build/stst.o \
	../tables/build/symtable.o \
	../tables/build/cc.o

OBJS := $(AST_OBJS) $(TRANSPILER_OBJS) \
	$(OBJS_TABLE) $(OBJS_TEST) \
	$(OBJS_TYPEINFERENCE) $(OBJS_ANALYZER) \
	$(OBJS_CODEGEN)

all: all_no_valgrind valgrind

all_no_valgrind: smalldragon test

clean: 
	rm -f build/*.o
	rm -f a.out

test: sd
	./sd -test

VALGRIND_OPS := --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 --log-file="/dev/null"

valgrind: sd
	valgrind $(VALGRIND_OPS) ./sd -clean ../examples/other/Everything.dg

smalldragon: dir $(OBJS)
	$(CALL) -o sd $(OBJS)
	
dir:
	mkdir -p build

build/flags.o: main/flags/flags.c
	$(CALL) -c main/flags/flags.c -o $@

build/validate_flags.o: main/flags/validate_flags.c
	$(CALL) -c main/flags/validate_flags.c -o $@

build/ast_reader.o: ../ast/ast_reader.c
	$(CALL) -c ../ast/ast_reader.c -o $@

build/free_ast.o: ../ast/free_ast.c ../ast/free_ast.h
	$(CALL) -c ../ast/free_ast.c -o $@

build/serialize.o: ../ast/serialize.c ../ast/serialize.h
	$(CALL) -c ../ast/serialize.c -o $@

build/util.o: ../util/util.c
	$(CALL) -c ../util/util.c -o $@

build/transpiler.o: main/transpiler.c main/transpiler.h test/test.h main/util/help.h 
	$(CALL) -c main/transpiler.c -o $@

build/invoke.o: main/invoke/invoke.c main/invoke/invoke.h 
	$(CALL) -c main/invoke/invoke.c -o $@
	
# --- code_gen ---
build/c_code_gen.o: main/code_gen/c_code_gen/c_code_gen.c 
	$(CALL) -c main/code_gen/c_code_gen/c_code_gen.c -o $@

DIR_CG := main/code_gen/c_code_gen/

build/c_code_gen_const.o: $(DIR_CG)/const/c_code_gen_const.c 
	$(CALL) -c $(DIR_CG)/const/c_code_gen_const.c -o $@

build/c_code_gen_expr.o: $(DIR_CG)/expr/c_code_gen_expr.c 
	$(CALL) -c $(DIR_CG)/expr/c_code_gen_expr.c -o $@

build/c_code_gen_stmts.o: $(DIR_CG)/stmts/c_code_gen_stmts.c 
	$(CALL) -c $(DIR_CG)/stmts/c_code_gen_stmts.c -o $@

build/c_code_gen_struct.o: $(DIR_CG)/struct/c_code_gen_struct.c 
	$(CALL) -c $(DIR_CG)/struct/c_code_gen_struct.c -o $@
	
build/c_code_gen_subr.o: $(DIR_CG)/subr/c_code_gen_subr.c 
	$(CALL) -c $(DIR_CG)/subr/c_code_gen_subr.c -o $@

build/c_code_gen_types.o: $(DIR_CG)/types/c_code_gen_types.c 
	$(CALL) -c $(DIR_CG)/types/c_code_gen_types.c -o $@

build/c_code_gen_var.o: $(DIR_CG)/var/c_code_gen_var.c 
	$(CALL) -c $(DIR_CG)/var/c_code_gen_var.c -o $@

# --------------

build/code_gen_util.o: main/code_gen/util/code_gen_util.c 
	$(CALL) -c main/code_gen/util/code_gen_util.c -o $@

build/structs_code_gen.o: main/code_gen/structs/structs_code_gen.c
	$(CALL) -c main/code_gen/structs/structs_code_gen.c -o $@

build/test.o: test/test.c test/test.h \
		main/transpiler.h main/flags/flags.h \
		test/util/test_statuscode.h \
		test/code_gen/test_op.h \
		test/code_gen/test_other.h \
		test/code_gen/test_assign.h \
		test/typeinference/test_typeinference.h
	$(CALLTEST) -c test/test.c -o $@

build/test_statuscode.o: test/util/test_statuscode.h test/util/test_statuscode.c main/transpiler.h
	$(CALLTEST) -c test/util/test_statuscode.c -o $@
	
build/test_assign.o: test/util/test_statuscode.h test/util/test_statuscode.c test/code_gen/test_assign.h test/code_gen/test_assign.c
	$(CALLTEST) -c test/code_gen/test_assign.c -o $@

build/test_other.o: test/util/test_statuscode.h test/util/test_statuscode.c test/code_gen/test_other.h test/code_gen/test_other.c
	$(CALLTEST) -c test/code_gen/test_other.c -o $@
	
build/test_op.o: test/util/test_statuscode.h test/util/test_statuscode.c test/code_gen/test_op.h test/code_gen/test_op.c
	$(CALLTEST) -c test/code_gen/test_op.c -o $@
	
build/test_stmt.o: test/util/test_statuscode.h test/util/test_statuscode.c test/code_gen/test_stmt.h test/code_gen/test_stmt.c
	$(CALLTEST) -c test/code_gen/test_stmt.c -o $@
	
build/test_typeinference.o: test/typeinference/test_typeinference.h test/typeinference/test_typeinference.c
	$(CALLTEST) -c test/typeinference/test_typeinference.c -o $@


build/help.o: main/util/help.c main/util/help.h
	$(CALL) -c main/util/help.c -o $@
	
build/fileutils.o: main/util/fileutils/fileutils.c
	$(CALL) -c main/util/fileutils/fileutils.c -o $@

build/gen_c_types.o: main/code_gen/types/gen_c_types.h
	$(CALL) -c main/code_gen/types/gen_c_types.c -o $@

# ----------- typeinference --------------

build/typeinfer.o: main/typeinference/typeinfer.c main/typeinference/typeinfer.h 
	$(CALL) -c main/typeinference/typeinfer.c -o $@
	
build/typeinfer_expr.o: main/typeinference/typeinfer_expr.c
	$(CALL) -c main/typeinference/typeinfer_expr.c -o $@
	
build/typeinfer_var.o: main/typeinference/typeinfer_var.c
	$(CALL) -c main/typeinference/typeinfer_var.c -o $@
	
build/typeinfer_simplevar.o: main/typeinference/typeinfer_simplevar.c
	$(CALL) -c main/typeinference/typeinfer_simplevar.c -o $@
	
build/typeinfer_term.o: main/typeinference/typeinfer_term.c
	$(CALL) -c main/typeinference/typeinfer_term.c -o $@

build/typeinfer_methodcall.o: main/typeinference/typeinfer_methodcall.c
	$(CALL) -c main/typeinference/typeinfer_methodcall.c -o $@
	
# ----------- Analyzer ----------------

build/lv_analyzer.o: $(SYMTABLES_H) main/analyzer/lv_analyzer.c
	$(CALL) -c main/analyzer/lv_analyzer.c -o $@
	
build/fn_analyzer.o: $(SYMTABLES_H) main/analyzer/fn_analyzer.c
	$(CALL) -c main/analyzer/fn_analyzer.c -o $@
	
build/dead_analyzer.o: $(SYMTABLES_H) main/analyzer/dead_analyzer.c
	$(CALL) -c main/analyzer/dead_analyzer.c -o $@
	
build/halt_analyzer.o: $(SYMTABLES_H) main/analyzer/halt_analyzer.c
	$(CALL) -c main/analyzer/halt_analyzer.c -o $@
	
# ----------- utility -----------------
	
build/type_str.o: $(SYMTABLES_H) main/typeinference/util/type_str.c main/typeinference/util/type_str.h
	$(CALL) -c main/typeinference/util/type_str.c -o $@


