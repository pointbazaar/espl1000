#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "parser/test/test_parser_util.h"

#include "statements/MAssignStmt.h"
#include "expr/Expr.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "ast/util/free_ast.h"

#include "MAssignStmtTest.h"

bool massignstmt_test1(){

    status_test("massignstmt_test1");

    struct TokenList* tokens = makeTokenList();

    list_add(tokens, makeToken(LBRACKET));
    list_add(tokens, makeToken2(INTEGER,"4"));
    list_add(tokens, makeToken(RBRACKET));
    list_add(tokens, makeToken2(ASSIGNOP,"="));

    list_add(tokens, makeToken2(INTEGER,"5"));

    list_add(tokens, makeToken(SEMICOLON));

    struct MAssignStmt* a = makeMAssignStmt(tokens);

    assert(a != NULL);

    assert(a->lhs->expr->term1->term->kind == 12); //constvalue
    assert(a->lhs->expr->term1->term->ptr.m12->ptr.m2_int_const->value == 4);

    assert(a->expr != NULL);
    assert(a->expr->term1->term->kind == 12); //constvalue
    assert(a->expr->term1->term->ptr.m12->ptr.m2_int_const->value == 5);

    free_massign_stmt(a);
    freeTokenList(tokens);

    return true;
}