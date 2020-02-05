#ifndef CHARCONSTNODE
#define CHARCONSTNODE

#include "ITermNode.hpp"
#include "../commandline/TokenList.hpp"

using namespace std;

class CharConstNode : public ITermNode {

public:
	CharConstNode(TokenList tokens);

	char content;
};

#endif