#include <stdio.h>
#include <assert.h>
#include <parser/test/test_parser_util.h>

#include "ParseTypeTests.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "types/PointerType.h"

#include "ast/util/free_ast.h"

int pointertype_test_type_parsing() {

	status_test((char*)__func__);

	struct TokenList* list = makeTokenList();
	list_add(list, makeToken(OPKEY_ARITHMETIC_MUL));
	list_add(list, makeToken(TYPEID_PRIMITIVE_UINT32));

	struct PointerType* pt = makePointerType2(list);

	assert(pt->element_type != NULL);

	assert(pt->element_type->basic_type != NULL);
	assert(pt->element_type->basic_type->simple_type != NULL);

	struct SimpleType* st = pt->element_type->basic_type->simple_type;

	assert(st->primitive_type != NULL);
	assert(st->primitive_type->is_int_type);
	assert(st->primitive_type->int_type == UINT32);

	assert(0 == list_size(list));

	freeTokenList(list);
	free_pointer_type(pt);

	return 1;
}
