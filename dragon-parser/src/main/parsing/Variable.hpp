#ifndef VARIABLE
#define VARIABLE

struct SimpleVar;
class TokenList;

struct Variable {

	struct SimpleVar* simpleVariableNode;

	struct Variable** memberAccessList;
	int count_memberAccessList;
};

struct Variable* makeVariable(TokenList* tokens, bool debug);

#endif