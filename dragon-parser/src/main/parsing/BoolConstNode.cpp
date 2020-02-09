#include <iostream>
#include <vector>

#include "BoolConstNode.hpp"
#include "../commandline/TokenList.hpp"
#include "../commandline/TokenKeys.hpp"

using namespace std;

BoolConstNode::BoolConstNode(TokenList tokens, bool debug) {

	if(debug){
		cout << "BoolConstNode(...)" << endl;
	}

	TokenList copy = TokenList(tokens);

	if (copy.get(0).kind == BOOLCONST) {
		Token tk = copy.get(0);

		if(tk.value.compare("true")==0){
			this->boolValue=true;
		}else  if (tk.value.compare("false")==0){
			this->boolValue=true;
		}else{
			throw "could not read Bool Constant node";
		}
		
		copy.consume(1);
	} else {
		throw "could not read Bool Constant node";
	}

	tokens.set(copy);
}