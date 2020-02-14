#include <vector>
#include <string>
#include <iostream>

#include "AST_Whole_Program.hpp"
#include "Namespace.hpp"
#include "../commandline/TokenList.hpp"

using namespace std;

struct AST_Whole_Program* makeAST_Whole_Program(struct Namespace* myNamespace) {
	//utility method, to facilitate creating AST_Whole_Program from a single namespace node

	struct AST_Whole_Program* res = (struct AST_Whole_Program*)malloc(sizeof(struct AST_Whole_Program));

	if(res == NULL){
		printf("could not malloc.\n");
		exit(1);
	}

	res->namespaces = (struct Namespace**)malloc(sizeof(struct Namespace*)*1);

	res->namespaces[0] = myNamespace;

	return res;
}

struct AST_Whole_Program* makeAST_Whole_Program(TokenList* tokens, char* myNamespace, bool debug) {

	//use this constructor as a shortcut to create an AST
	//if there is just a single namespace

	if (debug) {
		cout << "makeAST_Whole_Program(...)" << endl;
	}

	struct AST_Whole_Program* res = (struct AST_Whole_Program*)malloc(sizeof(struct AST_Whole_Program));

	if(res == NULL){
		printf("could not malloc.\n");
		exit(1);
	}

	res->namespaces = (struct Namespace**)malloc(sizeof(struct Namespace*)*1);

	res->namespaces[0] = makeNamespace(tokens, myNamespace, debug);

	return res;
}

