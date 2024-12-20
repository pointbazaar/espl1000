
add_library(sd-token-test
	testcases/tests_tokenlist.c
)

target_include_directories(sd-token-test PUBLIC "../..")

target_link_libraries(sd-token-test sd-util)
target_link_libraries(sd-token-test sd-token)
