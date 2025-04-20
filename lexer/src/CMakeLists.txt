
add_library(lexer-lib

	driver.h
	driver.c
	lexer_main.c
	lexer.c
)

target_include_directories(lexer-lib PUBLIC "../../")
