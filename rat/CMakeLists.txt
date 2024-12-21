
add_library("sd-rat" STATIC
        rat.c
)

target_link_libraries("sd-rat" sd-util)

target_compile_options("sd-rat" PUBLIC ${COMMON_OPTS})

add_subdirectory(test)
