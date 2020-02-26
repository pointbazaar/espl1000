#ifndef VARIABLE
#define VARIABLE

#include <stdbool.h>

struct SimpleVar;
struct TokenList;

struct Variable {

	struct SimpleVar* simpleVariableNode;

	struct Variable** memberAccessList;
	int count_memberAccessList;
};

struct Variable* makeVariable(struct TokenList* tokens, bool debug);

#endif