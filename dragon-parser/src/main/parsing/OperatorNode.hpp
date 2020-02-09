#ifndef OPERATORNODE
#define OPERATORNODE

#include <string>

#include "../commandline/TokenList.hpp"

using namespace std;

class OperatorNode  {

public:
	OperatorNode(TokenList tokens, bool debug);

	string op;
};

#endif