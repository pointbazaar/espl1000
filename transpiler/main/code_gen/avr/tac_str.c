#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "tac.h"

char* tac_tostring(struct TAC* t){

    char* res = malloc(sizeof(char)*100);
    strcpy(res, "");

    //print optional label
    if(t->label_index != TAC_NO_LABEL)
        sprintf(res,"L%3d:%5s", t->label_index, "");
    else if(strcmp(t->label_name, "") == 0)
        sprintf(res,"%10s", "");
    else
        sprintf(res,"%-9s:", t->label_name);

    char goto_label_str[5];
    if(t->goto_index != TAC_NO_LABEL){
        sprintf(goto_label_str, "L%d", t->goto_index);
    }

    char buffer[100];
    strcpy(buffer, "");

    char* opstr = "";
    switch (t->op) {
        case TAC_OP_NONE: opstr = "?ERR"; break;

        case TAC_OP_ADD: opstr = "+="; break;
        case TAC_OP_SUB: opstr = "-="; break;
        case TAC_OP_MUL: opstr = "*="; break;
        case TAC_OP_DIV: opstr = "/="; break;
        case TAC_OP_AND: opstr = "&="; break;
        case TAC_OP_OR:  opstr = "|="; break;


        case TAC_OP_CMP_LT: opstr = "<"; break;
        case TAC_OP_CMP_LE: opstr = "<="; break;
        case TAC_OP_CMP_GT: opstr = ">"; break;
        case TAC_OP_CMP_GE: opstr = ">="; break;
        case TAC_OP_CMP_EQ: opstr = "=="; break;
        case TAC_OP_CMP_NEQ: opstr = "!="; break;
    }

    switch (t->kind) {
        case TAC_GOTO:
            sprintf(buffer, "%s %s", "goto", goto_label_str); break;
        case TAC_IF_GOTO:
            sprintf(buffer, "%s %s %s", "if-goto", t->arg1, goto_label_str); break;

        case TAC_CONST_VALUE:
            sprintf(buffer,"%s = %d",t->dest, t->const_value); break;
        case TAC_COPY:
            sprintf(buffer,"%s = %s", t->dest, t->arg1); break;
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
            sprintf(buffer,"return %s", t->arg1); break;
        case TAC_DEREF:
            //TODO
            break;
    }

    strcat(res, buffer);
    return res;
}