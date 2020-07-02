#include "SimpleVarTest.h"
#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"
#include "../../main/parsing/SimpleVar.h"
#include "../../main/parsing/IntConst.h"
#include "../../main/parsing/Expr.h"
#include "../../main/parsing/Term.h"

#include <stdio.h>
#include <stdbool.h>

int simplevar_test_parse_simple_variable(bool debug) {

	printf("simplevar_test_parse_simple_variable()\n");

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(ID,"x") );

	struct SimpleVar* node = makeSimpleVar(list,debug);
	if(node==NULL){
		return 0;
	}
	bool assert1 = strcmp("x", node->name)==0;

	return (assert1)?1:0;
}

int simplevar_test_parse_simple_indexed_variable(bool debug) {

	printf("simplevar_test_parse_simple_indexed_variable()\n");

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken2(ID,"x")) ;
	list_add(list, makeToken2(LBRACKET,"[")) ;
	list_add(list, makeToken2(INTEGER,"0")) ;
	list_add(list, makeToken2(RBRACKET,"]")) ;


	struct SimpleVar* node = makeSimpleVar(list,debug);
	if(node==NULL){
		return 0;
	}

	bool assert1 = strcmp("x", node->name) == 0;
	bool assert2 = (node->indexOptional != NULL);

	bool assert3 = (
			0 == 
			(node->indexOptional->term1->m2)	//IntConst
			->value
	);

	return (assert1&&assert2&&assert3)?1:0;
}
