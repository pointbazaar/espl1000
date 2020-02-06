
#ifndef SUBROUTINETYPENODE
#define SUBROUTINETYPENODE

#include <vector>
//#include "TypeNode.hpp"
//#include "../../commandline/TokenList.hpp"
class TokenList;
class TypeNode;

class SubroutineTypeNode {

public:
	SubroutineTypeNode(TypeNode* tn, bool hasSideEffects);
	SubroutineTypeNode(TokenList tokens);

	TypeNode* returnType;
	bool hasSideEffects;
	std::vector<TypeNode*> argumentTypes;

};

#endif