#include "CharConstTest.h"

#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"

#include "../../main/parsing/CharConst.h"

#include <stdio.h>

int charconst_test_parse_char_constant_node(bool debug) {

	if(debug){
		printf("charconst_test_parse_char_constant_node\n");
	}

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(CCONST,"h"));

	struct CharConst* node = makeCharConst(list,false);
	bool assert1 = ('h'== node->content);

	return (assert1)?1:0;
}

int charconst_test_parse_char_constant_node_newline(bool debug) {

	if(debug){
		printf("charconst_test_parse_char_constant_node_newline\n");
	}

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(CCONST,"\n"));

	struct CharConst* node = makeCharConst(list,false);
	bool assert1 = ('\n' == node->content);

	return (assert1)?1:0;
}
