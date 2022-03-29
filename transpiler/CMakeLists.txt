cmake_minimum_required(VERSION 3.10.2)
project(sd C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE ON)

#use -pg to generate profiling information for gprof

add_library("sd-base"
	main/transpiler.c
	main/lambda/transpile_lambdas.c

	#analyzer
	main/analyzer/annotation/annotation_analyzer.c
	main/analyzer/dead/dead_analyzer.c
	main/analyzer/fn/fn_analyzer.c
	main/analyzer/halts/halt_analyzer.c
	main/analyzer/lv/lv_analyzer.c

	#code_gen/c_code_gen
	main/code_gen/c_code_gen/cg.c
	main/code_gen/c_code_gen/const/cg_const.c
	main/code_gen/c_code_gen/expr/cg_expr.c
	main/code_gen/c_code_gen/expr/cg_op.c
	main/code_gen/c_code_gen/expr/cg_term.c
	main/code_gen/c_code_gen/expr/cg_unopterm.c
	main/code_gen/c_code_gen/stmts/cg_stmts.c
	main/code_gen/c_code_gen/stmts/cg_assignstmt.c
	main/code_gen/c_code_gen/stmts/cg_call.c
	main/code_gen/c_code_gen/stmts/cg_forstmt.c
	main/code_gen/c_code_gen/stmts/cg_ifstmt.c
	main/code_gen/c_code_gen/stmts/cg_switchcase.c
	main/code_gen/c_code_gen/stmts/cg_whilestmt.c
	main/code_gen/c_code_gen/struct/cg_structdecl.c
	main/code_gen/c_code_gen/struct/cg_structmember.c
	main/code_gen/c_code_gen/struct_reorder/cg_struct_reorder.c
	main/code_gen/c_code_gen/subr/cg_subr.c
	main/code_gen/c_code_gen/types/cg_types.c
	main/code_gen/c_code_gen/var/cg_var.c
	main/code_gen/c_code_gen/_cg.h
	main/code_gen/c_code_gen/stmts/cg_stmtblock.c
	main/code_gen/c_code_gen/c_types_util/gen_c_types.c

	#code_gen/x86
	main/code_gen/x86/cg_x86.c
	main/code_gen/x86/_emit_x86.c

	#code_gen/avr
	main/code_gen/avr/cg_avr.c

	main/code_gen/avr/tac/tac.c
	main/code_gen/avr/tac/tac_str.c

	main/code_gen/avr/gen_tac/gen_tac.c
	main/code_gen/avr/gen_tac/gen_tac_assignstmt.c
	main/code_gen/avr/gen_tac/gen_tac_call.c
	main/code_gen/avr/gen_tac/gen_tac_constvalue.c
	main/code_gen/avr/gen_tac/gen_tac_expr.c
	main/code_gen/avr/gen_tac/gen_tac_forstmt.c
	main/code_gen/avr/gen_tac/gen_tac_ifstmt.c
	main/code_gen/avr/gen_tac/gen_tac_retstmt.c
	main/code_gen/avr/gen_tac/gen_tac_simplevar.c
	main/code_gen/avr/gen_tac/gen_tac_stmt.c
	main/code_gen/avr/gen_tac/gen_tac_switchstmt.c
	main/code_gen/avr/gen_tac/gen_tac_term.c
	main/code_gen/avr/gen_tac/gen_tac_unopterm.c
	main/code_gen/avr/gen_tac/gen_tac_variable.c
	main/code_gen/avr/gen_tac/gen_tac_whilestmt.c

	main/code_gen/avr/compile_ir/compile_tac.h
	main/code_gen/avr/compile_ir/compile_tac_binary_op.c
	main/code_gen/avr/compile_ir/compile_tac_call.c
	main/code_gen/avr/compile_ir/compile_tac_const_value.c
	main/code_gen/avr/compile_ir/compile_tac_copy.c
	main/code_gen/avr/compile_ir/compile_tac_deref.c
	main/code_gen/avr/compile_ir/compile_tac_goto.c
	main/code_gen/avr/compile_ir/compile_tac_if_goto.c
	main/code_gen/avr/compile_ir/compile_tac_label.c
	main/code_gen/avr/compile_ir/compile_tac_nop.c
	main/code_gen/avr/compile_ir/compile_tac_return.c
	main/code_gen/avr/compile_ir/compile_tac_unary_op.c

	main/code_gen/avr/rat/rat.c

	main/code_gen/avr/tacbuffer/tacbuffer.c

	main/code_gen/avr/basic_block/basicblock.c

	main/code_gen/avr/cg_avr_basic_block.c
	main/code_gen/avr/cg_avr_single_tac.c
	main/code_gen/avr/cg_avr_single_function.c

	#code_gen/util
	main/code_gen/util/indent.c

	#invoke
	main/invoke/invoke.c

	#typechecker
	main/typechecker/typecheck.c
	main/typechecker/util/tc_errors.c
	main/typechecker/tc_stmts.c
	main/typechecker/tc_methodcall.c
	main/typechecker/tc_assignstmt.c
	main/typechecker/tc_expr.c
	main/typechecker/tc_unopterm.c
	main/typechecker/tc_term.c
	main/typechecker/tc_var.c
	main/typechecker/util/tc_utils.c
	main/typechecker/tc_range.c
	main/typechecker/tc_method.c
	main/typechecker/type_contains/tc_type_contains.c
	main/typechecker/type_contains/tc_type_contains.h
	main/typechecker/tc_switchstmt.c
	main/typechecker/tc_retstmt.c
	main/typechecker/tc_ifstmt.c
	main/typechecker/tc_whilestmt.c
	main/typechecker/tc_forstmt.c
	main/typechecker/tc_trycatchstmt.c

	#typeinference
	main/typeinference/typeinfer.c
	main/typeinference/typeinfer_expr.c
	main/typeinference/typeinfer_methodcall.c
	main/typeinference/typeinfer_simplevar.c
	main/typeinference/typeinfer_term.c
	main/typeinference/typeinfer_var.c
	main/typeinference/typeinfer_const.c
	main/typeinference/util/type_str.c
	main/typeinference/infer_in_context.c

	#util
	main/util/help.c
	main/util/fill_tables.c
	main/util/fill_tables.h
	main/util/fileutils/fileutils.c

	#flags
	cli/flags/flags.c
	cli/flags/validate_flags.c
)

