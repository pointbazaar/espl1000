#ifndef LOOPIO
#define LOOPIO

#include <stdbool.h>
#include <inttypes.h>

struct Token;

//this must be in the header so loop.c
//can see 'input' variable

//statically allocate a char array to store the contents of
//our input file

char input[5000];

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
