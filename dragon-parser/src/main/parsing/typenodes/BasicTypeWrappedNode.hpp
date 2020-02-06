
#ifndef BASICTYPEWRAPPEDNODE
#define BASICTYPEWRAPPEDNODE

#include <variant>
//#include "../../commandline/TokenList.hpp"
class TokenList;
class SimpleTypeNode;
class SubroutineTypeNode;
/*
#include "SimpleTypeNode.hpp"
#include "SubroutineTypeNode.hpp"
*/

class BasicTypeWrappedNode {

public:
	BasicTypeWrappedNode(SimpleTypeNode* typeNode);
	BasicTypeWrappedNode(SubroutineTypeNode* typeNode);
	BasicTypeWrappedNode(TokenList tokens);

	//these are alternatives,
	//to replace my use of <variant> which was
	//causing me problems
	//only one of these is != NULL
	SimpleTypeNode* m1 = NULL;
	SubroutineTypeNode* m2 = NULL;
};

#endif