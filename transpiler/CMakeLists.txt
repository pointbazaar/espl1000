cmake_minimum_required(VERSION 3.10.2)
project(sd C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE ON)

#use -pg to generate profiling information for gprof

add_executable(${PROJECT_NAME} 

	main/transpiler.c
	main/transpile_lambdas.c
	
	#analyzer
	main/analyzer/annotation/annotation_analyzer.c
	main/analyzer/dead/dead_analyzer.c
	main/analyzer/fn/fn_analyzer.c
	main/analyzer/halts/halt_analyzer.c
	main/analyzer/lv/lv_analyzer.c
	
	#code_gen
	main/code_gen/c_code_gen/c_code_gen.c
	main/code_gen/c_code_gen/const/c_code_gen_const.c
	main/code_gen/c_code_gen/expr/c_code_gen_expr.c
	main/code_gen/c_code_gen/stmts/c_code_gen_stmts.c
	main/code_gen/c_code_gen/struct/c_code_gen_struct.c
	main/code_gen/c_code_gen/subr/c_code_gen_subr.c
	main/code_gen/c_code_gen/types/c_code_gen_types.c
	main/code_gen/c_code_gen/var/c_code_gen_var.c
	main/code_gen/structs/structs_code_gen.c
	main/code_gen/types/gen_c_types.c
	main/code_gen/util/code_gen_util.c
	
	#flags
	main/flags/flags.c
	main/flags/validate_flags.c
	
	#invoke
	main/invoke/invoke.c
	
	#typechecker
	main/typechecker/typecheck.c
	main/typechecker/typecheck_errors.c
	main/typechecker/typecheck_stmts.c
	main/typechecker/typecheck_utils.c
	
	#typeinference
	main/typeinference/typeinfer.c
	main/typeinference/typeinfer_expr.c
	main/typeinference/typeinfer_methodcall.c
	main/typeinference/typeinfer_simplevar.c
	main/typeinference/typeinfer_term.c
	main/typeinference/typeinfer_var.c
	main/typeinference/util/type_str.c
	
	#util
	main/util/help.c
	main/util/fileutils/fileutils.c
	
	#tests
	test/test.c
	test/code_gen/test_assign.c
	test/code_gen/test_op.c
	test/code_gen/test_other.c
	test/code_gen/test_stmt.c
	test/typeinference/test_typeinference.c
	test/util/test_statuscode.c
)

#TODO: add some kind of optimized build with -O2
#TODO: use -Wextra
target_compile_options(${PROJECT_NAME} PUBLIC "-g" "-march=native" "-Wall" "-Werror")

#AST dependency
target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/../ast/build/libsd-ast.a)

#tables dependency
target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/../tables/build/libsd-tables.a)

target_include_directories(${PROJECT_NAME} PUBLIC "main")
target_include_directories(${PROJECT_NAME} PUBLIC "test")

target_include_directories(${PROJECT_NAME} PUBLIC "..")
