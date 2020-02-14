#include <vector>
#include <iostream>

#include "TypeParam.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/Token.hpp"
#include "../../commandline/TokenKeys.hpp"

#include <stdio.h>

using namespace std;

struct TypeParam* makeTypeParam(TokenList* tokens, bool debug){

	if(debug){
		cout << "TypeParam(...)" << endl;
	}

	struct TypeParam* res = (struct TypeParam*)malloc(sizeof(struct TypeParam));

	if(res==NULL){
		printf("could not malloc.\n");
		exit(1);
	}
	
	Token token = tokens->get(0);

	if (token.kind == TPARAM) {
		res->typeParameterIndex = stoi(token.value);
		tokens->consume(1);
	} else {
		cout << "Error: could not read type parameter" << endl;
		throw string("could not read type parameter node");
	}

	return res;
}

