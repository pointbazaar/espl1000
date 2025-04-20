
add_library("sd-tables-test" STATIC

	test.c
)

target_link_libraries("sd-tables-test" sd-tables)
target_link_libraries("sd-tables-test" sd-util)

target_include_directories("sd-tables-test" PUBLIC "../..")

