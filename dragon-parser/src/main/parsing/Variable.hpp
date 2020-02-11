#ifndef VARIABLE
#define VARIABLE

#include <vector>

class SimpleVar;
#include "../commandline/TokenList.hpp"

struct Variable {

	SimpleVar* simpleVariableNode;
	std::vector<struct Variable*> memberAccessList;
};

struct Variable* makeVariable(TokenList* tokens, bool debug);

#endif