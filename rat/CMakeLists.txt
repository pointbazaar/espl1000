cmake_minimum_required(VERSION 3.10.2)
project("sd-rat" C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE ON)

add_library(${PROJECT_NAME} STATIC
        rat.c
)

add_executable("sd-rat-tests"
	rat.c
	test.c
)


#Util dependency
target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/../util/build/libsd-util.a)
target_link_libraries("sd-rat-tests" ${CMAKE_SOURCE_DIR}/../util/build/libsd-util.a)

target_compile_options(${PROJECT_NAME} PUBLIC "-g" "-march=native" "-Wall" "-Wextra" "-Werror")
target_compile_options("sd-rat-tests" PUBLIC "-g" "-march=native" "-Wall" "-Wextra" "-Werror")
