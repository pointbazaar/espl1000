#ifndef TOKENREADER
#define TOKENREADER

#include <string.h>
#include <stdbool.h>

#include "Token.h"
#include "TokenList.h"
#include "TokenKeys.h"

//may return NULL if no token was recognized
struct Token* recognizeToken(char* tkn, bool debug);

#endif