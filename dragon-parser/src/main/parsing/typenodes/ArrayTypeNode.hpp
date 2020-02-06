#ifndef ARRAYTYPENODE
#define ARRAYTYPENODE


//#include "TypeNode.hpp"
//#include "../../commandline/TokenList.hpp"

class TokenList;
class TypeNode;

class ArrayTypeNode {

public:

	ArrayTypeNode(TypeNode* element_type);
	ArrayTypeNode(TokenList tokens);

	TypeNode* element_type;

};

#endif