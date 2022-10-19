#include <stdio.h>
#include <stdlib.h>

#include "tac/tac.h"
#include "tac/tacbuffer.h"
#include "gen_tac.h"

void tac_massignstmt(struct TACBuffer* buffer, struct MAssignStmt* m){

    struct Expr* lhs = m->lhs->expr;

    if(lhs->term1->term->kind != 12){
        printf("non-const memory assign currently unsupported. exiting\n");
        exit(1);
    }

    if(lhs->term1->term->ptr.m12->kind == 4){
        printf("non-int-address memory assign currently unsupported. exiting\n");
        exit(1);
    }

    const uint32_t addr = lhs->term1->term->ptr.m12->ptr.m2_int_const->value;

    tac_expr(buffer, m->expr);
    
    struct TAC* tstore = makeTACStoreConstAddr(addr, tacbuffer_last_dest(buffer));

    tacbuffer_append(buffer, tstore, true);
}
