
add_library("sd-rat" STATIC
        rat.c
)

add_library("sd-rat-test"
	rat.c
	test.c
)

target_link_libraries("sd-rat" sd-util)
target_link_libraries("sd-rat-test" sd-util)

target_compile_options("sd-rat" PUBLIC ${COMMON_OPTS})
target_compile_options("sd-rat-test" PUBLIC ${COMMON_OPTS})
