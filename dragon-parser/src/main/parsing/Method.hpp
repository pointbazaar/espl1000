#ifndef METHOD
#define METHOD

#include <string>
#include <vector>

class DeclaredArg;
class Stmt;
class Type;
#include "../commandline/TokenList.hpp"

class Method {

public:

	Method(TokenList tokens, bool debug);

	bool isPublic = true;
	bool hasSideEffects;
	Type* returnType;
	std::string methodName;
	std::vector<DeclaredArg*> arguments;
	std::vector<Stmt*> statements;
};

#endif