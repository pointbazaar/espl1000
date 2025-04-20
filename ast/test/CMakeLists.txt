
add_library("sd-ast-test"
	test_str_ast.c
)

target_link_libraries("sd-ast-test" sd-util)
target_include_directories("sd-ast-test" PUBLIC "../..")
