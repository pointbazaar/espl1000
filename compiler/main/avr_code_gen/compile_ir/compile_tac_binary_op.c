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

static char* c = "TAC_BINARY_OP";

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

static void case_arithmetic_add(int rdest, int rsrc, bool dest_wide, bool src_wide, struct IBuffer* ibu){

	char* c = "TAC_BINARY_OP + ";

	int reg2 = RAT_SCRATCH_REG;

	if(src_wide)
		reg2 = rsrc+1;
	else
		ldi(reg2, 0, c);

	add(rdest, rsrc, c);

	if(dest_wide)
		adc(rdest+1, reg2, c);

}

static void case_arithmetic_sub(int rdest, int rsrc, bool dest_wide, bool src_wide, struct IBuffer* ibu){

	char* c = "TAC_BINARY_OP - ";

	if(dest_wide){
		if(src_wide){
			sub(rdest, rsrc, c);
			sbc(rdest+1, rsrc+1, c);
		}else{
			ldi(RAT_SCRATCH_REG, 0, c);
			sub(rdest, rsrc, c);
			sbc(rdest+1, RAT_SCRATCH_REG, c);
		}
	}else{
		sub(rdest, rsrc, c);
	}

}

static void case_arithmetic(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu){

	//left and right operand should have registers

	int rsrc = rat_get_register(rat, tac->arg1);

	int rdest = rat_get_register(rat, tac->dest);

	bool dest_wide = rat_is_wide(rat, tac->dest);
	bool src_wide  = rat_is_wide(rat, tac->arg1);

	switch (tac->op) {

	case TAC_OP_ADD:
		case_arithmetic_add(rdest, rsrc, dest_wide, src_wide, ibu); break;

	case TAC_OP_SUB:
		case_arithmetic_sub(rdest, rsrc, dest_wide, src_wide, ibu); break;

	case TAC_OP_MUL:
		mul(rdest, rsrc, "");
		//TODO: figure out the case with wide temporaries
		break;

	case TAC_OP_AND:
		and(rdest, rsrc, "");
		if(dest_wide && src_wide)
			and(rdest+1, rsrc+1, "");
		break;

	case TAC_OP_OR:
		or(rdest, rsrc, "");
		if(dest_wide && src_wide)
			or(rdest+1, rsrc+1, "");
		break;

	case TAC_OP_XOR:
		eor(rdest, rsrc, "");
		if(dest_wide && src_wide)
			or(rdest+1, rsrc+1, "");
		break;

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

	cp(rdest, rsrc, c);
	brlt(Ltrue, c);

	clr(rdest, c);
	rjmp(Lend, c);
	label(Ltrue);

	sub(rdest, rsrc,c );
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


	ldi(RAT_SCRATCH_REG, 1, c);
	cp(rdest, rsrc, c);
	brge(Ltrue, c);
	ldi(RAT_SCRATCH_REG, 0, c);

	label(Ltrue);
	mov(rdest, RAT_SCRATCH_REG, c);
}

static void case_cmp_neq(struct IBuffer* ibu, int rdest, int rsrc){

	//we just subtract the 2 registers,
	//if they were equal, rdest will be 0, meaning false
	//otherwise it will be nonzero, meaning true

	sub(rdest, rsrc, c);
}

static void case_cmp_eq(struct IBuffer* ibu, int rdest, int rsrc){


	//we use r16, which is reserved in the RAT as multi-use register.

	//4 instructions
	ldi(RAT_SCRATCH_REG, 1, c);
	cpse(rdest, rsrc, c);
	clr(RAT_SCRATCH_REG, c);
	mov(rdest, RAT_SCRATCH_REG, c);
}
