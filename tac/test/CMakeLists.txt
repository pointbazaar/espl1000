
add_library("sd-tac-test"
	test.c
	test_tac.c
	test_tacbuffer.c
)

target_link_libraries("sd-tac-test" sd-tac)
target_link_libraries("sd-tac-test" sd-util)
target_link_libraries("sd-tac-test" sd-tables)
target_link_libraries("sd-tac-test" sd-ast)
