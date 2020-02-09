#ifndef TYPEPARAM
#define TYPEPARAM

class TokenList;

class TypeParam {

public:
	int typeParameterIndex;
	TypeParam(TokenList, bool debug);
};

#endif