#ifndef SMALLDRAGON_TOPLEVEL_CG_AVR_H
#define SMALLDRAGON_TOPLEVEL_CG_AVR_H

#include <stdbool.h>

struct Flags;
struct Ctx;
struct AST;

void emit_call_main_endloop(FILE* fout);

void emit_setup_stack_pointer_avr(FILE* fout);

bool compile_and_write_avr(char* asm_file_filename, struct AST* ast, struct Ctx* ctx);

#endif
