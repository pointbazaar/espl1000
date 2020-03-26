#include "CharConstTest.h"

#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"

#include "../../main/parsing/CharConst.h"

int charconst_test_parse_char_constant_node() {
	TokenList* list = new TokenList();
	list->add(CCONST,"h");

	struct CharConst* node = makeCharConst(list,false);
	bool assert1 = ('h'== node->content);

	return (assert1)?1:0;
}

int charconst_test_parse_char_constant_node_newline() {

	TokenList* list = new TokenList();
	list->add(CCONST,"\n");

	struct CharConst* node = makeCharConst(list,false);
	bool assert1 = ('\n' == node->content);

	return (assert1)?1:0;
}
