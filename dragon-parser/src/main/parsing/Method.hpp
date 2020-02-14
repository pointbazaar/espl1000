#ifndef METHOD
#define METHOD

struct DeclArg;
struct Stmt;
struct Type;

class TokenList;

struct Method {

	bool isPublic = true;
	bool hasSideEffects;
	struct Type* returnType;
	char* methodName;

	int count_arguments;
	struct DeclArg** arguments;

	int count_statements;
	struct Stmt** statements;
};

struct Method* makeMethod(TokenList* tokens, bool debug);

#endif