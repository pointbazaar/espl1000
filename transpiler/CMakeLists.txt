cmake_minimum_required(VERSION 3.10.2)
project(sd C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE ON)

#use -pg to generate profiling information for gprof

add_executable(${PROJECT_NAME} 

	main/main.c
	main/transpiler.c
	main/transpile_lambdas.c
	
	#analyzer
	main/analyzer/annotation/annotation_analyzer.c
	main/analyzer/dead/dead_analyzer.c
	main/analyzer/fn/fn_analyzer.c
	main/analyzer/halts/halt_analyzer.c
	main/analyzer/lv/lv_analyzer.c
	
	#code_gen
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
	main/code_gen/c_code_gen/stmts/cg_loopstmt.c
	main/code_gen/c_code_gen/stmts/cg_switchcase.c
	main/code_gen/c_code_gen/stmts/cg_throwstmt.c
	main/code_gen/c_code_gen/stmts/cg_trycatch.c
	main/code_gen/c_code_gen/stmts/cg_whilestmt.c
	main/code_gen/c_code_gen/struct/cg_structdecl.c
	main/code_gen/c_code_gen/struct/cg_structmember.c
	main/code_gen/c_code_gen/struct/cg_struct_reorder.c
	main/code_gen/c_code_gen/subr/cg_subr.c
	main/code_gen/c_code_gen/types/cg_types.c
	main/code_gen/c_code_gen/var/cg_var.c
	main/code_gen/structs/structs_code_gen.c
	main/code_gen/types/gen_c_types.c
	main/code_gen/util/indent.c
	
	#flags
	main/flags/flags.c
	main/flags/validate_flags.c
	
	#invoke
	main/invoke/invoke.c
	
	#typechecker
	main/typechecker/typecheck.c
	main/typechecker/typecheck_errors.c
	main/typechecker/typecheck_stmts.c
	main/typechecker/typecheck_methodcall.c
	main/typechecker/typecheck_assignstmt.c
	main/typechecker/typecheck_expr.c
	main/typechecker/typecheck_unopterm.c
	main/typechecker/typecheck_term.c
	main/typechecker/typecheck_op.c
	main/typechecker/typecheck_var.c
	main/typechecker/typecheck_utils.c
	
	#typeinference
	main/typeinference/typeinfer.c
	main/typeinference/typeinfer_expr.c
	main/typeinference/typeinfer_methodcall.c
	main/typeinference/typeinfer_simplevar.c
	main/typeinference/typeinfer_term.c
	main/typeinference/typeinfer_var.c
	main/typeinference/typeinfer_const.c
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
