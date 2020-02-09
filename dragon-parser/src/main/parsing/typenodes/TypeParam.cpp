#include <vector>

#include "TypeParam.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/Token.hpp"
#include "../../commandline/TokenKeys.hpp"

TypeParam::TypeParam(TokenList tokens){
	
	Token token = tokens.get(0);

	if (token.kind == TPARAM) {
		this->typeParameterIndex = stoi(token.value);
		tokens.consume(1);
	} else {
		throw "could not read type parameter node";
	}
}

