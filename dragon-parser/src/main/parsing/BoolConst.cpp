#include <iostream>
#include <vector>

#include "BoolConst.hpp"
#include "../commandline/TokenList.hpp"
#include "../commandline/TokenKeys.hpp"

using namespace std;

BoolConst::BoolConst(TokenList tokens, bool debug) {

	if(debug){
		cout << "BoolConst(...)" << endl;
	}

	TokenList copy = TokenList(tokens);

	if (copy.get(0).kind == BCONST) {
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