#include <stdio.h>
#include <stdlib.h>

#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "gen_tac.h"

static void tac_expr_part_2_constvalue(struct TACBuffer* buffer, struct Expr* expr, char* t1);
static void tac_expr_part_2_no_constvalue(struct TACBuffer* buffer, struct Expr* expr, char* t1);
static enum TAC_OP op_str_to_tac_op(char* op_str, bool* reverse_operands);
static bool operator_immediate_applicable(enum TAC_OP op, int32_t immediate);

void tac_expr(struct TACBuffer* buffer, struct Expr* expr){

    tac_unopterm(buffer, expr->term1);

    if(expr->term2 != NULL) {

        //we know dest of term1
        char *t1 = buffer->buffer[buffer->count - 1]->dest;

		bool reverse_operands = false;
        enum TAC_OP op = op_str_to_tac_op(expr->op->op, &reverse_operands);

        bool is_const_value = expr->term2->op == NULL
                              && expr->term2->term->kind == 12;

        bool applicable = false;

        if (is_const_value) {

            const int32_t immediate = int_value_from_const(expr->term2->term->ptr.m12);
            applicable = operator_immediate_applicable(op, immediate);
        }

        if(applicable){
            //constvalue and applicable operator
            tac_expr_part_2_constvalue(buffer, expr, t1);
        }else{
            tac_expr_part_2_no_constvalue(buffer, expr, t1);
        }
    }
}

static bool operator_immediate_applicable(enum TAC_OP op, int32_t immediate){
    //is the given operator applicable for immediate
    //operations, such as ANDI, SUBI, ORI, ...
    switch (op) {
        case TAC_OP_ADD:
            //in that case, we can use INC
            return immediate == 1;
        case TAC_OP_SUB:
        case TAC_OP_AND:
        case TAC_OP_OR:
            return true;
        default:
            return false;
    }
}

static enum TAC_OP op_str_to_tac_op(char* opstr, bool* reverse_operands){
    enum TAC_OP op = op = TAC_OP_NONE;
    *reverse_operands = false;

    if(strcmp(opstr, "+") == 0) op = TAC_OP_ADD;
    if(strcmp(opstr, "-") == 0) op = TAC_OP_SUB;
    if(strcmp(opstr, "*") == 0) op = TAC_OP_MUL;

    if(strcmp(opstr, "<") == 0) op = TAC_OP_CMP_LT;
    if(strcmp(opstr, "<=") == 0){
		op = TAC_OP_CMP_GE;
		*reverse_operands = true;
	}
    
    if(strcmp(opstr, ">") == 0){
		op = TAC_OP_CMP_GE;
		*reverse_operands = true;
	}
    
    if(strcmp(opstr, ">=") == 0) op = TAC_OP_CMP_GE;
    if(strcmp(opstr, "==") == 0) op = TAC_OP_CMP_EQ;
    if(strcmp(opstr, "!=") == 0) op = TAC_OP_CMP_NEQ;

    if(op == TAC_OP_NONE){
        printf("error, opstr was none of supported TAC_OP_... values\n");
        printf("opstr = %s\n", opstr);
    }
    
    return op;
}

static void tac_expr_part_2_constvalue(struct TACBuffer* buffer, struct Expr* expr, char* t1){

    const int32_t immediate = int_value_from_const(expr->term2->term->ptr.m12);

	bool reverse_operands = false;
    const enum TAC_OP op = op_str_to_tac_op(expr->op->op, &reverse_operands);

    tacbuffer_append(buffer,makeTACBinOpImmediate(t1, op, immediate),true);
}

static void tac_expr_part_2_no_constvalue(struct TACBuffer* buffer, struct Expr* expr, char* t1){

    tac_unopterm(buffer, expr->term2);
    char* t2 = buffer->buffer[buffer->count - 1]->dest;

    struct TAC* t = makeTAC();
    
    t->kind = TAC_BINARY_OP;
    
    bool reverse_operands = false;
    t->op = op_str_to_tac_op(expr->op->op, &reverse_operands);

	if(reverse_operands){
		
		strcpy(t->dest, t2);
		sprintf(t->arg1, "%s", t1);
	}else{
		
		sprintf(t->dest, "%s", t1);
		strcpy(t->arg1,  t2);
	}

    tacbuffer_append(buffer, t, true);
}
