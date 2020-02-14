#include "NamespaceTest.hpp"

#include "../../main/commandline/TokenList.hpp"
#include "../../main/commandline/TokenKeys.hpp"
#include "../../main/commandline/Token.hpp"

#include "../../main/parsing/Namespace.hpp"
#include "../../main/parsing/StructDecl.hpp"

int namespace_test_can_parse_namespace_with_1_empty_struct() {

	TokenList l = TokenList();

	l.add(STRUCT);
	l.add(TYPEIDENTIFIER,"MyStruct");
	l.add(LCURLY);
	l.add(RCURLY);

	struct Namespace* n = makeNamespace(&l,"Main", false);

	bool assert1 = (1 == n->count_structs);
	bool assert2 = (0 == n->structs[0]->count_members);

	return (assert1&&assert2)?1:0;
}

int namespace_test_can_parse_namespace_with_1_empty_method() {

	//TODO:
	//these parser tests are problematic, because they don't just test the parser,
	//they also test the lexer.
	//i should write the tests so that the parser alone can accomplic shem

	TokenList l = TokenList();

	l.add(FN);
	l.add(ID,"main");
	l.add(LPARENS);
	l.add(TYPEIDENTIFIER,"String");
	l.add(ID,"hello");
	l.add(RPARENS);
	l.add(ARROW);
	l.add(TYPEIDENTIFIER,"PInt");
	l.add(LCURLY);

	l.add(RCURLY);

	struct Namespace* n = makeNamespace(&l,"Main", false);

	bool assert1 = string("Main").compare(n->name) == 0;
	bool assert2 = (1 == n->count_methods);

	return (assert1&&assert2)?1:0;
}

