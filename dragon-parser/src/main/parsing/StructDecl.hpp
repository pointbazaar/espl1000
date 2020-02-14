#ifndef STRUCTDECL
#define STRUCTDECL

struct StructMember;
class TokenList;

struct StructDecl{

	struct StructMember** members;
	int count_members;
};

struct StructDecl* makeStructDecl(TokenList* tokens, bool debug);

#endif