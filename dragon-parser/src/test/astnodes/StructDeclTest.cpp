#include "StructDeclTest.hpp"

#include "../../main/commandline/TokenList.hpp"
#include "../../main/commandline/TokenKeys.hpp"
#include "../../main/commandline/Token.hpp"


#include "../../main/parsing/StructDecl.hpp"
#include "../../main/parsing/StructMember.hpp"

int structdecl_test_can_parse_empty_struct_decl() {

	TokenList list = TokenList(); 
	list.add(STRUCT);
	list.add(TYPEIDENTIFIER,"MyStruct");
	list.add(LCURLY);
	list.add(RCURLY);

	StructDecl* s = new StructDecl(&list, false);

	bool assert1 = (0 == s->members.size());

	return (assert1)?1:0;
}

int structdecl_test_will_not_parse_invalid_typename_for_struct() {

	TokenList list = TokenList(); 

	list.add(STRUCT);
	list.add(ID,"myStruct");
	list.add(LCURLY);
	list.add(RCURLY);

	try {
		StructDecl* s = new StructDecl(&list, false);
		return 0;
	} catch (string e) {
		return 1;
	}
}

int structdecl_test_rejects_struct_with_subroutine_type() {

	TokenList list = TokenList(); 

	list.add(STRUCT);
	list.add(LPARENS);
	list.add(RPARENS);
	list.add(ARROW);
	list.add(TYPEIDENTIFIER,"MyStruct");
	list.add(LCURLY);
	list.add(RCURLY);

	try {
		StructDecl* s = new StructDecl(&list, false);
		return 0;
	} catch (string e) {
		return 1;
	}
}

int structdecl_test_can_parse_struct_with_1_member() {

	TokenList list = TokenList();

	list.add(STRUCT);
	list.add(TYPEIDENTIFIER,"MyStruct");
	list.add(LCURLY);
	list.add(TYPEIDENTIFIER,"PInt");
	list.add(ID,"a");
	list.add(RCURLY);

	StructDecl* node = new StructDecl(&list, false);

	bool assert1 = (1 == node->members.size());
	bool assert2 = string("a").compare( node->members.at(0)->name) == 0;

	return (assert1&&assert2)?1:0;
}

int structdecl_test_can_parse_struct_with_2_members() {

	TokenList list = TokenList(); 

	list.add(STRUCT);
	list.add(TYPEIDENTIFIER,"MyStruct");
	list.add(LCURLY);
	list.add(TYPEIDENTIFIER,"PInt");
	list.add(ID,"a");
	list.add(COMMA),
	list.add(TYPEIDENTIFIER,"PInt");
	list.add(ID,"b");
	list.add(RCURLY);

	StructDecl* node = new StructDecl(&list, false);

	bool assert1 = (2 == node->members.size());

	bool assert2 = string("a").compare( node->members.at(0)->name) == 0;

	bool assert3 = string("b").compare( node->members.at(1)->name) == 0;

	return (assert1&&assert2&&assert3)?1:0;
}
