
//standard headers
#include <vector>

//project headers
#include "TypeParameterNode.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/BaseToken.hpp"
#include "../../commandline/TokenKeys.hpp"

//using namespace std;

TypeParameterNode::TypeParameterNode(TokenList tokens){
	
	BaseToken token = tokens.get(0);

	if (token.kind == TYPEPARAM) {
		this->typeParameterIndex = stoi(token.value);
		tokens.consume(1);
	} else {
		throw "could not read type parameter node";
	}
}

