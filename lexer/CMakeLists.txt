cmake_minimum_required(VERSION 3.10.2)
project(dragon-lexer C)

#Manual:
#https://westes.github.io/flex/manual/

#find_package(FLEX)

#https://cmake.org/cmake/help/latest/module/FindFLEX.html

#FLEX_TARGET(
#	myscanner smalldragon.flex lex.yy.c
#	DEFINES_FILE lex.yy.h
#)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
 
add_executable(dragon-lexer-tests
	test/lexer_test_utils.c
	test/test.c
	
	test/testcases/tests_operators.c
	test/testcases/tests_mixed.c
	test/testcases/tests_keywords.c
	test/testcases/tests_const.c
	test/testcases/tests_comments.c
	test/testcases/tests_other.c
)

# Add the executable for dragon-lexer without lex.yy.c
add_executable(dragon-lexer
    src/driver.c
    src/main.c
)

# Add a separate target for lex.yy.c
add_library(lexer-lib STATIC lex.yy.c)

set(MINIMAL_OPTS, "-g" "-march=native" "--std=c11")
set(COMMON_OPTS, "-Wall" "-Wextra" "-Werror")

# Apply minimal compile options for lex.yy.c
target_compile_options(lexer-lib PRIVATE ${MINIMAL_OPTS} "-Wno-implicit-function-declaration")

# Link lexer-lib to the main executable
target_link_libraries(dragon-lexer lexer-lib)

# Apply compile options for dragon-lexer
target_compile_options(dragon-lexer PUBLIC ${COMMON_OPTS} "-lfl")

target_compile_options(dragon-lexer-tests PUBLIC ${COMMON_OPTS})

#target_link_libraries(dragon-lexer ${FLEX_LIBRARIES})
#https://askubuntu.com/questions/289547/where-are-flex-libraries-located
#this is the non-portable version... 
#target_link_libraries(dragon-lexer /usr/lib/x86_64-linux-gnu/libfl.a)
target_link_libraries(dragon-lexer libfl.so)

target_link_libraries(dragon-lexer ${CMAKE_SOURCE_DIR}/../token/build/libsd-token.a)

target_link_libraries(dragon-lexer-tests ${CMAKE_SOURCE_DIR}/../token/build/libsd-token.a)

#Util dependency
target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/../util/build/libsd-util.a)

#Util dependency
target_link_libraries(${PROJECT_NAME}-tests ${CMAKE_SOURCE_DIR}/../util/build/libsd-util.a)
