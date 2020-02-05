#ifndef SUBROUTINETYPENODE
#define SUBROUTINETYPENODE

#include <vector>

#include "TypeNode.hpp"
#include "../../commandline/TokenList.hpp"
#include "IBasicAndWrappedTypeNode.hpp"

using namespace std;

class SubroutineTypeNode : public IBasicAndWrappedTypeNode  {

public:
	SubroutineTypeNode(TypeNode tn, bool hasSideEffects);
	SubroutineTypeNode(TokenList tokens);

	TypeNode returnType;
	bool hasSideEffects;
	vector<TypeNode> argumentTypes;

};

#endif