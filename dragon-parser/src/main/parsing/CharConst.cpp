#include <iostream>
#include <vector>

#include "CharConst.hpp"
#include "../commandline/TokenList.hpp"
#include "../commandline/TokenKeys.hpp"

#include <stdio.h>

using namespace std;

struct CharConst* makeCharConst(TokenList* tokens, bool debug) {

	if(debug){
		cout << "CharConst(...)" << endl;
	}

	struct CharConst* res = (struct CharConst*)malloc(sizeof(struct CharConst));

	if(res == NULL){
		printf("could not malloc.\n");
		exit(1);
	}

	Token token = tokens->get(0);

	if (token.kind == CCONST) {
		res->content = token.value.at(0);
		tokens->consume(1);
	} else {
		throw string("could not read charConstant node");
	}

	return res;
}

