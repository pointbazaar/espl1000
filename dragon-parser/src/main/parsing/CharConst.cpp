#include <iostream>
#include <vector>

#include "CharConst.hpp"
#include "../commandline/TokenList.hpp"
#include "../commandline/TokenKeys.hpp"

using namespace std;

CharConst::CharConst(TokenList tokens, bool debug) {

	if(debug){
		cout << "CharConst(...)" << endl;
	}

	Token token = tokens.get(0);

	if (token.kind == CCONST) {
		this->content = token.value.at(0);
		tokens.consume(1);
	} else {
		throw string("could not read charConstant node");
	}

}

