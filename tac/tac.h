#ifndef SMALLDRAGON_TOPLEVEL_TAC_H
#define SMALLDRAGON_TOPLEVEL_TAC_H

#include "ast/ast_declare.h"

#include "rat/rat.h"

enum TAC_OP{
    TAC_OP_NONE, //for error cases

    //arithmetic operators
    TAC_OP_ADD,
    TAC_OP_SUB,
    TAC_OP_MUL,

    //unary
    TAC_OP_UNARY_MINUS,
    TAC_OP_UNARY_NOT,
    TAC_OP_UNARY_BITWISE_NEG,

    //logical operators
    TAC_OP_AND,
    TAC_OP_OR,

    //comparison operators
    TAC_OP_CMP_LT, //<,>    //brlt
    TAC_OP_CMP_GE, //>=,<=  //brge
    TAC_OP_CMP_EQ, //==,    //breq
    TAC_OP_CMP_NEQ, //!=    //brne
};

enum TAC_KIND{

    TAC_BINARY_OP = 0, //e.g. t1 += t2
    TAC_UNARY_OP, //e.g. t1 = -t2;

    TAC_GOTO, //unconditional jump
    TAC_IF_GOTO, //conditional jump depending on a single temporary

    TAC_COPY,

    TAC_CONST_VALUE,

    TAC_CALL, //call to a label (string) without anything else
    TAC_PARAM,
    TAC_RETURN, //return, without arguments

    TAC_NOP,
    TAC_LABEL,

    TAC_BINARY_OP_IMMEDIATE, //NEW TODO

    TAC_STORE_CONST_ADDR, // [const_dest] = src //memory store
    TAC_LOAD_CONST_ADDR,  // dest = [const_src] //memory load
};

#define TAC_NO_LABEL 0

//this is our 2-Address Code
//it easily maps onto x86 and avr_code_gen assembly
//which assumes 1 operation and 2 registers,
//dest and src.
//where dest = dest op src
struct TAC{
    uint32_t index; //for when we put it in a TACBuffer

    //three address code
    uint32_t label_index;

    char dest[DEFAULT_STR_SIZE];
    enum TAC_KIND kind;
    char arg1[DEFAULT_STR_SIZE];

    enum TAC_OP op;

    int32_t const_value;
};

bool tac_is_unconditional_jump(struct TAC* tac);

char* tac_tostring(struct TAC* tac);
uint32_t make_label();
uint32_t make_temp();

//for most TACs, there should be a dedicated constructor,
//which helps with avoiding invalid state and malformed TACs

struct TAC* makeTACLabel(uint32_t label);
struct TAC* makeTACLabel2(char* label);
struct TAC* makeTACGoto(uint32_t label);
struct TAC* makeTACReturn(char* tmp);
struct TAC* makeTACIfGoto(char* tmp_condition, uint32_t label_destination);
struct TAC* makeTACCopy(char* dest, char* src);
struct TAC* makeTACConst(uint32_t tmp, int value);

struct TAC* makeTACBinOp(char* dest, enum TAC_OP op, char* src);
struct TAC* makeTACBinOpImmediate(char* tmp, enum TAC_OP op, int32_t immediate);
struct TAC* makeTACUnaryOp(char* dest, char* src, enum TAC_OP op);

struct TAC* makeTACStoreConstAddr(uint32_t addr, char* src);
struct TAC* makeTACLoadConstAddr(char* dest, uint32_t addr);

struct TAC* makeTACParam(char* dest);
struct TAC* makeTACCall(uint32_t tmp, char* function_name);
#endif
