#include <iostream>
#include <vector>

#include "IntConst.hpp"
#include "../commandline/TokenList.hpp"
#include "../commandline/TokenKeys.hpp"

using namespace std;

struct IntConst* makeIntConst(TokenList* tokens, bool debug) {

	struct IntConst* res = (struct IntConst*)malloc(sizeof(struct IntConst));

	if(debug){
		cout << "IntConst(...)" << " from " << tokens->code() << endl;
	}

	TokenList copy = TokenList(*tokens);

	Token tk = copy.get(0);

	switch (tk.kind){

		case OPKEY: 
			;
			if (tk.value.compare("-")==0 && (copy.get(1).kind == INTEGER)) {
				res->number = - stoi( copy.get(1).value );
				copy.consume(2);
			} else {
				throw string("cannot parse integer constant node with such operator:") + (string)tk.value;
			}
			break;

		case INTEGER: 
			;
			res->number = stoi(tk.value);
			copy.consume(1);
			break;

		default:
			;
			throw string("could not read IntConst node");
	}

	tokens->set(copy);

	return res;
}

