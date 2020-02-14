#ifndef TYPEPARAM
#define TYPEPARAM

class TokenList;

struct TypeParam {

	int typeParameterIndex;
};

struct TypeParam* makeTypeParam(TokenList* tokens, bool debug);

#endif