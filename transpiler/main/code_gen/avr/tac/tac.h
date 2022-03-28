#ifndef SMALLDRAGON_TOPLEVEL_TAC_H
#define SMALLDRAGON_TOPLEVEL_TAC_H

#include "ast/ast_declare.h"

#include "code_gen/avr/tacbuffer/tacbuffer.h"
#include "code_gen/avr/rat/rat.h"

enum TAC_OP{
    TAC_OP_NONE, //for error cases

    //arithmetic operators
    TAC_OP_ADD,
    TAC_OP_SUB,
    TAC_OP_MUL,
    TAC_OP_DIV,

    //logical operators
    TAC_OP_AND,
    TAC_OP_OR,

    //comparison operators
    TAC_OP_CMP_LT, //<
    TAC_OP_CMP_LE, //<=
    TAC_OP_CMP_GT, //>
    TAC_OP_CMP_GE, //>=
    TAC_OP_CMP_EQ, //==
    TAC_OP_CMP_NEQ, //!=
};

enum TAC_KIND{

    TAC_BINARY_OP = 0, //e.g. t1 += t2
    TAC_UNARY_OP, //e.g. t1 = -t2;

    TAC_GOTO, //unconditional jump
    TAC_IF_GOTO, //conditional jump depending on a single temporary

    TAC_DEREF, //currently unused

    TAC_COPY,

    TAC_CONST_VALUE,

    TAC_CALL, //call to a label (string) without anything else
    TAC_PARAM,
    TAC_RETURN, //return, without arguments

    TAC_NOP,
};

#define TEMP_SIZE 20
#define TAC_NO_LABEL 0

//this is our 2-Address Code
//it easily maps onto x86 and avr assembly
//which assumes 1 operation and 2 registers,
//dest and src.
//where dest = dest op src
struct TAC{
    uint32_t index; //for when we put it in a TACBuffer

    //three address code
    uint32_t label_index;
    char label_name[DEFAULT_STR_SIZE]; //for labels with names, like functions

    char dest[TEMP_SIZE];
    enum TAC_KIND kind;
    char arg1[TEMP_SIZE];

    enum TAC_OP op;

    uint32_t goto_index;

    int32_t const_value;
};

bool tac_is_unconditional_jump(struct TAC* tac);

struct TAC* makeTAC();
char* tac_tostring(struct TAC* tac);
uint32_t make_label();
uint32_t make_temp();

struct TAC* makeTACLabel(uint32_t label);
struct TAC* makeTACGoto(uint32_t label);
struct TAC* makeTACIfGoto(char* tmp_condition, uint32_t label_destination);

#endif
