#include "CharConstTest.hpp"

#include "../../main/commandline/TokenList.hpp"
#include "../../main/commandline/TokenKeys.hpp"
#include "../../main/commandline/Token.hpp"

#include "../../main/parsing/CharConst.hpp"

int charconst_test_parse_char_constant_node() {
	TokenList* list = new TokenList();
	list->add(CCONST,"h");

	CharConst* node = new CharConst(*list,false);
	bool assert1 = ('h'== node->content);

	return (assert1)?1:0;
}

int charconst_test_parse_char_constant_node_newline() {

	TokenList* list = new TokenList();
	list->add(CCONST,"\n");

	CharConst* node = new CharConst(*list,false);
	bool assert1 = ('\n' == node->content);

	return (assert1)?1:0;
}
