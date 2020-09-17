#ifndef TOKENREADER
#define TOKENREADER

#include <string.h>
#include <stdbool.h>

#include "TokenList.h"
#include "TokenKeys.h"

struct Token;

//may return NULL if no token was recognized
struct Token* recognizeToken(char* tkn, bool* isLineNo, bool debug);

#endif
