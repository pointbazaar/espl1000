add_library("sd-tac" STATIC
        tac.c
        tac_ctor.c
        tac_str.c
        tacbuffer.c
)

target_compile_options("sd-tac" PUBLIC ${COMMON_OPTS})

target_include_directories("sd-tac" PUBLIC "..")

add_library("sd-tac-test"
	test/test.c
	test/test_tac.c
	test/test_tacbuffer.c
)

target_link_libraries("sd-tac-test" sd-tac)
target_link_libraries("sd-tac-test" sd-util)
target_link_libraries("sd-tac-test" sd-tables)
target_link_libraries("sd-tac-test" sd-ast)
