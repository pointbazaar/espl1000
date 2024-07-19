
#Manual:
#https://westes.github.io/flex/manual/

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
    ${CMAKE_BINARY_DIR}/lex.yy.h
)

include_directories(${CMAKE_SOURCE_DIR}/lexer/)

# Add custom command to generate lex.yy.c and lex.yy.h
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/lex.yy.c ${CMAKE_BINARY_DIR}/lex.yy.h
    COMMAND flex --header-file=${CMAKE_BINARY_DIR}/lex.yy.h -o ${CMAKE_BINARY_DIR}/lex.yy.c ${CMAKE_SOURCE_DIR}/lexer/smalldragon.flex
    DEPENDS smalldragon.flex
    COMMENT "Generating lex.yy.c and lex.yy.h with Flex"
    VERBATIM
)

# Add custom target that depends on the generated files
add_custom_target(GenerateLexerFiles ALL
	DEPENDS ${CMAKE_BINARY_DIR}/lex.yy.c ${CMAKE_BINARY_DIR}/lex.yy.h ${CMAKE_SOURCE_DIR}/lexer/src/driver.h
)

# Add a separate target for lex.yy.c
add_library(lexer-lib STATIC ${CMAKE_BINARY_DIR}/lex.yy.c src/driver.h)

set(MINIMAL_OPTS, "-g" "-march=native" "--std=c11")
#set(COMMON_OPTS, "-Wall" "-Wextra" "-Werror")

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

target_link_libraries(dragon-lexer sd-token)

target_link_libraries(dragon-lexer-tests sd-token)

#Util dependency
target_link_libraries(dragon-lexer sd-util)

#Util dependency
target_link_libraries(dragon-lexer-tests sd-util)
