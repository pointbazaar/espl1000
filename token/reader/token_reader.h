#ifndef TOKENREADER
#define TOKENREADER

#include <stdbool.h>

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"

struct Token;

struct TokenList* readTokensFromTokensFile(FILE* file, char* tokensFile, bool debug);


#endif
