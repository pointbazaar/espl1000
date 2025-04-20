cmake_minimum_required(VERSION 3.10.2)
project(smalldragon-toplevel C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE ON)

# for clang tooling support
# such as language server when editing
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options("-g" "-march=native" "-Wall" "-Wextra" "-Werror")

# this helps to provide the correct flags to include
# debug symbols at every step so that we get
# line numbers and sources in valgrind and other tools
set(CMAKE_BUILD_TYPE Debug)

#purpose of this file is so that CLion will recognize
#the whole project

add_subdirectory(util/)
add_subdirectory(token/)
add_subdirectory(ast/)
add_subdirectory(ibuffer/)

add_subdirectory(tables/)
add_subdirectory(rat/)
add_subdirectory(tac/)

add_subdirectory(lexer/)
add_subdirectory(parser/)
add_subdirectory(compiler/)
