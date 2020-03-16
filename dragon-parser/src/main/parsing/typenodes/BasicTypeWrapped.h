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
	struct SimpleType* m1;
	struct SubrType* m2;
};

struct BasicTypeWrapped* makeBasicTypeWrapped(struct SimpleType* typeNode);
struct BasicTypeWrapped* makeBasicTypeWrapped(struct SubrType* typeNode);
struct BasicTypeWrapped* makeBasicTypeWrapped2(struct TokenList* tokens, bool debug);

#endif