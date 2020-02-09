#include <iostream>
#include <vector>

//project headers
#include "CharConstNode.hpp"
#include "../commandline/TokenList.hpp"
#include "../commandline/TokenKeys.hpp"

using namespace std;

CharConstNode::CharConstNode(TokenList tokens, bool debug) {

	if(debug){
		cout << "CharConstNode(...)" << endl;
	}

	Token token = tokens.get(0);

	if (token.kind == CHARCONST) {
		this->content = token.value.at(0);
		tokens.consume(1);
	} else {
		throw ("could not read charConstant node");
	}

}

