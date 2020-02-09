#ifndef ARRAYTYPENODE
#define ARRAYTYPENODE

class TokenList;
class TypeNode;

class ArrayTypeNode {

public:

	ArrayTypeNode(TypeNode* element_type);
	ArrayTypeNode(TokenList tokens);

	TypeNode* element_type;

};

#endif