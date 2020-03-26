#include "DeclArgTest.h"

#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"

#include "../../main/parsing/DeclArg.h"

int declarg_test_parse_declared_argument() {
	string source = "((PInt)~>PInt) subr";

	TokenList* list = new TokenList();

	list->add(LPARENS);
	list->add(LPARENS);
	list->add(TYPEIDENTIFIER,"PInt");
	list->add(LPARENS);
	list->add(ARROW);
	list->add(TYPEIDENTIFIER,"PInt");
	list->add(RPARENS);
	list->add(ID,"subr");

	struct DeclArg* node = makeDeclArg(list,false);

	bool assert1 = string("subr").compare( node->name) == 0;
	bool assert2 = (0 == list->size());

	return (assert1&&assert2)?1:0;
}

