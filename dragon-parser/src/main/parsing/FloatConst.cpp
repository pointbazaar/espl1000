#include "FloatConst.hpp"
#include "../commandline/TokenList.hpp"
#include "../commandline/TokenKeys.hpp"
#include "../commandline/Token.hpp"

#include <iostream>

using namespace std;

struct FloatConst* makeFloatConst(TokenList* tokens, bool debug){

	struct FloatConst* res = (struct FloatConst*)malloc(sizeof(struct FloatConst));

	if(debug){
		cout << "FloatConst(...)" << endl;
	}

	if(tokens->get(0).kind == FLOATING){
		res->value = stof(tokens->get(0).value);
	}else{
		throw string("could not find a float const");
	}

	return res;
}