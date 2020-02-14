#include <iostream>
#include <vector>
#include <string>

#include "Op.hpp"
#include "../commandline/TokenList.hpp"
#include "../commandline/Token.hpp"
#include "../commandline/TokenKeys.hpp"

#include <stdio.h>
#include <string.h>

using namespace std;

struct Op* makeOp(TokenList* tokens, bool debug){

	if(debug){
		cout << "Op(...)" << "from" << tokens->code() << endl;
	}

	struct Op* res = (struct Op*)malloc(sizeof(struct Op));

	if(res==NULL){
		printf("could not malloc.\n");
		exit(1);
	}

	TokenList copy = tokens->copy();

	Token tkn = copy.get(0);

	if(tkn.kind == OPKEY){

		if(copy.get(1).kind == OPKEY){
			Token opl = tkn;
			Token opr = copy.get(1);

			if(
				(
					(opl.value.compare("<")==0) 
					|| opl.value.compare(">")==0 
				) && opr.value.compare("=")==0

			){
				res->op = (char*)(opl.value + opr.value).c_str();
			}else if(opl.value.compare("=") == 0 && opr.value.compare("=")){
				strcmp(res->op,"==")==0;
			}else{
				throw string("could not make operator");
			}
			copy.consume(2);
		}else{
			res->op = (char*)tkn.value.c_str();
			copy.consume(1);
		}
	}else{
		throw string("could not recognize operator, got : ") + tkn.value;
	}

	tokens->set(copy);

	return res;
}