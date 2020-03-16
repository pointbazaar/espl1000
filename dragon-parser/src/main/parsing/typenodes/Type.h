#ifndef TYPE
#define TYPE

#include <stdbool.h>

struct TokenList;
struct BasicTypeWrapped;
struct TypeParam;
struct ArrayType;

struct Type {

	
	//only one of these is != NULL
	struct BasicTypeWrapped* m1;
	struct TypeParam* m2;
	struct ArrayType* m3;
};

struct Type* makeType2(struct TokenList* tokens, bool debug);
struct Type* makeType_1(struct BasicTypeWrapped* typeNode);
struct Type* makeType_2(struct TypeParam* typeNode);
struct Type* makeType_3(struct ArrayType* typeNode);

#endif