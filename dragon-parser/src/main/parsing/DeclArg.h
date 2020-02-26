#ifndef DECLARG
#define DECLARG

#include <stdbool.h>

struct TokenList;
struct Type;

struct DeclArg  {

	struct Type* type;
	char* name;
};

struct DeclArg* makeDeclArg(struct TokenList* tokens, bool debug);

#endif