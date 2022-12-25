#include <stdio.h>
#include <assert.h>
#include <parser/test/test_parser_util.h>

#include "token/list/TokenList.h"
#include "token/token/token.h"
#include "token/TokenKeys.h"
#include "ast/util/free_ast.h"
#include "types/StructType.h"
#include "StructTypeTest.h"
#include "ast/ast/ast_types.h"


int structtype_test(){

    status_test("structtype_test");

    struct TokenList* list = makeTokenList();
    list_add(list, makeToken2(TYPEID,"MyType") );

    struct StructType* node = makeStructType(list);

    assert(0 == list_size(list));
    assert(node != NULL);

    freeTokenList(list);
    free_struct_type(node);

    return 1;
}

