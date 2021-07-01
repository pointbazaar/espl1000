#ifndef TOKENREADER
#define TOKENREADER

#include <stdbool.h>

struct Token;
struct TokenList;

struct TokenList* readTokensFromTokensFile(FILE* file, char* tokensFile, bool debug);


#endif
