#include "VariableTest.h"
#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"
#include "../../main/parsing/Variable.h"
#include "../../main/parsing/SimpleVar.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

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

	bool assert1 = (1 == v->count_memberAccessList);
	
	bool assert2 = strcmp("x", v->simpleVar->name) == 0;
	
	bool assert3 = !(v->simpleVar->indexOptional != NULL);
	
	bool assert4 = strcmp("a", v->memberAccessList[0]->simpleVar->name) == 0;
	
	bool assert5 = !(v->memberAccessList[0]->simpleVar->indexOptional != NULL);

	return (assert1&&assert2&&assert3&&assert4&&assert5)?1:0;
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
	if(node==NULL){
		return 0;
	}

	bool assert1 = (0 == node->count_memberAccessList);
	bool assert2 = strcmp("x", node->simpleVar->name) == 0;
	bool assert3 = (node->simpleVar->indexOptional != NULL);

	return (assert1&&assert2&&assert3)?1:0;
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

	if(node==NULL){
		return 0;
	}

	bool assert1 = (1 == node->count_memberAccessList);
	bool assert2 = strcmp("x", node->simpleVar->name) == 0;

	bool assert3 = strcmp("a", node->memberAccessList[0]->simpleVar->name) == 0;
	bool assert4 = (node->memberAccessList[0]->simpleVar->indexOptional != NULL);

	return (assert1&&assert2&&assert3&&assert4)?1:0;
}
