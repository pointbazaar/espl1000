#include <iostream>
#include <vector>

#include "IntConst.hpp"
#include "../commandline/TokenList.hpp"
#include "../commandline/TokenKeys.hpp"

using namespace std;

IntConst::IntConst(TokenList tokens, bool debug) {

	if(debug){
		cout << "IntConst(...)" << endl;
	}

	TokenList copy = TokenList(tokens);

	if (copy.get(0).kind == OPKEY) {

		Token tk = copy.get(0);
		if (tk.value.compare("-")==0 && (copy.get(1).kind == INTEGER)) {
			this->number = - stoi( copy.get(1).value );
			copy.consume(2);
		} else {
			throw "cannot parse integer constant node with such operator:" + (string)tk.value;
		}
	} else if (copy.get(0).kind == INTEGER) {
		this->number = stoi( copy.get(0).value);
		copy.consume(1);
	} else {
		throw "could not read IntConst node";
	}
	tokens.set(copy);
}

