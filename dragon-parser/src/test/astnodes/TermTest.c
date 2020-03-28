#include "TermTest.h"
#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"
#include "../../main/parsing/Term.h"

#include <stdio.h>
#include <stdbool.h>

int term_test_simple_term(bool debug) {

	printf("term_test_simple_term()\n");

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(INTEGER,"4"));
	struct Term* expr = makeTerm(list,debug);
	return (expr!=NULL)?1:0;
}

int term_test_variable_term(bool debug) {

	printf("term_test_variable_term()\n");

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(ID,"x"));
	struct Term* expr = makeTerm(list,debug);
	return (expr!=NULL)?1:0;
}


