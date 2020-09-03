#ifndef CHARCONST
#define CHARCONST

#include <stdbool.h>

struct CharConst;
struct TokenList;

struct CharConst* makeCharConst(struct TokenList* tokens,bool debug);


#endif
