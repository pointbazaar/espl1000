
//standard headers
#include <vector>
#include <set>

//project headers
#include "CharConstNode.hpp"
#include "../commandline/TokenList.hpp"
#include "../commandline/TokenKeys.hpp"

using namespace std;

CharConstNode::CharConstNode(TokenList tokens) {

	BaseToken token = tokens.get(0);

	if (token.kind == CHARCONST) {
		this->content = token.value.at(0);
		tokens.consume(1);
	} else {
		throw ("could not read charConstant node");
	}

}

