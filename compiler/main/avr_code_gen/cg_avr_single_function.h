
#ifndef SMALLDRAGON_TOPLEVEL_CG_AVR_SINGLE_FUNCTION_H
#define SMALLDRAGON_TOPLEVEL_CG_AVR_SINGLE_FUNCTION_H
#include <stdio.h>
struct Method;
struct Ctx;

void compile_and_write_avr_single_function(struct Method* m, struct Ctx* ctx, FILE* fout);
#endif
