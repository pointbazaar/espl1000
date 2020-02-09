#include <vector>
#include <iostream>

#include "BasicTypeWrapped.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/TokenKeys.hpp"
#include "SubrType.hpp"
#include "SimpleType.hpp"

using namespace std;

BasicTypeWrapped::BasicTypeWrapped(SimpleType* typeNode) {
	this->m1 = typeNode;
}

BasicTypeWrapped::BasicTypeWrapped(SubrType* typeNode) {
	this->m2 = typeNode;
}

BasicTypeWrapped::BasicTypeWrapped(TokenList tokens, bool debug) {

	if(debug){
		cout << "BasicTypeWrapped(...)" << endl;
	}

	TokenList copy = tokens.copy();

	if (copy.size() > 1 && copy.get(0).tokenEquals(Token(LPARENS))) {
		TokenList copy2 = copy.copy();

		copy2.expect(Token(LPARENS));
		this->m2 = new SubrType(copy2,debug);
		copy2.expect(Token(RPARENS));

		copy.set(copy2);

	} else {
		this->m1 = new SimpleType(copy,debug);
	}
	tokens.set(copy);
}

