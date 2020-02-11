#ifndef STRUCTDECL
#define STRUCTDECL

#include <vector>

class StructMember;
class TokenList;

class StructDecl{

public:
	std::vector<StructMember*> members;

	StructDecl(TokenList* tokens, bool debug);
};

#endif