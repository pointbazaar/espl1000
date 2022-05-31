
#ifndef SMALLDRAGON_TOPLEVEL_CG_AVR_SINGLE_FUNCTION_H
#define SMALLDRAGON_TOPLEVEL_CG_AVR_SINGLE_FUNCTION_H
#include <stdio.h>
struct Method;
struct Ctx;

void emit_create_stack_frame(uint32_t stack_frame_size, FILE* fout);

void compile_and_write_avr_single_function(struct Method* m, struct Ctx* ctx, FILE* fout);
#endif
