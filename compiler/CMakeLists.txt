cmake_minimum_required(VERSION 3.10.2)
project(sd C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE ON)

#use -pg to generate profiling information for gprof

add_library("sd-base"
		main/compiler.c
	main/lambda/transpile_lambdas.c

	#analyzer
	main/analyzer/annotation/annotation_analyzer.c
	main/analyzer/dead/dead_analyzer.c
	main/analyzer/fn/fn_analyzer.c
	main/analyzer/halts/halt_analyzer.c
	main/analyzer/lv/lv_analyzer.c

	#code_gen/c_code_gen
        main/c_code_gen/cg.c
        main/c_code_gen/cg_struct.c
        main/c_code_gen/cg_subr.c
        main/c_code_gen/cg_types.c
        main/c_code_gen/_cg.h
        main/c_code_gen/c_types_util/gen_c_types.c

	#code_gen/avr_code_gen
		main/avr_code_gen/cg_avr.c

		main/avr_code_gen/tac/tac.c
		main/avr_code_gen/tac/tac_str.c

		main/avr_code_gen/gen_tac/gen_tac.c
		main/avr_code_gen/gen_tac/gen_tac_assignstmt.c
		main/avr_code_gen/gen_tac/gen_tac_call.c
		main/avr_code_gen/gen_tac/gen_tac_constvalue.c
		main/avr_code_gen/gen_tac/gen_tac_expr.c
		main/avr_code_gen/gen_tac/gen_tac_forstmt.c
		main/avr_code_gen/gen_tac/gen_tac_ifstmt.c
		main/avr_code_gen/gen_tac/gen_tac_retstmt.c
		main/avr_code_gen/gen_tac/gen_tac_simplevar.c
		main/avr_code_gen/gen_tac/gen_tac_stmt.c
		main/avr_code_gen/gen_tac/gen_tac_switchstmt.c
		main/avr_code_gen/gen_tac/gen_tac_term.c
		main/avr_code_gen/gen_tac/gen_tac_unopterm.c
		main/avr_code_gen/gen_tac/gen_tac_variable.c
		main/avr_code_gen/gen_tac/gen_tac_whilestmt.c

		main/avr_code_gen/compile_ir/compile_tac.h
		main/avr_code_gen/compile_ir/compile_tac_binary_op.c
		main/avr_code_gen/compile_ir/compile_tac_call.c
		main/avr_code_gen/compile_ir/compile_tac_const_value.c
		main/avr_code_gen/compile_ir/compile_tac_copy.c
		main/avr_code_gen/compile_ir/compile_tac_deref.c
		main/avr_code_gen/compile_ir/compile_tac_goto.c
		main/avr_code_gen/compile_ir/compile_tac_if_goto.c
		main/avr_code_gen/compile_ir/compile_tac_label.c
		main/avr_code_gen/compile_ir/compile_tac_nop.c
		main/avr_code_gen/compile_ir/compile_tac_return.c
		main/avr_code_gen/compile_ir/compile_tac_unary_op.c
		main/avr_code_gen/compile_ir/compile_tac_binary_op_immediate.c

		main/avr_code_gen/tacbuffer/tacbuffer.c

		main/avr_code_gen/basic_block/basicblock.c

		main/avr_code_gen/cg_avr_basic_block.c
		main/avr_code_gen/cg_avr_single_tac.c
		main/avr_code_gen/cg_avr_single_function.c

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
		test/typeinference/test_typeinference.c
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
#RAT dependency
target_link_libraries("sd-base" ${CMAKE_SOURCE_DIR}/../rat/build/libsd-rat.a)

target_link_libraries("sd" sd-base)

target_link_libraries("sd-tests" sd-base)

target_include_directories("sd-base" PUBLIC "main")
target_include_directories("sd-base" PUBLIC "..")

target_include_directories("sd" PUBLIC "main")
target_include_directories("sd" PUBLIC "..")

target_include_directories("sd-tests" PUBLIC "main")
target_include_directories("sd-tests" PUBLIC "test")
target_include_directories("sd-tests" PUBLIC "..")



