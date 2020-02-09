#ifndef BASICTYPEWRAPPED
#define BASICTYPEWRAPPED

class TokenList;
class SimpleType;
class SubrType;

class BasicTypeWrapped {

public:
	BasicTypeWrapped(SimpleType* typeNode);
	BasicTypeWrapped(SubrType* typeNode);
	BasicTypeWrapped(TokenList tokens);

	//these are alternatives,
	//to replace my use of <variant> which was
	//causing me problems
	//only one of these is != NULL
	SimpleType* m1 = NULL;
	SubrType* m2 = NULL;
};

#endif