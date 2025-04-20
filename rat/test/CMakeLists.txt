
add_library("sd-rat-test"
	test.c
)

target_link_libraries("sd-rat-test" sd-util sd-rat)
target_include_directories("sd-rat-test" PUBLIC "../..")
