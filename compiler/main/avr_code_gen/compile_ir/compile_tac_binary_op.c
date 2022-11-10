#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

static void case_arithmetic(struct RAT* rat, struct TAC* tac, FILE* fout);
static void case_compare(struct RAT* rat, struct TAC* tac, FILE* fout);

static void case_cmp_lt(FILE* fout, int rdest, int rsrc);
static void case_cmp_ge(FILE* fout, int rdest, int rsrc);
static void case_cmp_neq(FILE* fout, int rdest, int rsrc);
static void case_cmp_eq(FILE* fout, int rdest, int rsrc);

static int label_counter = 0;

void compile_tac_binary_op(struct RAT* rat, struct TAC* tac, FILE* fout){

    switch (tac->op) {

        case TAC_OP_ADD: 
        case TAC_OP_SUB: 
        case TAC_OP_MUL: 
        case TAC_OP_AND: 
        case TAC_OP_OR:  
        case TAC_OP_XOR: 
			case_arithmetic(rat, tac, fout);
			break;


        case TAC_OP_CMP_LT: 
        case TAC_OP_CMP_GE: 
        case TAC_OP_CMP_EQ: 
        case TAC_OP_CMP_NEQ: 
			case_compare(rat,tac,fout);
			break;

		default: 
			//should not happen
			printf("fatal error in compile_tac_binary_op. Exiting.");
			fflush(stdout);
			exit(1);
			break; 
    }
}


static void case_arithmetic(struct RAT* rat, struct TAC* tac, FILE* fout){
	
	//left and right operand should have registers

    int reg_src = rat_get_register(rat, tac->arg1);

    int reg_dest = rat_get_register(rat, tac->dest);
	
	char* mnem = "?";

    switch (tac->op) {

        case TAC_OP_ADD: mnem = "add"; break;
        case TAC_OP_SUB: mnem = "sub"; break;
        case TAC_OP_MUL: mnem = "mul"; break;
        case TAC_OP_AND: mnem = "and"; break;
        case TAC_OP_OR:  mnem = "or";  break;
        case TAC_OP_XOR: mnem = "eor"; break;
        default: break;
	}
	
	fprintf(fout, "%s r%d, r%d\n", mnem, reg_dest, reg_src);
}

static void case_compare(struct RAT* rat, struct TAC* tac, FILE* fout){
	
	//left and right operand should have registers

    int rsrc = rat_get_register(rat, tac->arg1);

    int rdest = rat_get_register(rat, tac->dest);
	
	switch(tac->op){
		case TAC_OP_CMP_LT: case_cmp_lt(fout, rdest, rsrc); break;
        case TAC_OP_CMP_GE: case_cmp_ge(fout, rdest, rsrc); break;
        case TAC_OP_CMP_EQ: case_cmp_eq(fout, rdest, rsrc); break;
        case TAC_OP_CMP_NEQ: case_cmp_neq(fout, rdest, rsrc); break;
        default: break;
	}
}


static void case_cmp_lt(FILE* fout, int rdest, int rsrc){
	
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

	fprintf(fout, "cp r%d, r%d\n", rdest, rsrc);
	fprintf(fout, "brlt %s\n", Ltrue);

	fprintf(fout, "clr r%d\n", rdest);

	fprintf(fout, "rjmp %s\n", Lend);
	fprintf(fout, "%s:\n", Ltrue);

	fprintf(fout, "sub r%d, r%d\n", rdest, rsrc);

	fprintf(fout, "%s:\n", Lend);
}

static void case_cmp_ge(FILE* fout, int rdest, int rsrc){
	
	//5 instructions
	
	// ldi r16, 1
	// cp rdest, rsrc
	// brge Ltrue
	// ldi r16, 0
	//Ltrue:
	// mov rdest, r16
	
	char Ltrue[20];
	sprintf(Ltrue, "Ltrue%d", label_counter++);
	
	fprintf(fout, "ldi r%d, 1\n", RAT_SCRATCH_REG);
	fprintf(fout, "cp r%d, r%d\n", rdest, rsrc);
	fprintf(fout, "brge %s\n", Ltrue);
	fprintf(fout, "ldi r%d, 0\n", RAT_SCRATCH_REG);
	fprintf(fout, "%s:\n", Ltrue);
	fprintf(fout, "mov r%d, r%d\n", rdest, RAT_SCRATCH_REG);
}

static void case_cmp_neq(FILE* fout, int rdest, int rsrc){
	
	//we just subtract the 2 registers,
	//if they were equal, rdest will be 0, meaning false
	//otherwise it will be nonzero, meaning true
	
	fprintf(fout, "sub r%d, r%d\n", rdest, rsrc);
}

static void case_cmp_eq(FILE* fout, int rdest, int rsrc){
	
	//we use r16, which is reserved in the RAT as multi-use register.
	
	//4 instructions
	
	fprintf(fout, "ldi r%d, 1\n", RAT_SCRATCH_REG);
	fprintf(fout, "cpse r%d, r%d\n", rdest, rsrc);
	fprintf(fout, "clr r%d\n", RAT_SCRATCH_REG);
	fprintf(fout, "mov r%d, r%d\n", rdest, RAT_SCRATCH_REG);
}
