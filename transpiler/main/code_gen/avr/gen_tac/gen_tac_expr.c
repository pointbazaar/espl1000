#include <stdio.h>
#include <stdlib.h>

#include "code_gen/avr/tac/tac.h"
#include "code_gen/avr/tacbuffer/tacbuffer.h"
#include "gen_tac.h"

void tac_expr(struct TACBuffer* buffer, struct Expr* expr){

    tac_unopterm(buffer, expr->term1);

    if(expr->term2 != NULL){

        //we know dest of term1
        char* t1 = buffer->buffer[buffer->count - 1]->dest;

        tac_unopterm(buffer, expr->term2);
        char* t2 = buffer->buffer[buffer->count - 1]->dest;

        struct TAC* t = makeTAC();
        sprintf(t->dest, "%s", t1);
        strcpy(t->arg1,  t2);

        char* opstr = expr->op->op;

        t->kind = TAC_BINARY_OP;
        t->op = TAC_OP_NONE;

        if(strcmp(opstr, "+") == 0) t->op = TAC_OP_ADD;
        if(strcmp(opstr, "-") == 0) t->op = TAC_OP_SUB;
        if(strcmp(opstr, "*") == 0) t->op = TAC_OP_MUL;
        if(strcmp(opstr, "/") == 0) t->op = TAC_OP_DIV;

        if(strcmp(opstr, "<") == 0) t->op = TAC_OP_CMP_LT;
        if(strcmp(opstr, "<=") == 0) t->op = TAC_OP_CMP_LE;
        if(strcmp(opstr, ">") == 0) t->op = TAC_OP_CMP_GT;
        if(strcmp(opstr, ">=") == 0) t->op = TAC_OP_CMP_GE;
        if(strcmp(opstr, "==") == 0) t->op = TAC_OP_CMP_EQ;
        if(strcmp(opstr, "!=") == 0) t->op = TAC_OP_CMP_NEQ;

        if(t->op == TAC_OP_NONE){
            printf("error, opstr was none of supported TAC_OP_... values\n");
            printf("opstr = %s\n", opstr);
            exit(1);
        }

        tacbuffer_append(buffer, t, true);
    }
}