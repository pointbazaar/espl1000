#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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

char* tac_tostring(struct TAC* t){

    char* res = malloc(sizeof(char)*120);
    strcpy(res, "");

    char buffer[110];
    strcpy(buffer, "");

    char* opstr = get_op_str(t->op);

    switch (t->kind) {
        case TAC_LABEL:
            if (strcmp(t->dest, "") == 0)
                sprintf(res,"L%3d:%5s", t->label_index, "");
            else
                sprintf(res, "%-9s:", t->dest);
            break;
        case TAC_GOTO:
            sprintf(buffer, "goto L%d", t->label_index); break;
        case TAC_IF_GOTO:
            sprintf(buffer, "if-goto %s L%d", t->arg1, t->label_index); break;
        case TAC_CONST_VALUE:
            sprintf(buffer,"%s = %d",t->dest, t->const_value); break;
            
        case TAC_COPY:
            sprintf(buffer,"%s = %s", t->dest, t->arg1); break;
		case TAC_LOAD_LOCAL:
            sprintf(buffer,"load %s = %s", t->dest, t->arg1); break;
		case TAC_STORE_LOCAL:
            sprintf(buffer,"store %s = %s", t->dest, t->arg1); break;
            
        case TAC_LOAD_CONST_ADDR:
            sprintf(buffer,"%s = [%d]", t->dest, t->const_value); break;
        case TAC_STORE_CONST_ADDR:
            sprintf(buffer,"[%d] = %s", t->const_value, t->arg1); break;
        case TAC_NOP:
            sprintf(buffer,"%s", "nop"); break;
        case TAC_BINARY_OP:
            if(t->op >= TAC_OP_CMP_LT &&  t->op <= TAC_OP_CMP_NEQ){
                sprintf(buffer, "%s = %s %4s %s", t->dest, t->dest, opstr, t->arg1);
            }else {
                sprintf(buffer, "%s %4s %s", t->dest, opstr, t->arg1);
            }
            break;
        case TAC_UNARY_OP:
            sprintf(buffer,"%s = %4s %s", t->dest, opstr, t->arg1); break;
        case TAC_CALL:
            sprintf(buffer,"%s = call %s", t->dest, t->arg1); break;
        case TAC_PARAM:
            sprintf(buffer,"param %s", t->arg1); break;
        case TAC_RETURN:
            sprintf(buffer,"return %s", t->dest); break;
        case TAC_BINARY_OP_IMMEDIATE:
            sprintf(buffer, "%s %4s %d", t->dest, opstr, t->const_value); break;
		case TAC_SETUP_STACKFRAME:
			sprintf(buffer, "setup_stackframe %d", t->const_value); break;
    }

    strcat(res, buffer);
    return res;
}
