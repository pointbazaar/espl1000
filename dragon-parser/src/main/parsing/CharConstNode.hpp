#ifndef CHARCONSTNODE
#define CHARCONSTNODE

#include "../commandline/TokenList.hpp"

using namespace std;

class CharConstNode {

public:
	CharConstNode(TokenList tokens,bool debug);

	char content;
};

#endif