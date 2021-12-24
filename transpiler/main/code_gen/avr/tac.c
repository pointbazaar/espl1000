#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast/ast_declare.h"
#include "ast/ast.h"

#include "flags.h"
#include "util/ctx.h"

#include "tac.h"

size_t cap   = 10;
size_t count = 0;
struct TAC** tac = NULL;

static uint32_t current_label_index = TAC_NO_LABEL+1;
static uint32_t label_loop_end;
static uint32_t label_loop_start;

struct TAC* makeTAC(){
    struct TAC* res = malloc(sizeof(struct TAC));
    res->label_index = TAC_NO_LABEL;
    res->goto_index = TAC_NO_LABEL;

    strcpy(res->dest, "");
    res->kind = TAC_NONE;
    strcpy(res->arg1, "");
    strcpy(res->arg2, "");

    return res;
}

uint32_t make_label(){
    uint32_t res = current_label_index;
    current_label_index++;
    return res;
}
struct TAC* makeTACLabel(uint32_t label){
    struct TAC* t = makeTAC();
    t->kind = TAC_NOP;
    t->label_index = label;
    return t;
}

struct TAC* makeTACGoto(uint32_t label){
    struct TAC* t = makeTAC();
    t->kind = TAC_GOTO;
    t->goto_index = label;
    return t;
}

void append_tac(struct TAC* node){

    if(count >= cap)
        tac = realloc(tac, sizeof(struct TAC*) * (cap *= 2));

    tac[count++] = node;
}

void tac_call(struct Call* call){

    for(size_t i = 0; i < call->count_args; i++){
        struct Expr* expr = call->args[i];
        tac_expr(expr);
        struct TAC* t = makeTAC();
        t->kind = TAC_PARAM;
        strcpy(t->arg1, tac[count-1]->dest);
        append_tac(t);
    }

    if(call->callable->member_access != NULL){
        printf("member access calls currently unsupported on avr\n");
        exit(1);
    }

    if(call->callable->simple_var->count_indices != 0){
        printf("calls with indices currently unsupported on avr\n");
        exit(1);
    }

    struct TAC* t2 = makeTAC();
    t2->kind = TAC_CALL;
    strcpy(t2->arg1, call->callable->simple_var->name);
    append_tac(t2);
}

void tac_whilestmt(struct WhileStmt* w){
    //TODO

    //L0:
    //t1 = expr
    //if-goto t1 L1
    //goto Lend:
    //L1:
    // block
    // goto L0
    //end:
}

void tac_forstmt(struct ForStmt* f){
    //TODO: we should convert for loops into while loops before 3 address code
    printf("not implemented: ForStmt, %s\n", f->index_name);
    exit(1);
}

void tac_ifstmt(struct IfStmt* s){

    //t1 = expr
    //if-goto t1 L1
    //goto L2:
    //L1:
    // if block
    // goto end
    //L2:
    // else block
    //end:

    uint32_t l1 = make_label();
    uint32_t l2 = make_label();
    uint32_t lend = make_label();

    tac_expr(s->condition);

    struct TAC* t = makeTAC();
    t->kind = TAC_IF_GOTO;
    strcpy(t->arg1, tac[count-1]->dest);
    t->goto_index = l1;
    append_tac(t);

    append_tac(makeTACGoto(l2));

    append_tac(makeTACLabel(l1));

    tac_stmtblock(s->block);

    append_tac(makeTACGoto(lend));

    append_tac(makeTACLabel(l2));

    tac_stmtblock(s->else_block);

    append_tac(makeTACLabel(lend));
}

void tac_retstmt(struct RetStmt* r){

    if(r->return_value != NULL){
        tac_expr(r->return_value);
    }

    struct TAC* t = makeTAC();
    t->kind = TAC_RETURN;
    strcpy(t->arg1, tac[count-1]->dest);

    append_tac(t);
}

void tac_assignstmt(struct AssignStmt* a){

    tac_expr(a->expr);

    if(a->var->member_access != NULL){
        printf("member access assignments currently unsupported on avr\n");
        exit(1);
    }

    if(a->var->simple_var->count_indices != 0){
        printf("assignments with indices currently unsupported on avr\n");
        exit(1);
    }

    struct TAC* t = makeTAC();
    t->kind = TAC_COPY;
    strcpy(t->arg1, a->var->simple_var->name);
    strcpy(t->arg2, tac[count-1]->dest);
}

void tac_stmt(struct Stmt* stmt){

    if(stmt->is_throw){
        printf("throw not implemented for avr\n");
        exit(1);
    }

    if(stmt->is_break){
        struct TAC* t = makeTAC();
        t->kind = TAC_GOTO;
        t->goto_index = label_loop_end;
        append_tac(t);
        return;
    }
    if(stmt->is_continue){
        struct TAC* t = makeTAC();
        t->kind = TAC_GOTO;
        t->goto_index = label_loop_start;
        append_tac(t);
        return;
    }

    switch(stmt->kind){
        case 1: tac_call(stmt->ptr.m1); break;
        case 2: tac_whilestmt(stmt->ptr.m2); break;
        case 3: tac_ifstmt(stmt->ptr.m3); break;
        case 4: tac_retstmt(stmt->ptr.m4); break;
        case 5: tac_assignstmt(stmt->ptr.m5); break;
        case 6:
            //try-catch stmt, we can't do that for avr right now
            printf("try-catch not implemented for avr\n");
            exit(1);
        case 7: tac_forstmt(stmt->ptr.m7); break;
        case 8:
            //switch stmt, we can't do that for avr right now
            printf("switch not implemented for avr\n");
            exit(1);
    }
}

void tac_method(struct Method* m){
    tac_stmtblock(m->block);
}

void tac_stmtblock(struct StmtBlock* block){
    for(size_t k = 0; k < block->count; k++){
        struct Stmt* s = block->stmts[k];
        tac_stmt(s);
    }
}

void print_tac(struct TAC* t){
    char* op = "";
    char* op_str[10] = {"+","-","*","/", "%"};

    if(t->kind <= 4){
        op = op_str[t->kind];
    }

    if(t->label_index != TAC_NO_LABEL)
        printf("L%2d:", t->label_index);
    else
        printf("    ");

    char goto_label_str[5];
    if(t->goto_index != TAC_NO_LABEL){
        sprintf(goto_label_str, "L%d", t->goto_index);
    }

    switch (t->kind) {
        case TAC_GOTO:
            printf("%10s %10s", "goto", goto_label_str);
            break;
        case TAC_IF_GOTO:
            printf("%10s %10s %10s", "if-goto", t->arg1, goto_label_str);
            break;
        default:
            printf("%10s = %10s %4s %10s", t->dest, t->arg1, op, t->arg2);
    }

    printf("\n");
}