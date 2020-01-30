#ifndef SUBROUTINETYPENODE
#define SUBROUTINETYPENODE

#include <vector>

#include "TypeNode.hpp"
#include "IBasicAndWrappedTypeNode.hpp"

using namespace std;

class SubroutineTypeNode : IBasicAndWrappedTypeNode  {

public:
	TypeNode returnType;
	bool hasSideEffects;
	vector<TypeNode> argumentTypes;

};

#endif