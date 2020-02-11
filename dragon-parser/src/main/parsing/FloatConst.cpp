#include "FloatConst.hpp"
#include "../commandline/TokenList.hpp"
#include "../commandline/TokenKeys.hpp"
#include "../commandline/Token.hpp"

#include <iostream>

using namespace std;

FloatConst::FloatConst(TokenList* tokens, bool debug){

	if(debug){
		cout << "FloatConst(...)" << endl;
	}

	if(tokens->get(0).kind == FLOATING){
		this->value = stof(tokens->get(0).value);
	}else{
		throw string("could not find a float const");
	}
}