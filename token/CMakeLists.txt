add_library(sd-token STATIC
	list/TokenList.c
	token/token.c
)

target_link_libraries(sd-token sd-util)

target_include_directories(sd-token PUBLIC ".")

add_subdirectory(test)
