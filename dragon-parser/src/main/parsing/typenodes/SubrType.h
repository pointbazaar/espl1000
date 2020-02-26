#ifndef SUBROUTINETYPE
#define SUBROUTINETYPE

#include <stdbool.h>

struct TokenList;
struct Type;

struct SubrType {

	struct Type* returnType;
	bool hasSideEffects;

	struct Type** argumentTypes;
	int count_argumentTypes=0;

};

struct SubrType* makeSubrType(struct Type* tn, bool hasSideEffects);
struct SubrType* makeSubrType(struct TokenList* tokens, bool debug);

#endif