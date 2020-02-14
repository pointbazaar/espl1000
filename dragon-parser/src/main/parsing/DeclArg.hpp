#ifndef DECLARG
#define DECLARG

class TokenList;

struct Type;

struct DeclArg  {

	struct Type* type = NULL;
	char* name;
};

struct DeclArg* makeDeclArg(TokenList* tokens, bool debug);

#endif