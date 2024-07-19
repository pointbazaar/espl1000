
add_library("sd-rat" STATIC
        rat.c
)

add_executable("sd-rat-tests"
	rat.c
	test.c
)

target_link_libraries("sd-rat" sd-util)
target_link_libraries("sd-rat-tests" sd-util)

target_compile_options("sd-rat" PUBLIC ${COMMON_OPTS})
target_compile_options("sd-rat-tests" PUBLIC ${COMMON_OPTS})
