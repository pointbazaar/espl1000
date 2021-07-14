cmake_minimum_required(VERSION 3.10.2)
project(smalldragon-toplevel C)

#purpose of this file is so that CLion will recognize
#the whole project

add_subdirectory(token/)
add_subdirectory(ast/)

add_subdirectory(parser/)

add_subdirectory(tables/)
add_subdirectory(transpiler/)
