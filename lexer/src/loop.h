#ifndef LOOP_
#define LOOP_

#include <stdbool.h>

#include "tokens.h"

struct Token** lex(char* clean_source, char* tkn_filename);
struct Token** lex_main(char* tkn_filename, char* input_filename, long input_file_length, bool free_tokens);

void readFromFile(
	char* input_filename, 
	uint64_t input_capacity, 
	uint64_t amount_read, 
	uint64_t input_file_length,
	uint64_t* input_index
);

void writeToFile(
	char* tkn_filename, 
	struct Token** tokens, 
	int tokens_capacity, 
	int len,
	bool free_tokens
);

#endif
