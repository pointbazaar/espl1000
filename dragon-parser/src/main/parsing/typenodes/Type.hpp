#ifndef TYPE
#define TYPE

class TokenList;

struct BasicTypeWrapped;
struct TypeParam;
struct ArrayType;

struct Type {

	
	//only one of these is != NULL
	struct BasicTypeWrapped* m1 = NULL;
	struct TypeParam* m2 = NULL;
	struct ArrayType* m3 = NULL;
};

struct Type* makeType(TokenList* tokens, bool debug);
struct Type* makeType(struct BasicTypeWrapped* typeNode);
struct Type* makeType(struct TypeParam* typeNode);
struct Type* makeType(struct ArrayType* typeNode);

#endif