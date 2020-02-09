#ifndef VARIABLE
#define VARIABLE

#include <vector>

class SimpleVar;
#include "../commandline/TokenList.hpp"

class Variable {

public:
	Variable(TokenList tokens, bool debug);

	SimpleVar* simpleVariableNode;
	std::vector<Variable*> memberAccessList;
};

#endif