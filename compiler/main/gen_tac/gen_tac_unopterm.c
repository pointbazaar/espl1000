#include <stdio.h>
#include <stdlib.h>

#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "gen_tac.h"

static void tac_unopterm_default(struct TACBuffer* buffer, struct UnOpTerm* u);
static void tac_unopterm_const(struct TACBuffer* buffer, struct UnOpTerm* u);

void tac_unopterm(struct TACBuffer* buffer, struct UnOpTerm* u){

    if(u->term->kind == 12 && u->op != OP_NONE){
		//const value
		tac_unopterm_const(buffer, u);
	}else{
		tac_unopterm_default(buffer, u);
	}
}


static void tac_unopterm_default(struct TACBuffer* buffer, struct UnOpTerm* u){

	tac_term(buffer, u->term);

    if(u->op != OP_NONE){
        
        const uint32_t dest = make_temp();

        enum TAC_OP op = TAC_OP_NONE;
        
        switch(u->op){
			case OP_MINUS: 		op = TAC_OP_UNARY_MINUS; 		break;
			case OP_NOT: 		op = TAC_OP_UNARY_NOT; 			break;
			case OP_COMPLEMENT: op = TAC_OP_UNARY_BITWISE_NEG; 	break;
			
			default:
				printf("error, op was none of supported TAC_OP_... values\n");
				printf("op is %d\n", u->op);
				exit(1);
		}
        
        struct TAC* t = makeTACUnaryOp(dest, tacbuffer_last_dest(buffer), op);

        tacbuffer_append(buffer, t);
    }
}

static void tac_unopterm_const(struct TACBuffer* buffer, struct UnOpTerm* u){
	
	struct ConstValue* c = (struct ConstValue*)u->term->ptr.m12;
	
	tac_constvalue(buffer, c);
	
	struct TAC* t = tacbuffer_get_last(buffer);
	
	switch(u->op){
		case OP_MINUS: 		t->const_value = -t->const_value; 	break;
		case OP_NOT: 		t->const_value = !t->const_value;	break;
		case OP_COMPLEMENT: t->const_value = ~t->const_value; 	break;
		
		default:
			printf("error, op was none of supported TAC_OP_... values\n");
			printf("op is %d\n", u->op);
			exit(1);
	}
}
