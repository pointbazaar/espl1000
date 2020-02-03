#ifndef TYPEPARAMETERNODE
#define TYPEPARAMETERNODE


//standard headers
#include <string>

//project headers
#include "TypeParameterNode.hpp"
#include "../../lexing/TokenList.hpp"
#include "IBasicAndWrappedTypeNode.hpp"

using namespace std;

class TypeParameterNode : public IBasicAndWrappedTypeNode {

public:
	int typeParameterIndex;
	TypeParameterNode(TokenList);
};

#endif