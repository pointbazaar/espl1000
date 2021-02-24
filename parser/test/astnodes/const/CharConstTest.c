#include "CharConstTest.h"

#include "../../../main/commandline/TokenList.h"
#include "../../../main/commandline/TokenKeys.h"
#include "../../../main/parsing/const/CharConst.h"
#include "../../../../token/token.h"
#include "../../../../ast/free_ast.h"

#include <stdio.h>
#include <assert.h>

int charconst_test_parse_char_constant_node(bool debug) {

	if(debug){
		printf("TEST: charconst_test_parse_char_constant_node\n");
	}

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(CCONST,"'h'"));

	struct CharConst* node = makeCharConst(list, debug);
	
	assert(node != NULL);
	assert('h'== node->value);
	
	if(debug){
		printf("node->value == %c\n", node->value);
	}
	
	freeTokenList(list);
	freeCharConst(node);

	return 1;
}

int charconst_test_parse_char_constant_node_newline(bool debug) {

	if(debug){
		printf("TEST: charconst_test_parse_char_constant_node_newline\n");
	}

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(CCONST,"'\n'"));

	struct CharConst* node = makeCharConst(list, debug);
	assert('\n' == node->value);
	
	freeTokenList(list);
	freeCharConst(node);

	return 1;
}
