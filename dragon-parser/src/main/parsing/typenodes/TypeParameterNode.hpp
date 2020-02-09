#ifndef TYPEPARAMETERNODE
#define TYPEPARAMETERNODE

class TokenList;

class TypeParameterNode {

public:
	int typeParameterIndex;
	TypeParameterNode(TokenList);
};

#endif