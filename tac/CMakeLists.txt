add_library("sd-tac" STATIC
        tac.c
        tac_ctor.c
        tac_str.c
        tacbuffer.c
        tacbuffer_optimize.c
)

target_include_directories("sd-tac" PUBLIC "..")

add_subdirectory(test)
