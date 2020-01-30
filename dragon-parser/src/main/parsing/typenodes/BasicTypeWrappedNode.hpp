#ifndef BASICTYPEWRAPPEDNODE
#define BASICTYPEWRAPPEDNODE

#include "ITypeNode.hpp"
#include "IBasicAndWrappedTypeNode.hpp"

using namespace std;

class BasicTypeWrappedNode :  ITypeNode {

public:
	IBasicAndWrappedTypeNode typeNode;
};

#endif