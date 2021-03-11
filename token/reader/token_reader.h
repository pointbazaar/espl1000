#ifndef TOKENREADER
#define TOKENREADER

#include <stdbool.h>

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"

struct Token;

//may return NULL if no token was recognized
struct Token* recognizeToken(char* tkn, bool* isLineNo, bool debug);

#endif
