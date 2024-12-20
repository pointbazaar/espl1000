
add_library("sd-ast-test"
	test_ast.c
	test_str_ast.c
)

target_link_libraries("sd-ast-test" sd-util)
target_compile_options("sd-ast-test" PUBLIC ${COMMON_OPTS})
