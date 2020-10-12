#ifndef LOOPIO
#define LOOPIO

#include <stdbool.h>
#include <inttypes.h>

#define INPUT_CAPACITY 5000

struct Token;

//in loop_io.c
extern char input[INPUT_CAPACITY];

void readFromFile(
	char* input_filename, 
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
