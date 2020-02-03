#ifndef SIMPLETYPENODE
#define SIMPLETYPENODE

#include <string>

#include "IBasicAndWrappedTypeNode.hpp"
#include "../../lexing/TokenList.hpp"

using namespace std;

class SimpleTypeNode : public IBasicAndWrappedTypeNode {

public:

	SimpleTypeNode(TokenList tokens);
	SimpleTypeNode(string typeName);

	string typeName;

};

#endif