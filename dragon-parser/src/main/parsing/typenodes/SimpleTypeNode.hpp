#ifndef SIMPLETYPENODE
#define SIMPLETYPENODE

#include <string>
//#include "../../commandline/TokenList.hpp"
class TokenList;

class SimpleTypeNode {

public:

	SimpleTypeNode(TokenList tokens);
	SimpleTypeNode(std::string typeName);

	std::string typeName;

};

#endif