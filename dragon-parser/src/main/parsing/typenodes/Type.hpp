#ifndef TYPE
#define TYPE

class TokenList;
class BasicTypeWrapped;
class TypeParam;
class ArrayType;

class Type {

public:
	Type(TokenList tokens, bool debug);
	Type(BasicTypeWrapped* typeNode);
	Type(TypeParam* typeNode);
	Type(ArrayType* typeNode);
	
	//only one of these is != NULL
	BasicTypeWrapped* m1 = NULL;
	TypeParam* m2 = NULL;
	ArrayType* m3 = NULL;
};

#endif