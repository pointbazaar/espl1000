#ifndef LEXER_FLAGS
#define LEXER_FLAGS

#include <stdbool.h>

struct LexerFlags{
	
	char* filename;
	
	bool debug;
	bool clean;
	bool test;
};


#endif
