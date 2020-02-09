#include <iostream>
#include <vector>
#include <string>

#include "Op.hpp"
#include "../commandline/TokenList.hpp"
#include "../commandline/Token.hpp"
#include "../commandline/TokenKeys.hpp"

using namespace std;

Op::Op(TokenList tokens, bool debug){

	if(debug){
		cout << "Op(...)" << endl;
	}

	TokenList copy = tokens.copy();

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
				this->op = opl.value + opr.value;
			}else if(opl.value.compare("=") == 0 && opr.value.compare("=")){
				this->op = "==";
			}else{
				throw "could not make operator";
			}
			copy.consume(2);
		}else{
			this->op = tkn.value;
			copy.consume(1);
		}
	}else{
		throw "could not recognize operator, got : " + tkn.value;
	}

	tokens.set(copy);
}