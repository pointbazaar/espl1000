#ifndef SUBROUTINETYPENODE
#define SUBROUTINETYPENODE

#include <vector>

#include "TypeNode.hpp"
#include "IBasicAndWrappedTypeNode.hpp"

using namespace std;

class SubroutineTypeNode : public IBasicAndWrappedTypeNode  {

public:
	SubroutineTypeNode(TypeNode tn, bool hasSideEffects);
	SubroutineTypeNode(TypeNode tn);

	TypeNode returnType;
	bool hasSideEffects;
	vector<TypeNode> argumentTypes;

};

#endif