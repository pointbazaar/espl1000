#ifndef STRUCTMEMBER
#define STRUCTMEMBER

struct TokenList;
struct Type;
struct Identifier;

struct StructMember{

	struct Type* type;
	char* name;

};

struct StructMember* makeStructMember(struct TokenList* tokens, bool debug);

#endif