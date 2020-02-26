#ifndef STRUCTDECL
#define STRUCTDECL

struct StructMember;
struct TokenList;

struct StructDecl{

	struct StructMember** members;
	int count_members;
};

struct StructDecl* makeStructDecl(struct TokenList* tokens, bool debug);

#endif