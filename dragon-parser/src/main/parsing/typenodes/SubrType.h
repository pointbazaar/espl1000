#ifndef SUBROUTINETYPE
#define SUBROUTINETYPE

#include <stdbool.h>

struct TokenList;
struct Type;

struct SubrType {

	struct Type* returnType;
	bool hasSideEffects;

	struct Type** argumentTypes;
	int count_argumentTypes;

};

struct SubrType* makeSubrType(struct TokenList* tokens, bool debug);
struct SubrType* makeSubrType2(struct Type* tn, bool hasSideEffects);

#endif
