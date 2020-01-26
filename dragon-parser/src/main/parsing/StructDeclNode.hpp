#ifndef STRUCTDECLNODE
#define STRUCTDECLNODE

#include "SimpleTypeNode.hpp";
#include "StructMemberDeclNode.hpp";

class StructDeclNode {

public:
	SimpleTypeNode typeNode;
	vector<StructMemberDeclNode> members;
};

#endif