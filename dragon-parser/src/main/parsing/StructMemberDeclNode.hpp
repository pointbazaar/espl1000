#ifndef STRUCTMEMBERDECLNODE
#define STRUCTMEMBERDECLNODE

#include <string>

#include "typenodes/BasicTypeWrappedNode.hpp"

using namespace std;

class StructMemberDeclNode {

public:
	BasicTypeWrappedNode typeNode;
	string name;
};

#endif