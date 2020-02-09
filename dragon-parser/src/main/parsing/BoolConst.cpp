#include <iostream>
#include <vector>

#include "BoolConst.hpp"
#include "../commandline/TokenList.hpp"
#include "../commandline/TokenKeys.hpp"

using namespace std;

struct BoolConst* makeBoolConst(TokenList tokens, bool debug) {

	struct BoolConst* res = (struct BoolConst*)malloc(sizeof(BoolConst));

	if(debug){
		cout << "BoolConst(...)" << endl;
	}

	TokenList copy = TokenList(tokens);

	if (copy.get(0).kind == BCONST) {
		Token tk = copy.get(0);

		if(tk.value.compare("true")==0){
			res->boolValue=true;
		}else  if (tk.value.compare("false")==0){
			res->boolValue=true;
		}else{
			throw "could not read Bool Constant node";
		}
		
		copy.consume(1);
	} else {
		throw "could not read Bool Constant node";
	}

	tokens.set(copy);

	return res;
}