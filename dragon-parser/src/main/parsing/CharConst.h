#ifndef CHARCONST
#define CHARCONST

#include <stdbool.h>

struct TokenList;

struct CharConst {
	char content;
};

struct CharConst* makeCharConst(struct TokenList* tokens,bool debug);

#endif