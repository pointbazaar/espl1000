#include <iostream>
#include <vector>

#include "BoolConst.hpp"
#include "../commandline/TokenList.hpp"
#include "../commandline/TokenKeys.hpp"

#include <stdio.h>

using namespace std;

struct BoolConst* makeBoolConst(TokenList* tokens, bool debug) {

	struct BoolConst* res = (struct BoolConst*)malloc(sizeof(struct BoolConst));

	if(res==NULL){
		printf("could not malloc.\n");
		exit(1);
	}

	if(debug){
		cout << "BoolConst(...)" << endl;
	}

	TokenList copy = TokenList(*tokens);

	if (copy.get(0).kind == BCONST) {
		Token tk = copy.get(0);

		if(tk.value != "" && tk.value.compare("true")==0) {
			res->boolValue = true;
		}else {
			res->boolValue = false;
		}
		copy.consume(1);
	} else {
		throw string("could not read Bool Constant node");
	}

	tokens->set(copy);

	return res;
}