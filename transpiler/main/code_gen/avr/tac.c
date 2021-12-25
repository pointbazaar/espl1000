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

static uint32_t temp_count = 0; //temporaries for TAC

static uint32_t current_label_index = TAC_NO_LABEL+1;
static uint32_t label_loop_end;
static uint32_t label_loop_start;

struct TAC* makeTAC(){
    struct TAC* res = malloc(sizeof(struct TAC));
    res->label_index = TAC_NO_LABEL;
    strcpy(res->label_name, "");
    res->goto_index = TAC_NO_LABEL;

    strcpy(res->dest, "");
    res->kind = TAC_NONE;
    strcpy(res->arg1, "");
    strcpy(res->op, "");
    strcpy(res->arg2, "");

    return res;
}

uint32_t make_label(){
    uint32_t res = current_label_index;
    current_label_index++;
    return res;
}
uint32_t make_temp(){
    uint32_t res = temp_count;
    temp_count++;
    return res;
}
struct TAC* makeTACLabel(uint32_t label){
    struct TAC* t = makeTAC();
    t->kind = TAC_NOP;
    t->label_index = label;
    strcpy(t->label_name, "");
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
    sprintf(t2->dest, "t%d", make_temp());
    strcpy(t2->arg1, call->callable->simple_var->name);
    append_tac(t2);
}

void tac_whilestmt(struct WhileStmt* w){

    //L0:
    //t1 = expr
    //if-goto t1 L1
    //goto Lend:
    //L1:
    // block
    // goto L0
    //end:

    uint32_t l0 = make_label();
    uint32_t l1 = make_label();
    uint32_t lend = make_label();

    append_tac(makeTACLabel(l0));

    tac_expr(w->condition);

    struct TAC* t = makeTAC();
    t->kind = TAC_IF_GOTO;
    strcpy(t->arg1, tac[count-1]->dest);
    t->goto_index = l1;
    append_tac(t);

    append_tac(makeTACGoto(lend));

    append_tac(makeTACLabel(l1));

    tac_stmtblock(w->block);

    append_tac(makeTACGoto(l0));

    append_tac(makeTACLabel(lend));
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

void tac_namespace(struct Namespace* ns){
    for(size_t j = 0; j < ns->count_methods; j++){
        struct Method* m = ns->methods[j];
        tac_method(m);
    }
}

void tac_method(struct Method* m){

    struct TAC* t = makeTAC();
    t->kind = TAC_NOP;
    t->label_index = TAC_NO_LABEL;
    strncpy(t->label_name, m->decl->name, DEFAULT_STR_SIZE);
    append_tac(t);

    tac_stmtblock(m->block);
}

void tac_stmtblock(struct StmtBlock* block){
    for(size_t k = 0; k < block->count; k++){
        struct Stmt* s = block->stmts[k];
        tac_stmt(s);
    }
}

void tac_constvalue(struct ConstValue* c){
    struct TAC* t = makeTAC();
    t->kind = TAC_CONST_VALUE;
    sprintf(t->dest, "t%d", make_temp());

    switch (c->kind) {
        case 1:
            t->const_value = c->ptr.m1_bool_const->value;
            break;
        case 2:
            t->const_value = c->ptr.m2_int_const->value;
            break;
        case 3:
            t->const_value = (unsigned char)c->ptr.m3_char_const->value;
            break;
        case 4:
            t->const_value = (int32_t)c->ptr.m4_float_const->value;
            break;
        case 5:
            t->const_value = (int32_t)c->ptr.m5_hex_const->value;
            break;
        case 6:
            t->const_value = (int32_t)c->ptr.m6_bin_const->value;
            break;
    }
    append_tac(t);
}

void tac_expr(struct Expr* expr){

    tac_unopterm(expr->term1);

    if(expr->term2 != NULL){

        //we know dest of term1
        char* t1 = tac[count-1]->dest;

        tac_unopterm(expr->term2);
        char* t2 = tac[count-1]->dest;

        struct TAC* t = makeTAC();
        sprintf(t->dest, "t%d", make_temp());
        strcpy(t->arg1,  t1);
        strcpy(t->arg2,  t2);

        char* opstr = expr->op->op;

        t->kind = TAC_BINARY_OP;
        strcpy(t->op, opstr);
        append_tac(t);
    }
}

void tac_unopterm(struct UnOpTerm* u){

    tac_term(u->term);

    if(u->op != NULL){
        struct TAC* t = makeTAC();
        t->kind = TAC_UNARY_OP;
        sprintf(t->dest,"t%d", make_temp());
        strcpy(t->arg1, tac[count-1]->dest);
        strcpy(t->op, u->op->op);

        append_tac(t);
    }
}

void tac_variable(struct Variable* v){

    tac_simplevar(v->simple_var);

    if(v->member_access != NULL){
        printf("member access not implemented for avr\n");
        exit(1);
    }
}

void tac_simplevar(struct SimpleVar* v){

    if(v->count_indices != 0){
        //TODO
        printf("indices not yet implemented for avr\n");
        exit(1);
    }

    struct TAC* t = makeTAC();
    t->kind = TAC_COPY;
    sprintf(t->dest, "t%d", make_temp());
    strcpy(t->arg1, v->name);

    append_tac(t);
}

void tac_term(struct Term* t){

    switch(t->kind){
        case 4: tac_call(t->ptr.m4); break;
        case 5: tac_expr(t->ptr.m5); break;
        case 6: tac_variable(t->ptr.m6); break;
        case 8 :
            //TODO :implement string const
            //t->ptr.m8
            break;
        case 11:
            //lambdas should not exist anymore at this stage,
            //having been converted into named functions
            //t->ptr.m11
            break;
        case 12: tac_constvalue(t->ptr.m12); break;
        default:
            break;
    }
}

void print_tac(struct TAC* t){

    //print optional label
    if(t->label_index != TAC_NO_LABEL)
        printf("L%3d:%5s", t->label_index, "");
    else if(strcmp(t->label_name, "") == 0)
        printf("%10s", "");
    else
        printf("%-9s:", t->label_name);

    char goto_label_str[5];
    if(t->goto_index != TAC_NO_LABEL){
        sprintf(goto_label_str, "L%d", t->goto_index);
    }

    switch (t->kind) {
        case TAC_GOTO:
            printf("%s %s", "goto", goto_label_str); break;
        case TAC_IF_GOTO:
            printf("%s %s %s", "if-goto", t->arg1, goto_label_str); break;
        case TAC_CONST_VALUE:
            printf("%s = %d",t->dest, t->const_value); break;
        case TAC_COPY:
            printf("%s = %s", t->dest, t->arg1); break;
        case TAC_NOP:
            printf("%s", "nop"); break;
        case TAC_BINARY_OP:
            printf("%s = %s %4s %s", t->dest, t->arg1, t->op, t->arg2); break;
        case TAC_UNARY_OP:
            printf("%s = %4s %s", t->dest, t->op, t->arg1); break;
        case TAC_CALL:
            printf("%s = call %s", t->dest, t->arg1); break;
        case TAC_PARAM:
            printf("param %s", t->arg1); break;
        case TAC_RETURN:
            printf("return %s", t->arg1); break;
        case TAC_DEREF:
            //TODO
            break;

        case TAC_NONE: break;
    }

    printf("\n");
}