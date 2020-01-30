#ifndef STRUCTDECLNODE
#define STRUCTDECLNODE

#include <vector>

#include "typenodes/SimpleTypeNode.hpp"
#include "StructMemberDeclNode.hpp"

using namespace std;

class StructDeclNode {

public:
	SimpleTypeNode typeNode;

	vector<StructMemberDeclNode> members;
};

#endif