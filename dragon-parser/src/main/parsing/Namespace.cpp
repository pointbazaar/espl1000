#include <vector>
#include <string>
#include <iostream>

#include "Namespace.hpp"
#include "Method.hpp"
#include "StructDecl.hpp"
#include "../commandline/TokenList.hpp"
#include "../commandline/Token.hpp"
#include "../commandline/TokenKeys.hpp"

#include <stdio.h>

using namespace std;

struct Namespace* makeNamespace(TokenList* tokens, char* name, bool debug) {

	if (debug) {
		cout << "Namespace(...)" << endl;
		cout << "from: " + tokens->code() << endl;
	}

	struct Namespace* res = (struct Namespace*)malloc(sizeof(struct Namespace));

	if(res==NULL){
		printf("could not malloc.\n");
		exit(1);
	}

	res->count_methods = 0;
	res->count_structs = 0;

	res->methods = (struct Method**)malloc(sizeof(struct Method*)*1);
	res->structs = (struct StructDecl**)malloc(sizeof(struct StructDecl*)*1);

	res->srcPath = (char*)"/dev/null";
	res->name = name;
	TokenList copy_1 = tokens->copy();
	TokenList* copy = &copy_1;

	
	if(copy->size()>0) {

		Token next_struct = copy->get(0);

		while (next_struct.kind == STRUCT) {
			res->structs[res->count_structs++] = makeStructDecl(copy, debug);
			res->structs = (struct StructDecl**)realloc(res->structs,sizeof(struct StructDecl*)*(res->count_structs+1));

			if (copy->size() > 0) {
				next_struct = copy->get(0);
			} else {
				break;
			}
		}
	}
	

	//it is be nice to have prefix 'fn' before a function
	//to make parsing easier.
	//this does not add much boilerplate to the syntax
	//and would probably make the parser faster
	
	if (copy->size() > 0) {

		Token next_subr = copy->get(0);

		while (next_subr.kind == FN) {
			res->methods[res->count_methods++] = makeMethod(copy, debug);
			res->methods = (struct Method**)realloc(res->methods,sizeof(struct Method*)*(res->count_methods+1));

			if (copy->size() > 0) {
				next_subr = copy->get(0);
			} else {
				break;
			}
		}
	}

	if(debug){
		cout << "done parsing Namespace Node" << endl;
	}

	tokens->set(*copy);

	return res;
}

