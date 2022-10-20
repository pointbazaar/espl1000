#include <stdio.h>
#include <stdlib.h>

#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "gen_tac.h"

void tac_switchstmt(struct TACBuffer* buffer, struct SwitchStmt* ss){

    tac_expr(buffer, ss->expr);

    //now the last temporary contains our expr
    //generate an if for each case
    char* tmp_expr = tacbuffer_last_dest(buffer);

    uint32_t label_end = make_label();
    uint32_t* labels_cases = malloc(sizeof(uint32_t)*ss->count_cases);

    for(size_t i=0;i < ss->count_cases; i++){

        struct CaseStmt* cs = ss->cases[i];

        uint32_t label_case = make_label();
        labels_cases[i] = label_case;

        //t2 = cs->const_value
        struct TAC* tc = makeTACConst(make_temp(), int_value_from_const(cs->const_value));
        
        tacbuffer_append(buffer, tc);

        //t2 = t2 == tmp_expr
        struct TAC* tc3 = makeTACBinOp(tc->dest, TAC_OP_CMP_EQ, tmp_expr);
        
        tacbuffer_append(buffer, tc3);

        //if t1 goto LCase???
        struct TAC* tcif = makeTACIfGoto(tacbuffer_last_dest(buffer), label_case);
        tacbuffer_append(buffer, tcif);
    }

    //goto end
    tacbuffer_append(buffer, makeTACGoto(label_end));

    //... code and labels for the cases
    for(size_t i=0;i < ss->count_cases; i++) {
        struct CaseStmt *cs = ss->cases[i];

        //LCase:
        tacbuffer_append(buffer, makeTACLabel(labels_cases[i]));
        //... code for that case
        if(cs->block != NULL)
            tac_stmtblock(buffer, cs->block);

        if(cs->block == NULL) {
            //fallthrough to next case
            uint32_t target_index = label_end;

            if(i < ss->count_cases - 1)
                target_index = labels_cases[i+1];

            tacbuffer_append(buffer, makeTACGoto(target_index));
        }else {
            //goto end
            tacbuffer_append(buffer, makeTACGoto(label_end));
        }
    }

    //end:
    tacbuffer_append(buffer, makeTACLabel(label_end));

    free(labels_cases);
}
