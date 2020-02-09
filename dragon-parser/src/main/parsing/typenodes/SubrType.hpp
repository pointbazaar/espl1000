
#ifndef SUBROUTINETYPE
#define SUBROUTINETYPE

#include <vector>

class TokenList;
class Type;

class SubrType {

public:
	SubrType(Type* tn, bool hasSideEffects);
	SubrType(TokenList tokens, bool debug);

	Type* returnType;
	bool hasSideEffects;
	std::vector<Type*> argumentTypes;

};
#endif