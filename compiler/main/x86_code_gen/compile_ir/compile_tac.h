#pragma once
#include <stdio.h>
#include <stdbool.h>

#include "ibuffer/ibuffer_x86.h"

struct RAT;
struct TAC;
struct Ctx;
struct ST;

void compile_tac_return_x86(struct RAT* rat, struct TAC* tac, struct Ctx* ctx, struct IBuffer* ibu);
void compile_tac_const_value_x86(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu);
void compile_tac_const_data_x86(struct RAT* rat, struct TAC* tac, struct Ctx* ctx, struct IBuffer* ibu);

void compile_tac_copy_x86(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu);
void compile_tac_load_local_addr_x86(struct RAT* rat, struct TAC* tac, struct Ctx* ctx, struct IBuffer* ibu);
void compile_tac_load_function_ptr_x86(struct RAT* rat, struct TAC* tac, struct Ctx* ctx, struct IBuffer* ibu);
void compile_tac_store_local_x86(struct RAT* rat, struct TAC* tac, struct Ctx* ctx, struct IBuffer* ibu);

bool compile_tac_binary_op_x86(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu);
void compile_tac_goto_x86(struct TAC* tac, struct IBuffer* ibu);
void compile_tac_nop_x86(struct IBuffer* ibu);

void compile_tac_if_goto_x86(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu);
void compile_tac_if_cmp_goto_x86(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu);

bool compile_tac_call_x86(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu, struct Ctx* ctx, char* current_function_name);
void compile_tac_icall_x86(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu, struct Ctx* ctx, char* current_function_name);
bool compile_tac_label_x86(struct TAC* tac, struct IBuffer* ibu, struct Ctx* ctx);
void compile_tac_unary_op_x86(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu);
void compile_tac_param_x86(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu);
void compile_tac_binary_op_immediate_x86(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu);

void compile_tac_setup_stackframe_x86(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu, struct Ctx* ctx, char* current_function_name);
void compile_tac_setup_sp_x86(struct IBuffer* ibu);

void compile_tac_load_x86(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu);
void compile_tac_store_x86(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu);
