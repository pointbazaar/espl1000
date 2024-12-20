
add_library(sd-token-test
	test.c
	testcases/tests_tokenlist.c
)

target_link_libraries(sd-token-test sd-util)
target_link_libraries(sd-token-test sd-token)
