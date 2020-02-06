#ifndef TYPEPARAMETERNODE
#define TYPEPARAMETERNODE

//#include "../../commandline/TokenList.hpp"
class TokenList;

class TypeParameterNode {

public:
	int typeParameterIndex;
	TypeParameterNode(TokenList);
};

#endif