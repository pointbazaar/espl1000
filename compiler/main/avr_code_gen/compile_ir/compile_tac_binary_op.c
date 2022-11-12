#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

static void case_arithmetic(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu);
static void case_compare(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu);

static void case_cmp_lt(struct IBuffer* ibu, int rdest, int rsrc);
static void case_cmp_ge(struct IBuffer* ibu, int rdest, int rsrc);
static void case_cmp_neq(struct IBuffer* ibu, int rdest, int rsrc);
static void case_cmp_eq(struct IBuffer* ibu, int rdest, int rsrc);

static int label_counter = 0;

void compile_tac_binary_op(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu){

    switch (tac->op) {

        case TAC_OP_ADD: 
        case TAC_OP_SUB: 
        case TAC_OP_MUL: 
        case TAC_OP_AND: 
        case TAC_OP_OR:  
        case TAC_OP_XOR: 
			case_arithmetic(rat, tac, ibu);
			break;


        case TAC_OP_CMP_LT: 
        case TAC_OP_CMP_GE: 
        case TAC_OP_CMP_EQ: 
        case TAC_OP_CMP_NEQ: 
			case_compare(rat, tac, ibu);
			break;

		default: 
			break; 
    }
}


static void case_arithmetic(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu){
	
	//left and right operand should have registers

    int rsrc = rat_get_register(rat, tac->arg1);

    int rdest = rat_get_register(rat, tac->dest);

    switch (tac->op) {

        case TAC_OP_ADD: add(rdest, rsrc); break;
        case TAC_OP_SUB: sub(rdest, rsrc); break;
        case TAC_OP_MUL: mul(rdest, rsrc); break;
        case TAC_OP_AND: and(rdest, rsrc); break;
        case TAC_OP_OR:  or(rdest, rsrc); break;
        case TAC_OP_XOR: eor(rdest, rsrc); break;
        default: break;
	}
}

static void case_compare(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu){
	
	//left and right operand should have registers

    int rsrc = rat_get_register(rat, tac->arg1);

    int rdest = rat_get_register(rat, tac->dest);
	
	switch(tac->op){
		case TAC_OP_CMP_LT: case_cmp_lt(ibu, rdest, rsrc); break;
        case TAC_OP_CMP_GE: case_cmp_ge(ibu, rdest, rsrc); break;
        case TAC_OP_CMP_EQ: case_cmp_eq(ibu, rdest, rsrc); break;
        case TAC_OP_CMP_NEQ: case_cmp_neq(ibu, rdest, rsrc); break;
        default: break;
	}
}


static void case_cmp_lt(struct IBuffer* ibu, int rdest, int rsrc){
	
	//5 instructions
	
	// cp rdest, rsrc
	// brlt Ltrue
	// clr rdest
	// rjmp Lend
	//Ltrue:
	// sub rdest, rsrc
	//Lend:
	
	char Ltrue[20];
	char Lend[20];

	sprintf(Ltrue, "Ltrue%d", label_counter);
	sprintf(Lend, "Lend%d", label_counter++);
	
	cp(rdest, rsrc);
	brlt(Ltrue);
	
	clr(rdest);
	rjmp(Lend);
	label(Ltrue);
	
	sub(rdest, rsrc);
	label(Lend);
}

static void case_cmp_ge(struct IBuffer* ibu, int rdest, int rsrc){
	
	//5 instructions
	
	// ldi r16, 1
	// cp rdest, rsrc
	// brge Ltrue
	// ldi r16, 0
	//Ltrue:
	// mov rdest, r16
	
	char Ltrue[20];
	sprintf(Ltrue, "Ltrue%d", label_counter++);
	
	
	ldi(RAT_SCRATCH_REG, 1, "");
	cp(rdest, rsrc);
	brge(Ltrue);
	ldi(RAT_SCRATCH_REG, 0, "");
	
	label(Ltrue);
	mov(rdest, RAT_SCRATCH_REG);
}

static void case_cmp_neq(struct IBuffer* ibu, int rdest, int rsrc){
	
	//we just subtract the 2 registers,
	//if they were equal, rdest will be 0, meaning false
	//otherwise it will be nonzero, meaning true
	
	sub(rdest, rsrc);
}

static void case_cmp_eq(struct IBuffer* ibu, int rdest, int rsrc){
	
	//we use r16, which is reserved in the RAT as multi-use register.
	
	//4 instructions
	ldi(RAT_SCRATCH_REG, 1, "");
	cpse(rdest, rsrc);
	clr(RAT_SCRATCH_REG);
	mov(rdest, RAT_SCRATCH_REG);
}
