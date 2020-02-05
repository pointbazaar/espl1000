#ifndef BASICTYPEWRAPPEDNODE
#define BASICTYPEWRAPPEDNODE

#include "ITypeNode.hpp"
#include "IBasicAndWrappedTypeNode.hpp"
#include "../../commandline/TokenList.hpp"

using namespace std;

class BasicTypeWrappedNode :  public ITypeNode {

public:
	BasicTypeWrappedNode(IBasicAndWrappedTypeNode typeNode);
	BasicTypeWrappedNode(TokenList tokens);

	IBasicAndWrappedTypeNode typeNode;
};

#endif