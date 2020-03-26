#include "SimpleVarTest.h"
#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"
#include "../../main/parsing/SimpleVar.h"
#include "../../main/parsing/IntConst.h"
#include "../../main/parsing/Expr.h"
#include "../../main/parsing/Term.h"

#include <stdio.h>

int simplevar_test_parse_simple_variable(bool debug) {

	printf("simplevar_test_parse_simple_variable()\n");

	TokenList list = TokenList();
	list.add(ID,"x");

	struct SimpleVar* node = makeSimpleVar(&list,debug);
	bool assert1 = string("x").compare(node->name)==0;

	return (assert1)?1:0;
}

int simplevar_test_parse_simple_indexed_variable(bool debug) {

	printf("simplevar_test_parse_simple_indexed_variable()\n");

	TokenList list = TokenList();
	list.add(ID,"x");
	list.add(LBRACKET,"[");
	list.add(INTEGER,"0");
	list.add(RBRACKET,"]");


	struct SimpleVar* node = makeSimpleVar(&list,debug);
	bool assert1 = string("x").compare( node->name) == 0;
	bool assert2 = (node->indexOptional != NULL);

	bool assert3 = (
			0 == 
			(node->indexOptional->term1->m2)	//IntConst
			->value
	);

	return (assert1&&assert2&&assert3)?1:0;
}
