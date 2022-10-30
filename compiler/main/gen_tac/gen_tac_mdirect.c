
#include <stdio.h>
#include <stdlib.h>
#include "gen_tac.h"
#include "tac/tacbuffer.h"

void tac_mdirect(struct TACBuffer* buffer, struct MDirect* m){

    struct Expr* expr = m->expr;

    if(expr->term1->term->kind != 12){
        printf("non-const memory load currently unsupported. exiting\n");
        exit(1);
    }

    const uint32_t addr = expr->term1->term->ptr.m12->ptr.m2_int_const;

    struct TAC* t = makeTACLoadConstAddr(make_temp(), addr);

    tacbuffer_append(buffer, t);
}
