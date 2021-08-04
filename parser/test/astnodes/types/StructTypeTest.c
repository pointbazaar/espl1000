#include <stdio.h>
#include <assert.h>

#include "token/list/TokenList.h"
#include "token/token/token.h"
#include "token/TokenKeys.h"
#include "ast/util/free_ast.h"
#include "types/StructType.h"
#include "StructTypeTest.h"
#include "ast/ast.h"
#include "ast/ast/ast_types.h"

static void status(char* msg){

    printf(" - [TEST] %s\n", msg);
}

int structtype_test(){

    status("structtype_test");

    struct TokenList* list = makeTokenList();
    list_add(list, makeToken2(TYPEID,"MyType") );

    struct StructType* node = makeStructType(list);

    assert(0 == list_size(list));
    assert(node != NULL);
    assert(node->count_type_params == 0);
    assert(node->type_params == NULL);

    freeTokenList(list);
    free_struct_type(node);

    return 1;
}

int structtype_test_type_param(){

    status("structtype_test_type_param");

    struct TokenList* list = makeTokenList();
    list_add(list, makeToken2(TYPEID,"MyType") );

    list_add(list, makeToken2(OPKEY_RELATIONAL,"<") );
    list_add(list, makeToken2(TPARAM,"?T0") );
    list_add(list, makeToken2(OPKEY_RELATIONAL,">") );

    struct StructType* node = makeStructType(list);

    assert(0 == list_size(list));
    assert(node != NULL);

    assert(node->count_type_params == 1);
    assert(node->type_params != NULL);

    freeTokenList(list);
    free_struct_type(node);

    return 1;
}

int structtype_test_generic(){

    status("structtype_test_generic");

    struct TokenList* list = makeTokenList();
    list_add(list, makeToken2(TYPEID,"MyType") );

    list_add(list, makeToken2(OPKEY_RELATIONAL,"<") );
    list_add(list, makeToken2(TYPEID,"OtherType") );
    list_add(list, makeToken2(OPKEY_RELATIONAL,">") );

    struct StructType* node = makeStructType(list);

    assert(0 == list_size(list));
    assert(node != NULL);

    assert(node->count_type_params == 1);
    assert(node->type_params != NULL);

    freeTokenList(list);
    free_struct_type(node);

    return 1;
}
