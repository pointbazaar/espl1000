#ifndef TYPEPARAMETERNODE
#define TYPEPARAMETERNODE


//standard headers
#include <string>

//project headers
#include "TypeParameterNode.hpp"
#include "../../../TokenList.hpp"
#include "basic_and_wrapped/IBasicAndWrappedTypeNode.hpp"

using namespace std;

class TypeParameterNode : IBasicAndWrappedTypeNode {

public:
	int typeParameterIndex;
	TypeParameterNode(TokenList);
};

#endif