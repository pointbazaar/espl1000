#ifndef ARRAYTYPENODE
#define ARRAYTYPENODE

#include "ITypeNode.hpp"
#include "TypeNode.hpp"
#include "../../lexing/TokenList.hpp"

using namespace std;

class ArrayTypeNode : public ITypeNode {

public:

	ArrayTypeNode(TypeNode element_type);
	ArrayTypeNode(TokenList tokens);

	TypeNode element_type;

};

#endif