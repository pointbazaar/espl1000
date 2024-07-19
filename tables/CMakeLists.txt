cmake_minimum_required(VERSION 3.10.2)
project("sd-tables" C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE ON)

add_library(${PROJECT_NAME} STATIC
	
	cc/cc.c
	
	lvst/lvst.c
	
	sst/sst.c
	sst/sst_fill.c
	sst/sst_print.c

        stst/stst.c
	stst/stst_print.c

        symtable/symtable.c
)

set(COMMON_OPTS, "-g" "-march=native" "-Wall" "-Wextra" "-Werror")

target_compile_options(${PROJECT_NAME} PUBLIC ${COMMON_OPTS})

target_include_directories(${PROJECT_NAME} PUBLIC "..")

