#include <vector>
#include <optional>
#include <string>
#include <iostream>

#include "DeclArg.hpp"
#include "../commandline/TokenList.hpp"
#include "typenodes/Type.hpp"
#include "Identifier.hpp"

using namespace std;

struct DeclArg* makeDeclArg(TokenList* tokens, bool debug) {

	struct DeclArg* res = (struct DeclArg*)malloc(sizeof(struct DeclArg));

	if(debug){
		cout << "DeclaredArg(...)" << endl;
		cout << "from " << tokens->code() << endl;
	}

	TokenList copy = tokens->copy();

	res->type = makeType(&copy,debug);

	try{
		res->name = NULL;
		res->name = makeIdentifier(&copy,debug)->identifier;
	}catch(string e){
		//pass
	}

	tokens->set(copy);

	return res;
}

