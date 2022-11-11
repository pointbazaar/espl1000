#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "tables/symtable/symtable.h"
#include "tables/sst/sst.h"
#include "tac.h"

static char* get_op_str(enum TAC_OP top){
    char* opstr = "";
    switch (top) {
        default:
        case TAC_OP_NONE: opstr = "?ERR in get_op_str"; break;

        case TAC_OP_ADD: opstr = "+="; break;
        case TAC_OP_SUB: opstr = "-="; break;
        case TAC_OP_MUL: opstr = "*="; break;
        case TAC_OP_AND: opstr = "&="; break;
        case TAC_OP_OR:  opstr = "|="; break;
        case TAC_OP_XOR: opstr = "^="; break;
        
        case TAC_OP_SHIFT_LEFT:  opstr = "<<="; break;
        case TAC_OP_SHIFT_RIGHT: opstr = ">>="; break;


        case TAC_OP_CMP_LT: opstr = "<"; break;
        case TAC_OP_CMP_GE: opstr = ">="; break;
        case TAC_OP_CMP_EQ: opstr = "=="; break;
        case TAC_OP_CMP_NEQ: opstr = "!="; break;

        case TAC_OP_UNARY_NOT: opstr = "!"; break;
        case TAC_OP_UNARY_BITWISE_NEG: opstr = "~"; break;
        case TAC_OP_UNARY_MINUS: opstr = "-"; break;
    }
    return opstr;
}

char* tac_tostring(struct TAC* t, struct Ctx* ctx){

    char* res = exit_malloc(sizeof(char)*120);
    strcpy(res, "");

    char buffer[110];
    strcpy(buffer, "");

    char* opstr = get_op_str(t->op);

    switch (t->kind) {
        case TAC_LABEL_INDEXED:
            sprintf(res,"L%3d:%5s", t->label_index, ""); break;
		case TAC_LABEL_FUNCTION:
			{
				char* function_name = "main"; //in case sst is not initialized
				
				if(t->arg1 < sst_size(ctx_tables(ctx)->sst)){
					function_name = sst_at(ctx_tables(ctx)->sst, t->dest)->name;
				}
				sprintf(res, "%-9s:", function_name);
			}
            break;
            
        case TAC_GOTO:
            sprintf(buffer, "goto L%d", t->label_index); break;
        case TAC_IF_GOTO:
            sprintf(buffer, "if-goto t%d L%d", t->arg1, t->label_index); break;
		case TAC_IF_CMP_GOTO:
			sprintf(buffer, "if t%d %s t%d goto L%d", t->dest, opstr, t->arg1, t->label_index); break;
            
        case TAC_CONST_VALUE:
            sprintf(buffer,"t%d = %d",t->dest, t->const_value); break;
            
        case TAC_COPY:
            sprintf(buffer,"t%d = t%d", t->dest, t->arg1); break;
		case TAC_LOAD_LOCAL:
			//TODO: local name
            sprintf(buffer,"load t%d = l%d", t->dest, t->arg1); break;
		case TAC_STORE_LOCAL:
			//TODO: local name
            sprintf(buffer,"store l%d = t%d", t->dest, t->arg1); break;
            
        case TAC_LOAD_CONST_ADDR:
            sprintf(buffer,"t%d = [%d]", t->dest, t->const_value); break;
        case TAC_STORE_CONST_ADDR:
            sprintf(buffer,"[%d] = t%d", t->const_value, t->arg1); break;
        case TAC_LOAD:
			sprintf(buffer,"t%d = [t%d]", t->dest, t->arg1); break;
		case TAC_STORE:
			sprintf(buffer,"[t%d] = t%d", t->dest, t->arg1); break;
        
        case TAC_NOP:
            sprintf(buffer,"%s", "nop"); break;
        case TAC_BINARY_OP:
            if(t->op >= TAC_OP_CMP_LT &&  t->op <= TAC_OP_CMP_NEQ){
                sprintf(buffer, "t%d = t%d %4s t%d", t->dest, t->dest, opstr, t->arg1);
            }else {
                sprintf(buffer, "t%d %4s t%d", t->dest, opstr, t->arg1);
            }
            break;
        case TAC_UNARY_OP:
            sprintf(buffer,"t%d = %4s t%d", t->dest, opstr, t->arg1); break;
            
        case TAC_CALL:
			{
				char* function_name = "main"; //in case sst is not initialized
				
				if(t->arg1 < sst_size(ctx_tables(ctx)->sst)){
					function_name = sst_at(ctx_tables(ctx)->sst, t->arg1)->name;
				}
				sprintf(buffer,"t%d = call %s", t->dest, function_name); 
			}
            break;
            
        case TAC_PARAM:
            sprintf(buffer,"param t%d", t->arg1); break;
        case TAC_RETURN:
            sprintf(buffer,"return t%d", t->dest); break;
        case TAC_BINARY_OP_IMMEDIATE:
            sprintf(buffer, "t%d %4s %d", t->dest, opstr, t->const_value); break;
            
		case TAC_SETUP_STACKFRAME:
			sprintf(buffer, "setup_stackframe %d", t->const_value); break;
		case TAC_SETUP_SP:
			sprintf(buffer, "setup SP"); break;
    }

    strcat(res, buffer);
    return res;
}
