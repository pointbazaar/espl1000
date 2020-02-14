
#ifndef SUBROUTINETYPE
#define SUBROUTINETYPE

class TokenList;
struct Type;

struct SubrType {

	struct Type* returnType;
	bool hasSideEffects;

	struct Type** argumentTypes;
	int count_argumentTypes=0;

};

struct SubrType* makeSubrType(struct Type* tn, bool hasSideEffects);
struct SubrType* makeSubrType(TokenList* tokens, bool debug);

#endif