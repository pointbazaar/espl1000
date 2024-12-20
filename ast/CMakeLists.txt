
add_library("sd-ast" STATIC
	util/copy_ast.c
	util/equals_ast.c
	util/free_ast.c
	util/str_ast.c
	visitor/visitor.c
)

add_library("sd-ast-test"
	test/test_ast.c
	test/test_str_ast.c

	util/copy_ast.c
	util/equals_ast.c
	util/free_ast.c
	util/str_ast.c
	visitor/visitor.c
)

target_link_libraries("sd-ast" sd-util)
target_link_libraries("sd-ast-test" sd-util)

target_compile_options("sd-ast" PUBLIC ${COMMON_OPTS})
target_compile_options("sd-ast-test" PUBLIC ${COMMON_OPTS})
