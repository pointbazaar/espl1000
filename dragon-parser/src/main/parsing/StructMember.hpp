#ifndef STRUCTMEMBER
#define STRUCTMEMBER

class TokenList;
struct Type;
struct Identifier;

struct StructMember{

	struct Type* type;
	char* name;

};

struct StructMember* makeStructMember(TokenList* tokens, bool debug);

#endif