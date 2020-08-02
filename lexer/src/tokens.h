#include <stdint.h>
#include <stdbool.h>

#ifndef TOKENS
#define TOKENS


struct Token{
	uint32_t kind;
	char* value;
	bool statically_allocated;
};


void printTokenList(struct Token** list);

#endif