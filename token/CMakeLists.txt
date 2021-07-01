cmake_minimum_required(VERSION 3.10.2)
project("sd-token" C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE ON)

add_library(${PROJECT_NAME} STATIC
	list/TokenList.c
	reader/token_reader.c
	token/token.c
)

target_compile_options(${PROJECT_NAME} PUBLIC "-g" "-march=native" "-Wall" "-Werror")

target_include_directories(${PROJECT_NAME} PUBLIC ".")
