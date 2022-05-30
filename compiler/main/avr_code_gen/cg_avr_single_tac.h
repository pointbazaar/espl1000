
#ifndef SMALLDRAGON_TOPLEVEL_CG_AVR_SINGLE_TAC_H
#define SMALLDRAGON_TOPLEVEL_CG_AVR_SINGLE_TAC_H

struct Ctx;

void emit_asm_avr_single_tac(struct RAT* rat, struct TAC *tac, struct Ctx *ctx, FILE *fout);

#endif
