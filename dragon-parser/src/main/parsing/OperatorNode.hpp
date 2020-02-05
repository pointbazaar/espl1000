#ifndef OPERATORNODE
#define OPERATORNODE

#include <string>

#include "../commandline/TokenList.hpp"

using namespace std;

class OperatorNode  {

public:
	OperatorNode(TokenList tokens);

	string op;
};

#endif