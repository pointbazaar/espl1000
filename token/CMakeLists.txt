add_library(sd-token STATIC
	list/TokenList.c
	reader/token_reader.c
	token/token.c
)

target_link_libraries(sd-token sd-util)

target_compile_options(sd-token PUBLIC ${COMMON_OPTS})

target_include_directories(sd-token PUBLIC ".")

add_subdirectory(test)
