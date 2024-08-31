#ifndef SMALLDRAGON_TOPLEVEL_COMPILE_TAC_H
#define SMALLDRAGON_TOPLEVEL_COMPILE_TAC_H

#include <stdio.h>

#include "ibuffer/ibuffer.h"

struct RAT;
struct TAC;
struct Ctx;
struct ST;

void compile_tac_return(struct RAT* rat, struct TAC* tac, struct Ctx* ctx, struct IBuffer* ibu);
void compile_tac_const_value(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu);

void compile_tac_copy(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu);
void compile_tac_load_local(struct RAT* rat, struct TAC* tac, struct Ctx* ctx, struct IBuffer* ibu);
void compile_tac_load_local_addr(struct RAT* rat, struct TAC* tac, struct Ctx* ctx, struct IBuffer* ibu);
void compile_tac_store_local(struct RAT* rat, struct TAC* tac, struct Ctx* ctx, struct IBuffer* ibu);

void compile_tac_binary_op(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu);
void compile_tac_goto(struct TAC* tac, struct IBuffer* ibu);
void compile_tac_nop(struct IBuffer* ibu);

void compile_tac_if_goto(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu);
void compile_tac_if_cmp_goto(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu);

void compile_tac_call(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu, struct Ctx* ctx);
void compile_tac_label(struct TAC* tac, struct IBuffer* ibu, struct Ctx* ctx);
void compile_tac_unary_op(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu);
void compile_tac_param(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu);
void compile_tac_binary_op_immediate(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu);
void compile_tac_load_const_addr(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu);
void compile_tac_store_const_addr(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu);

void compile_tac_setup_stackframe(struct TAC* tac, struct IBuffer* ibu);
void compile_tac_setup_sp(struct RAT* rat, struct IBuffer* ibu);

void compile_tac_load(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu);
void compile_tac_store(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu);
#endif
