#ifndef IDENTIFIER
#define IDENTIFIER

class TokenList;

struct Identifier  {

	char* identifier;
};

struct Identifier* makeIdentifier(TokenList* tokens, bool debug);

#endif