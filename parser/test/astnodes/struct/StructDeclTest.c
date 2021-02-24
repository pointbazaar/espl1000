#include "StructDeclTest.h"
#include "../../../main/commandline/TokenList.h"
#include "../../../main/commandline/TokenKeys.h"
#include "../../../main/astnodes/struct/StructDecl.h"
#include "../../../main/astnodes/struct/StructMember.h"
#include "../../../../token/token.h"
#include "../../../../ast/free_ast.h"

#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

int structdecl_test_can_parse_empty_struct_decl(bool debug) {

	if(debug){
		printf("TEST: structdecl_test_can_parse_empty_struct_decl\n");
	}

	struct TokenList* list = makeTokenList(); 

	list_add(list, makeToken(STRUCT) );
	list_add(list, makeToken2(TYPEID,"MyStruct") );
	list_add(list, makeToken(LCURLY) );
	list_add(list, makeToken(RCURLY) );

	struct StructDecl* s = makeStructDecl(list, debug);

	assert(0 == s->count_members);
	
	freeTokenList(list);
	freeStructDecl(s);

	return 1;
}

int structdecl_test_will_not_parse_invalid_typename_for_struct(bool debug) {

	if(debug){
		printf("TEST: structdecl_test_will_not_parse_invalid_typename_for_struct\n");
	}

	struct TokenList* list = makeTokenList(); 

	list_add(list, makeToken(STRUCT));
	list_add(list, makeToken2(ID,"myStruct"));
	list_add(list, makeToken(LCURLY));
	list_add(list, makeToken(RCURLY));

	struct StructDecl* s = makeStructDecl(list, debug);
	assert(s != NULL);
	
	freeTokenList(list);
	freeStructDecl(s);
	
	return 1;
}

int structdecl_test_rejects_struct_with_subroutine_type(bool debug) {

	if(debug){
		printf("TEST: structdecl_test_rejects_struct_with_subroutine_type\n");
	}

	struct TokenList* list = makeTokenList(); 

	list_add(list, makeToken(STRUCT) );
	list_add(list, makeToken(LPARENS) );
	list_add(list, makeToken(RPARENS) );
	list_add(list, makeToken(ARROW) );
	list_add(list, makeToken2(TYPEID,"MyStruct") );
	list_add(list, makeToken(LCURLY) );
	list_add(list, makeToken(RCURLY) );

	struct StructDecl* s = makeStructDecl(list, debug);
	assert(s != NULL);
	
	freeTokenList(list);
	freeStructDecl(s);
	
	return 1;
}

int structdecl_test_can_parse_struct_with_1_member(bool debug) {

	if(debug){
		printf("TEST: structdecl_test_can_parse_struct_with_1_member\n");
	}

	struct TokenList* list = makeTokenList();

	list_add(list, makeToken(STRUCT) );
	list_add(list, makeToken2(TYPEID,"MyStruct") );
	list_add(list, makeToken(LCURLY) );
	list_add(list, makeToken2(TYPEID,"uint") );
	list_add(list, makeToken2(ID,"a") );
	list_add(list, makeToken(SEMICOLON) ),
	list_add(list, makeToken(RCURLY) );

	struct StructDecl* node = makeStructDecl(list, debug);

	assert(1 == node->count_members);
	assert(strcmp("a", node->members[0]->name) == 0);
	
	freeTokenList(list);
	freeStructDecl(node);

	return 1;
}

int structdecl_test_can_parse_struct_with_2_members(bool debug) {

	if(debug){
		printf("TEST: structdecl_test_can_parse_struct_with_2_members\n");
	}

	struct TokenList* list = makeTokenList(); 

	list_add(list, makeToken(STRUCT) );
	list_add(list, makeToken2(TYPEID,"MyStruct") );
	list_add(list, makeToken(LCURLY) );
	list_add(list, makeToken2(TYPEID,"uint") );
	list_add(list, makeToken2(ID,"a") );
	list_add(list, makeToken(SEMICOLON) ),
	list_add(list, makeToken2(TYPEID,"uint") );
	list_add(list, makeToken2(ID,"b") );
	list_add(list, makeToken(SEMICOLON) ),
	list_add(list, makeToken(RCURLY) );

	struct StructDecl* node = makeStructDecl(list, debug);

	assert(2 == node->count_members);

	assert(strcmp("a", node->members[0]->name) == 0);

	assert(strcmp("b", node->members[1]->name) == 0);
	
	freeTokenList(list);
	freeStructDecl(node);

	return 1;
}
