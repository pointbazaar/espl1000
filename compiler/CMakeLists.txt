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

	#code_gen/avr_code_gen
		main/avr_code_gen/cg_avr.c

        main/gen_tac/gen_tac.c
        main/gen_tac/gen_tac_assignstmt.c
		main/gen_tac/gen_tac_massignstmt.c
        main/gen_tac/gen_tac_call.c
        main/gen_tac/gen_tac_constvalue.c
        main/gen_tac/gen_tac_expr.c
        main/gen_tac/gen_tac_forstmt.c
        main/gen_tac/gen_tac_ifstmt.c
        main/gen_tac/gen_tac_retstmt.c
        main/gen_tac/gen_tac_simplevar.c
        main/gen_tac/gen_tac_stmt.c
        main/gen_tac/gen_tac_switchstmt.c
        main/gen_tac/gen_tac_term.c
        main/gen_tac/gen_tac_unopterm.c
		main/gen_tac/gen_tac_mdirect.c
        main/gen_tac/gen_tac_variable.c
        main/gen_tac/gen_tac_whilestmt.c

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
		main/avr_code_gen/compile_ir/compile_tac_load_const_addr.c
		main/avr_code_gen/compile_ir/compile_tac_store_const_addr.c
		main/avr_code_gen/compile_ir/compile_tac_param.c
		main/avr_code_gen/compile_ir/compile_tac_setup_stackframe.c
		main/avr_code_gen/compile_ir/compile_tac_load_local.c
		main/avr_code_gen/compile_ir/compile_tac_store_local.c
		main/avr_code_gen/compile_ir/compile_tac_if_cmp_goto.c

        main/basic_block/basicblock.c
		main/basic_block/basicblock_printer.c

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
	main/typechecker/tc_mdirect.c
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
	main/util/ctx.c

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

	#typeinference
	test/typeinference/test_typeinference.c
	test/typeinference/test_typeinference_util.c
	test/typeinference/test_typeinference_util.h

	#typechecker
	test/typechecker/test_typechecker_util.c
	test/typechecker/test_typechecker_util.h
	test/typechecker/test_typechecker.c
	test/typechecker/test_typechecker.h

	#CodeGen from TAC
	test/avr_code_gen/test_avr_code_gen.c
	test/avr_code_gen/test_avr_code_gen.h
	test/avr_code_gen/test_avr_code_gen_util.c
	test/avr_code_gen/test_avr_code_gen_util.h
	
	test/avr_code_gen/compile_ir/test_compile_tac_binary_op.c
	test/avr_code_gen/compile_ir/test_compile_tac_binary_op_immediate.c
	test/avr_code_gen/compile_ir/test_compile_tac_const_value.c
	test/avr_code_gen/compile_ir/test_compile_tac_copy.c
	test/avr_code_gen/compile_ir/test_compile_tac_goto.c
	test/avr_code_gen/compile_ir/test_compile_tac_if_goto.c
	test/avr_code_gen/compile_ir/test_compile_tac_load_const_addr.c
	test/avr_code_gen/compile_ir/test_compile_tac_nop.c
	test/avr_code_gen/compile_ir/test_compile_tac_param.c
	test/avr_code_gen/compile_ir/test_compile_tac_return.c
	test/avr_code_gen/compile_ir/test_compile_tac_store_const_addr.c
	test/avr_code_gen/compile_ir/test_compile_tac_unary_op.c
	test/avr_code_gen/compile_ir/test_compile_tac_call.c
	test/avr_code_gen/compile_ir/test_compile_tac_if_cmp_goto.c
	
	#CodeGen from Snippets (test compiling AST pieces)
	test/gen_tac/test_gen_tac.c
	test/gen_tac/util/test_gen_tac_util.c
	test/gen_tac/test_gen_tac_assignstmt.c
	test/gen_tac/test_gen_tac_mdirect.c
	test/gen_tac/test_gen_tac_expr.c
	test/gen_tac/test_gen_tac_ifstmt.c
	test/gen_tac/test_gen_tac_whilestmt.c
	test/gen_tac/test_gen_tac_forstmt.c
	test/gen_tac/test_gen_tac_switchstmt.c
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
#TAC dependency
target_link_libraries("sd-base" ${CMAKE_SOURCE_DIR}/../tac/build/libsd-tac.a)
#Parser dependency
target_link_libraries("sd-base" ${CMAKE_SOURCE_DIR}/../parser/build/libdragon-parser-base.a)
#Token dependency
target_link_libraries("sd-base" ${CMAKE_SOURCE_DIR}/../token/build/libsd-token.a)

target_link_libraries("sd" sd-base)

#libvmcu dependency for sd-tests
target_link_libraries("sd-tests" ${CMAKE_SOURCE_DIR}/../dependencies/libvmcu-Virtual-MCU-Library/build-release/libvmcu.a)

target_link_libraries("sd-tests" sd-base)

target_include_directories("sd-base" PUBLIC "main")
target_include_directories("sd-base" PUBLIC "..")

target_include_directories("sd" PUBLIC "main")
target_include_directories("sd" PUBLIC "..")

target_include_directories("sd-tests" PUBLIC "main")
target_include_directories("sd-tests" PUBLIC "test")
target_include_directories("sd-tests" PUBLIC "..")

#libvmcu include dirs for sd-tests
target_include_directories("sd-tests" PUBLIC "../dependencies/libvmcu-Virtual-MCU-Library/engine/include")



