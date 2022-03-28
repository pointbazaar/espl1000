#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "ast/ast_declare.h"

#include "code_gen/avr/tacbuffer/tacbuffer.h"
#include "tac.h"

static uint32_t temp_count = 0; //temporaries for TAC

static uint32_t current_label_index = TAC_NO_LABEL+1;


struct TAC* makeTAC(){
    struct TAC* res = malloc(sizeof(struct TAC));
    *res = (struct TAC) {
            .index = 0,
            .label_index = TAC_NO_LABEL,
            .kind = TAC_NOP,
            .const_value = 0,
    };

    strcpy(res->label_name, "");

    strcpy(res->dest, "");
    strcpy(res->arg1, "");
    res->op = TAC_OP_NONE;

    return res;
}

uint32_t make_label(){
    uint32_t res = current_label_index;
    current_label_index++;
    return res;
}
uint32_t make_temp(){
    uint32_t res = temp_count;
    temp_count++;
    return res;
}
struct TAC* makeTACLabel(uint32_t label){
    struct TAC* t = makeTAC();
    t->kind = TAC_LABEL;
    t->label_index = label;
    strcpy(t->label_name, "");
    return t;
}

struct TAC* makeTACLabel2(char* label){
    struct TAC* t = makeTAC();
    t->kind = TAC_LABEL;
    t->label_index = TAC_NO_LABEL;
    strncpy(t->label_name, label, DEFAULT_STR_SIZE);
    return t;
}

struct TAC* makeTACGoto(uint32_t label){
    struct TAC* t = makeTAC();
    t->kind = TAC_GOTO;
    t->label_index = label;
    return t;
}

struct TAC* makeTACIfGoto(char* tmp_condition, uint32_t label_destination){
    struct TAC* t = makeTAC();
    t->kind = TAC_IF_GOTO;
    sprintf(t->arg1, "%s", tmp_condition);
    t->label_index = label_destination;
    return t;
}

bool tac_is_unconditional_jump(struct TAC* tac){
    return tac->kind == TAC_GOTO || tac->kind == TAC_RETURN;
}
