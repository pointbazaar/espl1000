#ifndef BASICTYPEWRAPPEDNODE
#define BASICTYPEWRAPPEDNODE

#include "ITypeNode.hpp"
#include "IBasicAndWrappedTypeNode.hpp"
#include "../../lexing/TokenList.hpp"

using namespace std;

class BasicTypeWrappedNode :  ITypeNode {

public:
	BasicTypeWrappedNode(IBasicAndWrappedTypeNode typeNode);
	BasicTypeWrappedNode(TokenList tokens);

	IBasicAndWrappedTypeNode typeNode;
};

#endif