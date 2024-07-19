cmake_minimum_required(VERSION 3.10.2)
project("sd-tac" C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE ON)

add_library(${PROJECT_NAME} STATIC
        tac.c
        tac_str.c
        tacbuffer.c
)

set(COMMON_OPTS, "-g" "-march=native" "-Wall" "-Wextra" "-Werror")

target_compile_options(${PROJECT_NAME} PUBLIC ${COMMON_OPTS})

target_include_directories(${PROJECT_NAME} PUBLIC "..")
