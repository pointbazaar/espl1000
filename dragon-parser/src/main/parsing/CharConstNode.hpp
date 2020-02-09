#ifndef CHARCONSTNODE
#define CHARCONSTNODE

#include "../commandline/TokenList.hpp"

class CharConstNode {

public:
	CharConstNode(TokenList tokens,bool debug);

	char content;
};

#endif