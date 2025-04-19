
add_library(sd-lexer-test
	lexer_test_utils.c
	test.c

	testcases/tests_operators.c
	testcases/tests_mixed.c
	testcases/tests_keywords.c
	testcases/tests_const.c
	testcases/tests_comments.c
	testcases/tests_other.c
)

target_link_libraries(sd-lexer-test lexer-lib)

target_compile_options(sd-lexer-test  PUBLIC ${COMMON_OPTS})

target_include_directories(sd-lexer-test PUBLIC "../..")

target_link_libraries(sd-lexer-test sd-token)

target_link_libraries(sd-lexer-test sd-util)
