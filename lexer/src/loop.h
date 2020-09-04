#ifndef LOOP_
#define LOOP_

#include <stdbool.h>

struct Token;

struct Token** lex(
	char* clean_source, 
	char* tkn_filename,
	bool debug
);

struct Token** lex_main(
	char* tkn_filename, 
	char* input_filename, 
	long input_file_length, 
	bool free_tokens,
	bool debug
);

#endif
