#ifndef CHARCONSTNODE
#define CHARCONSTNODE

#include "ITermNode.hpp"
#include "../lexing/TokenList.hpp"

using namespace std;

class CharConstNode : ITermNode {

public:
	CharConstNode(TokenList tokens);

	char content;
};

#endif