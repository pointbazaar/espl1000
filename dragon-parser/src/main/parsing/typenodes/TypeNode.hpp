#ifndef TYPENODE
#define TYPENODE

#include <variant>
class TokenList;
class BasicTypeWrappedNode;
class TypeParameterNode;
class ArrayTypeNode;

class TypeNode {

public:
	TypeNode(TokenList tokens);
	TypeNode(BasicTypeWrappedNode* typeNode);
	TypeNode(TypeParameterNode* typeNode);
	TypeNode(ArrayTypeNode* typeNode);
	
	//only one of these is != NULL
	BasicTypeWrappedNode* m1 = NULL;
	TypeParameterNode* m2 = NULL;
	ArrayTypeNode* m3 = NULL;
};

#endif