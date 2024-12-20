add_library("sd-tac" STATIC
        tac.c
        tac_ctor.c
        tac_str.c
        tacbuffer.c
)

target_compile_options("sd-tac" PUBLIC ${COMMON_OPTS})

target_include_directories("sd-tac" PUBLIC "..")

add_subdirectory(test)
