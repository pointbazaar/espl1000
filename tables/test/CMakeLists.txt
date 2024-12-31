
add_library("sd-tables-test" STATIC

	test.c
)

target_link_libraries("sd-tables-test" sd-tables)
target_link_libraries("sd-tables-test" sd-util)

target_compile_options("sd-tables-test" PUBLIC ${COMMON_OPTS})

target_include_directories("sd-tables-test" PUBLIC "../..")

