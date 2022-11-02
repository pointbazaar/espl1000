#ifndef SMALLDRAGON_TOPLEVEL_TAC_H
#define SMALLDRAGON_TOPLEVEL_TAC_H

struct Ctx;
struct ST;

extern struct ST* ctx_tables(struct Ctx* ctx);

#include "ast/ast_declare.h"

#include "rat/rat.h"

enum TAC_OP{
    TAC_OP_NONE, //for error cases

    //arithmetic operators
    TAC_OP_ADD,
    TAC_OP_SUB,
    TAC_OP_MUL,
    
    TAC_OP_XOR,
    
    //shift operators
    TAC_OP_SHIFT_LEFT,
    TAC_OP_SHIFT_RIGHT,

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
    TAC_IF_CMP_GOTO,

    TAC_COPY, //copy one temp into another
    TAC_LOAD_LOCAL, //load local var from stackframe
    TAC_STORE_LOCAL, //store local var into stackframe

    TAC_CONST_VALUE,

    TAC_CALL, //call to a label (string) without anything else
    TAC_PARAM,
    TAC_RETURN, //return, without arguments
    
    TAC_SETUP_STACKFRAME, //create space on stack for locals, load SP
    TAC_SETUP_SP, //setup the stack pointer. used at the start of programs

    TAC_NOP,
    
    TAC_LABEL_INDEXED,
    TAC_LABEL_FUNCTION,

    TAC_BINARY_OP_IMMEDIATE, //NEW TODO

    TAC_STORE_CONST_ADDR, // [const_dest] = src //memory store
    TAC_LOAD_CONST_ADDR,  // dest = [const_src] //memory load
    
    TAC_LOAD, //t1 = [t2]
    TAC_STORE, //[t1] = t2
};

#define TAC_NO_LABEL 0

//this is our 2-Address Code
//it easily maps onto avr_code_gen assembly
//which assumes 1 operation and 2 registers,
//dest and src.
//where dest = dest op src
struct TAC{
	
    //three address code
    uint32_t label_index;

    uint32_t dest;
    enum TAC_KIND kind;
    uint32_t arg1;

    enum TAC_OP op;

    int32_t const_value;
};

bool tac_is_unconditional_jump(struct TAC* tac);

char* tac_tostring(struct TAC* tac, struct Ctx* ctx);
uint32_t make_label();
uint32_t make_temp();

//for most TACs, there should be a dedicated constructor,
//which helps with avoiding invalid state and malformed TACs

struct TAC* makeTACLabel(uint32_t label);
struct TAC* makeTACLabelFunction(uint32_t sst_index);

struct TAC* makeTACGoto(uint32_t label);
struct TAC* makeTACReturn(uint32_t tmp);

struct TAC* makeTACIfGoto(uint32_t tmp_condition, uint32_t label_destination);
struct TAC* makeTACIfCMPGoto(uint32_t tmp1, enum TAC_OP op, uint32_t tmp2, uint32_t label_destination); 

struct TAC* makeTACCopy(uint32_t dest, uint32_t src);

struct TAC* makeTACLoadLocal(uint32_t tmp, uint32_t local_index);
struct TAC* makeTACStoreLocal(uint32_t local_index, uint32_t tmp);

struct TAC* makeTACConst(uint32_t tmp, int value);

struct TAC* makeTACBinOp(uint32_t dest, enum TAC_OP op, uint32_t src);
struct TAC* makeTACBinOpImmediate(uint32_t tmp, enum TAC_OP op, int32_t immediate);
struct TAC* makeTACUnaryOp(uint32_t dest, uint32_t src, enum TAC_OP op);

struct TAC* makeTACStoreConstAddr(uint32_t addr, uint32_t src);
struct TAC* makeTACLoadConstAddr(uint32_t dest, uint32_t addr);

struct TAC* makeTACParam(uint32_t dest);
struct TAC* makeTACCall(uint32_t tmp, uint32_t function_index);

struct TAC* makeTACSetupStackframe(uint32_t frame_size);
struct TAC* makeTACSetupSP();

struct TAC* makeTACNop();

struct TAC* makeTACLoad(uint32_t tmp, uint32_t taddr);
struct TAC* makeTACStore(uint32_t taddr, uint32_t tmp);
#endif
