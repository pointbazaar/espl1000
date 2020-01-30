#ifndef ARRAYTYPENODE
#define ARRAYTYPENODE

#include "ITypeNode.hpp"
#include "TypeNode.hpp"

using namespace std;

class ArrayTypeNode : ITypeNode {

public:
	TypeNode element_type;

};

#endif