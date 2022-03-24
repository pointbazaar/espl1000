#ifndef SMALLDRAGON_TOPLEVEL_CG_AVR_H
#define SMALLDRAGON_TOPLEVEL_CG_AVR_H

#include <stdbool.h>

struct Flags;
struct Ctx;
struct AST;

bool compile_and_write_avr(char* asm_file_filename, struct AST* ast, struct Ctx* ctx);

#endif
