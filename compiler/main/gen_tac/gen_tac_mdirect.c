
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

    if(expr->term1->term->ptr.m12->kind != 2){
        printf("non-int-address memory load currently unsupported. exiting\n");
        exit(1);
    }

    const uint32_t addr = expr->term1->term->ptr.m12->ptr.m2_int_const->value;

    char dest[10];
    sprintf(dest, "t%d", make_temp());

    struct TAC* t = makeTACLoadConstAddr(dest, addr);

    tacbuffer_append(buffer, t, true);
}