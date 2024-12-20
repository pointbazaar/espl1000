
add_library(sd-lexer-test
	test/lexer_test_utils.c
	test/test.c

	test/testcases/tests_operators.c
	test/testcases/tests_mixed.c
	test/testcases/tests_keywords.c
	test/testcases/tests_const.c
	test/testcases/tests_comments.c
	test/testcases/tests_other.c
	test/testcases/tests_include_decl.c

	src/driver.c
	src/lexer_main.c
)

include_directories(${CMAKE_SOURCE_DIR}/lexer/)

add_library(
	lexer-lib
	src/driver.h
	src/driver.c
	src/lexer_main.c
	src/lexer.c
)

set(MINIMAL_OPTS, "-g" "-march=native" "--std=c11")

target_compile_options(lexer-lib PRIVATE ${MINIMAL_OPTS} "-Wno-implicit-function-declaration")

target_link_libraries(sd-lexer-test lexer-lib)

target_compile_options(sd-lexer-test  PUBLIC ${COMMON_OPTS})

target_link_libraries(sd-lexer-test sd-token)

target_link_libraries(sd-lexer-test sd-util)
