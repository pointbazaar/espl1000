#ifndef FLOATCONSTNODE
#define FLOATCONSTNODE

#include "ITermNode.hpp"

using namespace std;

class FloatConstNode : ITermNode {

public:
	//this can also have a negative value if it is preceded by a '-' operator token
	float floatValue;

};

#endif