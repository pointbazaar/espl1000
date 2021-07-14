#ifndef TOKENREADER
#define TOKENREADER

#include <stdbool.h>

struct TokenList;

struct TokenList* read_tokens_from_tokens_file(FILE* file, char* tokensFile);

#endif
