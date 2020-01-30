#ifndef TYPENODE
#define TYPENODE

#include "ITypeNode.hpp"
#include "../../lexing/TokenList.hpp"

using namespace std;

class TypeNode {

public:
	TypeNode(TokenList tokens);
	ITypeNode typeNode;
};

#endif