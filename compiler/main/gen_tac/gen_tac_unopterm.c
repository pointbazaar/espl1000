#include <stdio.h>
#include <stdlib.h>

#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "gen_tac.h"

void tac_unopterm(struct TACBuffer* buffer, struct UnOpTerm* u){

    tac_term(buffer, u->term);

    if(u->op != OP_NONE){
        
        char dest_str[10];
        sprintf(dest_str, "t%d", make_temp());

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
        
        struct TAC* t = makeTACUnaryOp(dest_str, tacbuffer_last_dest(buffer), op);

        tacbuffer_append(buffer, t);
    }
}
