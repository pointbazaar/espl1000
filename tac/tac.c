#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "ast/ast_declare.h"

#include "tacbuffer.h"
#include "tac.h"

static uint32_t temp_count = 0; //temporaries for TAC

static uint32_t current_label_index = TAC_NO_LABEL+1;

static struct TAC* makeTAC();


struct TAC* makeTAC(){
    struct TAC* res = malloc(sizeof(struct TAC));
    *res = (struct TAC) {
            .index = 0,
            .label_index = TAC_NO_LABEL,
            .kind = TAC_NOP,
            .const_value = 0,
    };

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
    strcpy(t->dest, "");
    return t;
}

struct TAC* makeTACLabel2(char* label){
    struct TAC* t = makeTAC();
    t->kind = TAC_LABEL;
    t->label_index = TAC_NO_LABEL;
    strncpy(t->dest, label, DEFAULT_STR_SIZE);
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

struct TAC* makeTACCopy(char* dest, char* src){
    struct TAC* t = makeTAC();
    t->kind = TAC_COPY;
    sprintf(t->dest, "%s", dest);
    sprintf(t->arg1, "%s", src);
    return t;
}

struct TAC* makeTACConst(uint32_t tmp, int value){
    struct TAC* t = makeTAC();
    t->kind = TAC_CONST_VALUE;
    sprintf(t->dest, "t%d", tmp);
    t->const_value = value;
    return t;
}

struct TAC* makeTACBinOp(char* dest, enum TAC_OP op, char* src){
	
	struct TAC* t = makeTAC();
	
    t->kind = TAC_BINARY_OP;
    sprintf(t->dest, "%s", dest);
    t->op = op;
    sprintf(t->arg1, "%s", src);
    
    return t;
}

struct TAC* makeTACBinOpImmediate(char* tmp, enum TAC_OP op, int32_t immediate){
    struct TAC* t = makeTAC();
    t->kind = TAC_BINARY_OP_IMMEDIATE;
    sprintf(t->dest, "%s", tmp);
    t->op = op;
    t->const_value = immediate;
    return t;
}

struct TAC* makeTACUnaryOp(char* dest, char* src, enum TAC_OP op){
    struct TAC* t = makeTAC();
    t->kind = TAC_UNARY_OP;
    sprintf(t->dest, "%s", dest);
    sprintf(t->arg1, "%s", src);
    t->op = op;
    return t;
}

struct TAC* makeTACStoreConstAddr(uint32_t addr, char* src){

    struct TAC* t = makeTAC();
    t->kind = TAC_STORE_CONST_ADDR;
    t->const_value = addr;
    sprintf(t->arg1, "%s", src);
    return t;
}

struct TAC* makeTACLoadConstAddr(char* dest, uint32_t addr){
    struct TAC* t = makeTAC();
    t->kind = TAC_LOAD_CONST_ADDR;
    sprintf(t->dest, "%s", dest);
    t->const_value = addr;
    return t;
}


struct TAC* makeTACParam(char* dest){
	struct TAC* t = makeTAC();
    t->kind = TAC_PARAM;
    sprintf(t->dest, "%s", dest);
    return t;
}

struct TAC* makeTACCall(uint32_t tmp, char* function_name){
	struct TAC* t = makeTAC();
    t->kind = TAC_CALL;
    sprintf(t->dest, "t%d", tmp);
    strcpy(t->arg1, function_name);
    return t;
}

struct TAC* makeTACNop(){
	struct TAC* t = makeTAC();
    t->kind = TAC_NOP;
    return t;
}

struct TAC* makeTACReturn(char* tmp) {
    struct TAC* t = makeTAC();
    t->kind = TAC_RETURN;
    sprintf(t->dest, "%s", tmp);
    return t;
}

bool tac_is_unconditional_jump(struct TAC* tac){
    return tac->kind == TAC_GOTO || tac->kind == TAC_RETURN;
}
