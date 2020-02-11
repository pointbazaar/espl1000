#ifndef STRUCTMEMBER
#define STRUCTMEMBER

#include <string>

class TokenList;
class Type;
class Identifier;

class StructMember{

public:
	Type* type;
	std::string name;

	StructMember(TokenList* tokens, bool debug);
};

#endif