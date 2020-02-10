#include <vector>
#include <iostream>

#include "TypeParam.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/Token.hpp"
#include "../../commandline/TokenKeys.hpp"

using namespace std;

TypeParam::TypeParam(TokenList tokens, bool debug){

	if(debug){
		cout << "TypeParam(...)" << endl;
	}
	
	Token token = tokens.get(0);

	if (token.kind == TPARAM) {
		this->typeParameterIndex = stoi(token.value);
		tokens.consume(1);
	} else {
		cout << "Error: could not read type parameter" << endl;
		throw string("could not read type parameter node");
	}
}

