#ifndef BASICTYPEWRAPPED
#define BASICTYPEWRAPPED

#include <stdbool.h>

struct TokenList;
struct SimpleType;
struct SubrType;

struct BasicTypeWrapped {

	//these are alternatives,
	//to replace my use of <variant> which was
	//causing me problems
	//only one of these is != NULL
	struct SimpleType* m1 = NULL;
	struct SubrType* m2 = NULL;
};

struct BasicTypeWrapped* makeBasicTypeWrapped(struct SimpleType* typeNode);
struct BasicTypeWrapped* makeBasicTypeWrapped(struct SubrType* typeNode);
struct BasicTypeWrapped* makeBasicTypeWrapped(struct TokenList* tokens, bool debug);

#endif