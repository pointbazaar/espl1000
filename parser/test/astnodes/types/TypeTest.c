#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#include "parser/test/test_parser_util.h"

#include "TypeTest.h"

#include "types/Type.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "ast/util/free_ast.h"

int type_test_type_params(){
	
	status_test("type_test_type_params");

	struct TokenList* list = makeTokenList();
	
	list_add(list, makeToken2(TYPEID,"List"));
	list_add(list, makeToken2(OPKEY_RELATIONAL,"<"));
	list_add(list, makeToken2(TYPEID,"List"));
	list_add(list, makeToken2(OPKEY_RELATIONAL,"<"));
	list_add(list, makeToken2(TYPEID_PRIMITIVE_INT,"int"));
	list_add(list, makeToken2(OPKEY_RELATIONAL,">"));
	list_add(list, makeToken2(OPKEY_RELATIONAL,">"));

	struct Type* node = makeType2(list);

	assert(0 == list_size(list));
	assert(node != NULL);
	
	assert(node->m1 != NULL);
	assert(node->m1->simple_type != NULL);
	assert(node->m1->simple_type->struct_type != NULL);
	
	struct StructType* stype = node->m1->simple_type->struct_type;
	
	assert(stype->count_type_params == 1);
	assert(stype->type_params[0] != NULL);
	
	struct Type* first = stype->type_params[0];
	
	assert(first->m1 != NULL);
	assert(first->m1->simple_type != NULL);
	assert(first->m1->simple_type->struct_type != NULL);
	assert(first->m1->simple_type->struct_type->count_type_params == 1);
	assert(first->m1->simple_type->struct_type->type_params[0] != NULL);
	
	struct Type* second = first->m1->simple_type->struct_type->type_params[0];
	
	assert(second->m1 != NULL);
	assert(second->m1->simple_type != NULL);
	assert(second->m1->simple_type->primitive_type != NULL);
	assert(second->m1->simple_type->primitive_type->is_int_type);
	
	freeTokenList(list);
	free_type(node);

	return 1;
}
