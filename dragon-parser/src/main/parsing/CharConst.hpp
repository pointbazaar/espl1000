#ifndef CHARCONST
#define CHARCONST

class TokenList;

struct CharConst {
	char content;
};

struct CharConst* makeCharConst(TokenList* tokens,bool debug);

#endif