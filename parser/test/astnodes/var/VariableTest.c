#include "VariableTest.h"

#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"

#include "var/Variable.h"
#include "var/SimpleVar.h"

#include "../../../token/token.h"
#include "../../../ast/free_ast.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

int variable_test_parse_struct_member_access(bool debug) {

	if(debug){
		printf("TEST: variable_test_parse_struct_member_access(...)\n");
	}

	struct TokenList* tokens = makeTokenList();
	list_add(tokens, makeToken2(ID,"x"));
	list_add(tokens, makeToken2(STRUCTMEMBERACCESS,"."));
	list_add(tokens, makeToken2(ID,"a"));

	struct Variable* v = makeVariable(tokens,debug);
	if(v==NULL){
		return 0;
	}

	assert(1 == v->count_memberAccessList);
	
	assert(strcmp("x", v->simpleVar->name) == 0);

	assert(strcmp("a", v->memberAccessList[0]->simpleVar->name) == 0);
	
	freeTokenList(tokens);
	freeVariable(v);

	return 1;
}

int variable_test_parse_index_access(bool debug) {
	
	if(debug){
		printf("TEST: variable_test_parse_index_access(...)\n");
	}

	struct TokenList* tokens = makeTokenList();
	list_add(tokens, makeToken2(ID,"x"));
	list_add(tokens, makeToken(LBRACKET));
	list_add(tokens, makeToken2(INTEGER,"0"));
	list_add(tokens, makeToken(RBRACKET));

	struct Variable* node = makeVariable(tokens,debug);
	assert(node != NULL);

	assert(0 == node->count_memberAccessList);
	assert(strcmp("x", node->simpleVar->name) == 0);
	
	freeTokenList(tokens);
	freeVariable(node);

	return 1;
}

int variable_test_parse_struct_member_access_and_index_access(bool debug) {

	if(debug){
		printf("TEST: variable_test_parse_struct_member_access_and_index_access(...)\n");
	}

	// x.a[0].b
	struct TokenList* tokens = makeTokenList();
	list_add(tokens, makeToken2(ID,"x") );
	list_add(tokens, makeToken2(STRUCTMEMBERACCESS,".") );
	list_add(tokens, makeToken2(ID,"a") );

	list_add(tokens, makeToken2(LBRACKET,"[") );
	list_add(tokens, makeToken2(INTEGER,"0") );
	list_add(tokens, makeToken2(RBRACKET,"]") );

	list_add(tokens, makeToken2(STRUCTMEMBERACCESS,".") );
	list_add(tokens, makeToken2(ID,"b") );

	struct Variable* node = makeVariable(tokens,debug);
	assert(node != NULL);

	assert(1 == node->count_memberAccessList);
	assert(strcmp("x", node->simpleVar->name) == 0);
	assert(strcmp("a", node->memberAccessList[0]->simpleVar->name) == 0);
	
	freeTokenList(tokens);
	freeVariable(node);

	return 1;
}
