#ifndef SMALLDRAGON_TOPLEVEL_COMPILE_TAC_H
#define SMALLDRAGON_TOPLEVEL_COMPILE_TAC_H
#include <stdio.h>
struct RAT;
struct TAC;
struct Ctx;

void compile_tac_return(struct RAT* rat, struct TAC* tac, struct Ctx* ctx, FILE* fout);
void compile_tac_const_value(struct RAT* rat, struct TAC* tac, FILE* fout);
void compile_tac_copy(struct RAT* rat, struct TAC* tac, struct Ctx* ctx, FILE* fout);
void compile_tac_binary_op(struct RAT* rat, struct TAC* tac, FILE* fout);
void compile_tac_goto(struct TAC* tac, FILE* fout);
void compile_tac_nop(FILE* fout);
void compile_tac_if_goto(struct RAT* rat, struct TAC* tac, FILE* fout);
void compile_tac_call(struct RAT* rat, struct TAC* tac, FILE* fout);
void compile_tac_label(struct TAC* tac, FILE* fout);
void compile_tac_unary_op(struct RAT* rat, struct TAC* tac, FILE* fout);
void compile_tac_param(struct RAT* rat, struct TAC* tac, FILE* fout);
void compile_tac_binary_op_immediate(struct RAT* rat, struct TAC* tac, FILE* fout);
void compile_tac_load_const_addr(struct RAT* rat, struct TAC* tac, FILE* fout);
void compile_tac_store_const_addr(struct RAT* rat, struct TAC* tac, FILE* fout);
void compile_tac_setup_stackframe(struct TAC* tac, FILE* fout);
#endif
