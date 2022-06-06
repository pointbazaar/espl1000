#include <stdio.h>
#include <stdlib.h>

#include "expr/Expr.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "MDirect.h"
#include "parse_astnode.h"

const char* PARSE_ERR_MDIRECT = "Failed to parse MDirect. Exiting\n";

struct MDirect* makeMDirect(struct TokenList* tokens){

    struct MDirect* res = make(MDirect);

    struct TokenList* copy = list_copy(tokens);

    parse_astnode(copy, &(res->super));

    struct Token* head = list_head(copy);

    if(head->kind != LBRACKET){
        freeTokenListShallow(copy);
        free(res);
        return NULL;
    }

    list_consume(copy, 1);

    res->expr = makeExpr(copy);

    if(res->expr == NULL){
        freeTokenListShallow(copy);
        free(res);
        return NULL;
    }

    head = list_head(copy);

    if(head->kind != RBRACKET){
        freeTokenListShallow(copy);
        free(res);
        return NULL;
    }

    list_consume(copy, 1);

    list_set(tokens, copy);
    freeTokenListShallow(copy);

    return res;
}