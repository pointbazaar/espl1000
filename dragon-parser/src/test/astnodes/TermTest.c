#include "TermTest.h"
#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"
#include "../../main/parsing/Term.h"

#include <stdio.h>

int term_test_simple_term(bool debug) {

	printf("term_test_simple_term()\n");

	try {
		TokenList list = TokenList();
		list.add(INTEGER,"4");
		struct Term* expr = makeTerm(&list,debug);
		return 1;
	}catch (string e){
		return 0;
	}
}

int term_test_variable_term(bool debug) {


	printf("term_test_variable_term()\n");

	try {
		TokenList list = TokenList();
		list.add(ID,"x");
		struct Term* expr = makeTerm(&list,debug);
		return 1;
	}catch (string e){
		return 0;
	}
}


