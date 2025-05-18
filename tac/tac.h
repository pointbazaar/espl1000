#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct Ctx;
struct ST;
struct SST;
struct LVST;

struct TAC;

extern struct ST* ctx_tables(struct Ctx* ctx);

enum TAC_OP {
	TAC_OP_NONE, //for error cases

	//arithmetic operators
	TAC_OP_ADD,
	TAC_OP_SUB,
	TAC_OP_MUL,
	TAC_OP_DIV,
	TAC_OP_MOD,

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

enum TAC_KIND {

	TAC_BINARY_OP = 0, //e.g. t1 += t2
	TAC_UNARY_OP = 1, //e.g. t1 = -t2;

	TAC_GOTO = 2, //unconditional jump
	TAC_IF_GOTO = 3, //conditional jump depending on a single temporary
	TAC_IF_CMP_GOTO = 4,

	TAC_COPY = 5, //copy one temp into another
	TAC_STORE_LOCAL = 6, //store local var into stackframe
	TAC_LOAD_LOCAL_ADDR = 7, //load the addr of a local variable
	TAC_LOAD_FUNCTION_PTR = 8, //load the addr of a function

	TAC_CONST_VALUE = 9,

	// Load a symbol from .data segment into register.
	// The assembler will figure out the rest here.
	TAC_CONST_DATA = 10,

	TAC_CALL = 11, //call to a label (string) without anything else
	TAC_ICALL = 12, // call to a temporary 't1 = call t2'.
	                // So we can comput arbitrary address to call.
	                // Good for calling function pointers
	TAC_PARAM = 13,
	TAC_RETURN = 14, //return, without arguments

	TAC_SETUP_STACKFRAME = 15, //create space on stack for locals, load SP
	TAC_SETUP_SP = 16, //setup the stack pointer. used at the start of programs

	TAC_NOP = 17,

	TAC_LABEL_INDEXED = 18,
	TAC_LABEL_FUNCTION = 19,

	TAC_LOAD = 20, //t1 = [t2]
	TAC_STORE = 21, //[t1] = t2
};

#define TAC_NO_LABEL 0

#include "tac_ctor.h"

// returns an optional IR temporary which is written
// in this statement
// @returns temporary index >= 0 if there is one
// @returns < 0     if there is no destination temporary
int32_t tac_opt_dest(struct TAC* tac);

// protected member access
// @returns < 0 on error
int32_t tac_dest(struct TAC* tac);
// protected member access
// @returs < 0 on error
int64_t tac_arg1(struct TAC* tac);
// protected member access
int64_t tac_const_value(struct TAC* tac);
// protected member access
// @returns < 0 on error
int32_t tac_label_index(struct TAC* tac);
// protected member access
// @return TAC_OP_NONE on error
enum TAC_OP tac_op(struct TAC* tac);
// protected member access
enum TAC_KIND tac_kind(struct TAC* tac);

bool tac_param_is_syscall(struct TAC* tac);

// @returns  the width (in bytes) of a load/store operation.
//           Meaning how many bytes to load/store.
uint8_t tac_load_store_width(struct TAC* tac);

// only valid for TAC_PARAM
uint32_t tac_param_index(struct TAC* tac);

// @returns  the highest index of an IR temorary variable
//           used in this IR statement
//           e.g. t4 += t7  -> 7
//           e.g. t0 = 0x83 -> 0
//           e.g. setup SP  -> 0
// @returns  0 if no IR temporary was used
// @returns < 0 on error
int32_t tac_max_temp(struct TAC* tac);

// @returns   true if this IR statement may alter control flow
//            e.g. TAC_GOTO, TAC_IF_GOTO
bool tac_may_branch_to_label(struct TAC* tac);

// @param tac   the IR statement
// @returns     true if this IR statement needs a register for storage of a temporary
bool tac_needs_register(struct TAC* tac);

// marks used_map[i] = true
// if temporary 'i' is used as a value
// in this statement. e.g.
// temp1 += temp0
// means used_map[0] == true for temp1
// and used_map[1] == true also.
// @returns 0 on succes
int tac_mark_used(struct TAC* tac, bool* used_map, size_t map_size);

// marks defines_map[i] = true
// if temporary 'i' is defined in this statement
// temp1 += temp0
// means defines_map[0] == false
// means defines_map[1] == true
// @returns 0 on succes
int tac_mark_defines(struct TAC* tac, bool* defines_map, size_t map_size);

bool tac_is_unconditional_jump(struct TAC* tac);

// @returns NULL on error
char* tac_tostring(struct TAC* tac, struct SST* sst, struct LVST* lvst);

// @returns approximate source line number of the
// AST node used to generate the IR statement
// @returns 0   if there is no line number
uint32_t tac_line_num(struct TAC* tac);
