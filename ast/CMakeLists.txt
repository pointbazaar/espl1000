cmake_minimum_required(VERSION 3.10.2)
project("sd-ast" C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE ON)

add_library(${PROJECT_NAME} STATIC
	io/ast_reader.c
	io/ast_writer.c
	io/serialize.c
	util/copy_ast.c
	util/equals_ast.c
	util/free_ast.c
	util/str_ast.c
	visitor/visitor.c
)

add_executable(${PROJECT_NAME}-test
	test/test_ast.c
	test/test_str_ast.c

	io/ast_reader.c
	io/ast_writer.c
	io/serialize.c
	util/copy_ast.c
	util/equals_ast.c
	util/free_ast.c
	util/str_ast.c
	visitor/visitor.c
)

target_compile_options(${PROJECT_NAME}      PUBLIC "-g" "-march=native" "-Wall" "-Werror")
target_compile_options(${PROJECT_NAME}-test PUBLIC "-g" "-march=native" "-Wall" "-Werror")
