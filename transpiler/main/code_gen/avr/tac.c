#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast/ast_declare.h"
#include "ast/ast.h"

#include "flags.h"
#include "util/ctx.h"

#include "tacbuffer.h"
#include "tac.h"

static uint32_t temp_count = 0; //temporaries for TAC

static uint32_t current_label_index = TAC_NO_LABEL+1;
static uint32_t label_loop_end;
static uint32_t label_loop_start;

struct TAC* makeTAC(){
    struct TAC* res = malloc(sizeof(struct TAC));
    *res = (struct TAC) {
            .index = 0,
            .label_index = TAC_NO_LABEL,
            .goto_index = TAC_NO_LABEL,
            .kind = TAC_NOP,
            .const_value = 0,
            .const_string = NULL,
    };

    strcpy(res->label_name, "");

    strcpy(res->dest, "");
    strcpy(res->arg1, "");
    res->op = TAC_OP_NONE;

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


void tac_call(struct TACBuffer* buffer, struct Call* call){

    for(size_t i = 0; i < call->count_args; i++){
        struct Expr* expr = call->args[i];
        tac_expr(buffer, expr);

        struct TAC* t = makeTAC();
        t->kind = TAC_PARAM;
        strcpy(t->arg1, buffer->buffer[buffer->count-1]->dest);
        tacbuffer_append(buffer, t, true);
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
    tacbuffer_append(buffer, t2, true);
}

void tac_whilestmt(struct TACBuffer* buffer, struct WhileStmt* w){

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

    tacbuffer_append(buffer, makeTACLabel(l0), true);

    tac_expr(buffer, w->condition);

    struct TAC* t = makeTAC();
    t->kind = TAC_IF_GOTO;
    strcpy(t->arg1, buffer->buffer[buffer->count-1]->dest);
    t->goto_index = l1;
    tacbuffer_append(buffer, t, true);

    tacbuffer_append(buffer, makeTACGoto(lend), true);

    tacbuffer_append(buffer, makeTACLabel(l1), true);

    tac_stmtblock(buffer, w->block);

    tacbuffer_append(buffer, makeTACGoto(l0), true);

    tacbuffer_append(buffer, makeTACLabel(lend), true);
}

void tac_forstmt(struct TACBuffer* buffer, struct ForStmt* f){
    //TODO: we should convert for loops into while loops before 3 address code
    printf("not implemented: ForStmt, %s\n", f->index_name);
    exit(1+buffer->count);
}

void tac_ifstmt(struct TACBuffer* buffer, struct IfStmt* s){

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

    tac_expr(buffer, s->condition);

    struct TAC* t = makeTAC();
    t->kind = TAC_IF_GOTO;
    strcpy(t->arg1, buffer->buffer[buffer->count-1]->dest);
    t->goto_index = l1;
    tacbuffer_append(buffer, t, true);

    tacbuffer_append(buffer, makeTACGoto(l2), true);

    tacbuffer_append(buffer, makeTACLabel(l1), true);

    tac_stmtblock(buffer, s->block);

    tacbuffer_append(buffer, makeTACGoto(lend), true);

    tacbuffer_append(buffer, makeTACLabel(l2), true);

    if(s->else_block != NULL)
        tac_stmtblock(buffer, s->else_block);

    tacbuffer_append(buffer, makeTACLabel(lend), true);
}

void tac_retstmt(struct TACBuffer* buffer, struct RetStmt* r){

    if(r->return_value != NULL){
        tac_expr(buffer, r->return_value);
    }

    struct TAC* t = makeTAC();
    t->kind = TAC_RETURN;
    strcpy(t->arg1, buffer->buffer[buffer->count-1]->dest);

    tacbuffer_append(buffer, t, true);
}

void tac_assignstmt(struct TACBuffer* buffer, struct AssignStmt* a){

    tac_expr(buffer, a->expr);

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
    strcpy(t->dest, a->var->simple_var->name);
    strcpy(t->arg1, buffer->buffer[buffer->count - 1]->dest);

    tacbuffer_append(buffer, t, true);
}

void tac_stmt(struct TACBuffer* buffer, struct Stmt* stmt){

    if(stmt->is_break){
        struct TAC* t = makeTAC();
        t->kind = TAC_GOTO;
        t->goto_index = label_loop_end;
        tacbuffer_append(buffer, t, true);
        return;
    }
    if(stmt->is_continue){
        struct TAC* t = makeTAC();
        t->kind = TAC_GOTO;
        t->goto_index = label_loop_start;
        tacbuffer_append(buffer, t, true);
        return;
    }

    switch(stmt->kind){
        case 1: tac_call(buffer, stmt->ptr.m1); break;
        case 2: tac_whilestmt(buffer, stmt->ptr.m2); break;
        case 3: tac_ifstmt(buffer, stmt->ptr.m3); break;
        case 4: tac_retstmt(buffer, stmt->ptr.m4); break;
        case 5: tac_assignstmt(buffer, stmt->ptr.m5); break;
        case 7: tac_forstmt(buffer, stmt->ptr.m7); break;
        case 8:
            //switch stmt, we can't do that for avr right now
            printf("switch not implemented for avr\n");
            exit(1);
    }
}

void tac_method(struct TACBuffer* buffer, struct Method* m){

    struct TAC* t = makeTAC();
    t->kind = TAC_NOP;
    t->label_index = TAC_NO_LABEL;
    strncpy(t->label_name, m->decl->name, DEFAULT_STR_SIZE);
    tacbuffer_append(buffer, t, true);

    tac_stmtblock(buffer, m->block);
}

void tac_stmtblock(struct TACBuffer* buffer, struct StmtBlock* block){
    for(size_t k = 0; k < block->count; k++){
        struct Stmt* s = block->stmts[k];
        tac_stmt(buffer, s);
    }
}

void tac_constvalue(struct TACBuffer* buffer, struct ConstValue* c){
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
    tacbuffer_append(buffer, t, true);
}

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

void tac_unopterm(struct TACBuffer* buffer, struct UnOpTerm* u){

    tac_term(buffer, u->term);

    if(u->op != NULL){
        struct TAC* t = makeTAC();
        t->kind = TAC_UNARY_OP;
        sprintf(t->dest,"t%d", make_temp());
        strcpy(t->arg1, buffer->buffer[buffer->count - 1]->dest);

        t->op = TAC_OP_NONE;
        char* opstr = u->op->op;

        if(strcmp(opstr, "+") == 0) t->op = TAC_OP_ADD;
        if(strcmp(opstr, "-") == 0) t->op = TAC_OP_ADD;
        if(strcmp(opstr, "*") == 0) t->op = TAC_OP_ADD;
        if(strcmp(opstr, "/") == 0) t->op = TAC_OP_ADD;

        if(t->op == TAC_OP_NONE){
            printf("error, t->op was none of supported TAC_OP_... values\n");
            exit(1);
        }

        tacbuffer_append(buffer, t, true);
    }
}

void tac_variable(struct TACBuffer* buffer, struct Variable* v){

    tac_simplevar(buffer, v->simple_var);

    if(v->member_access != NULL){
        printf("member access not implemented for avr\n");
        exit(1);
    }
}

void tac_simplevar(struct TACBuffer* buffer, struct SimpleVar* v){

    if(v->count_indices != 0){
        //TODO
        printf("indices not yet implemented for avr\n");
        exit(1);
    }

    struct TAC* t = makeTAC();
    t->kind = TAC_COPY;
    sprintf(t->dest, "t%d", make_temp());
    strcpy(t->arg1, v->name);

    tacbuffer_append(buffer, t, true);
}

void tac_stringconst(struct TACBuffer* buffer, struct StringConst* s){

    struct TAC* t = makeTAC();
    t->kind = TAC_CONST_STRING;
    t->const_string = s->value;

    tacbuffer_append(buffer, t, true);
}

void tac_term(struct TACBuffer* buffer, struct Term* t){

    switch(t->kind){
        case 4: tac_call(buffer, t->ptr.m4); break;
        case 5: tac_expr(buffer, t->ptr.m5); break;
        case 6: tac_variable(buffer, t->ptr.m6); break;
        case 8: tac_stringconst(buffer, t->ptr.m8); break;
        case 11:
            printf("Fatal Error. Lambdas should not exist at this stage.\n");
            exit(1);
            //lambdas should not exist anymore at this stage,
            //having been converted into named functions
        case 12: tac_constvalue(buffer, t->ptr.m12); break;
        default:
            break;
    }
}

bool tac_is_unconditional_jump(struct TAC* tac){
    return tac->kind == TAC_GOTO || tac->kind == TAC_RETURN;
}
