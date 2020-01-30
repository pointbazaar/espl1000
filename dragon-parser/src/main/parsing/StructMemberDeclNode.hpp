#ifndef STRUCTMEMBERDECLNODE
#define STRUCTMEMBERDECLNODE

#include <string>

#include "typenodes/BasicTypeWrappedNode.hpp"

using namespace std;

class StructMemberDeclNode {

public:
	StructMemberDeclNode(TokenList tokens, bool b);

	BasicTypeWrappedNode typeNode;
	string name;
};

#endif