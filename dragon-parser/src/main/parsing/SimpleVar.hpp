#ifndef SIMPLEVAR
#define SIMPLEVAR

#include <string>
#include <optional>

#include "../commandline/TokenList.hpp"

class Expr;

class SimpleVar {

public:
	SimpleVar(TokenList* tokens, bool debug);

	std::string name;

	//may be NULL
	struct Expr* indexOptional = NULL;
};

#endif