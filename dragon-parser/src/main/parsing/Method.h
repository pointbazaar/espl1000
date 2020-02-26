#ifndef METHOD
#define METHOD

#include <stdbool.h>

struct DeclArg;
struct Stmt;
struct Type;

struct TokenList;

struct Method {

	bool isPublic;
	bool hasSideEffects;
	
	struct Type* returnType;
	char* methodName;

	int count_arguments;
	struct DeclArg** arguments;

	int count_statements;
	struct Stmt** statements;
};

struct Method* makeMethod(struct TokenList* tokens, bool debug);

#endif