add_executable("sd"
	cli/main.c
)

add_executable("sd-tests"
	#tests
	test/test.c
	test/code_gen/test_assign.c
	test/code_gen/test_op.c
	test/code_gen/test_other.c
	test/code_gen/test_stmt.c
	test/code_gen/test_call.c
	test/typeinference/test_typeinference.c
	test/util/test_statuscode.c
	test/typechecker/test_typechecker_util.c
	test/typechecker/test_typechecker_util.h
	test/typeinference/test_typeinference_util.c
	test/typeinference/test_typeinference_util.h
	test/typechecker/test_typechecker.c
	test/typechecker/test_typechecker.h
)

target_compile_options("sd-base"  PUBLIC "-g" "-march=native" "-Wall" "-Wextra" "-Werror")
target_compile_options("sd"       PUBLIC "-g" "-march=native" "-Wall" "-Wextra" "-Werror")
target_compile_options("sd-tests" PUBLIC "-g" "-march=native" "-Wall" "-Wextra" "-Werror")

#AST dependency
target_link_libraries("sd-base" ${CMAKE_SOURCE_DIR}/../ast/build/libsd-ast.a)
#tables dependency
target_link_libraries("sd-base" ${CMAKE_SOURCE_DIR}/../tables/build/libsd-tables.a)

target_link_libraries("sd" sd-base)

target_link_libraries("sd-tests" sd-base)

target_include_directories("sd-base" PUBLIC "main")
target_include_directories("sd-base" PUBLIC "..")

target_include_directories("sd" PUBLIC "main")
target_include_directories("sd" PUBLIC "..")

target_include_directories("sd-tests" PUBLIC "main")
target_include_directories("sd-tests" PUBLIC "test")
target_include_directories("sd-tests" PUBLIC "..")



