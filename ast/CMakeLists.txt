cmake_minimum_required(VERSION 3.10.2)
project("sd-ast" C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE ON)

add_library(${PROJECT_NAME} STATIC
	util/copy_ast.c
	util/equals_ast.c
	util/free_ast.c
	util/str_ast.c
	visitor/visitor.c
)

add_executable(${PROJECT_NAME}-test
	test/test_ast.c
	test/test_str_ast.c

	util/copy_ast.c
	util/equals_ast.c
	util/free_ast.c
	util/str_ast.c
	visitor/visitor.c
)

target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/../util/build/libsd-util.a)
target_link_libraries(${PROJECT_NAME}-test ${CMAKE_SOURCE_DIR}/../util/build/libsd-util.a)

set(COMMON_OPTS, "-g" "-march=native" "-Wall" "-Wextra" "-Werror")

target_compile_options(${PROJECT_NAME}      PUBLIC ${COMMON_OPTS})
target_compile_options(${PROJECT_NAME}-test PUBLIC ${COMMON_OPTS})
