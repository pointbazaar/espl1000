#pragma once

#include <stdbool.h>

struct Flags;
struct Ctx;
struct AST;
struct IBuffer;

void emit_call_main_endloop(struct IBuffer* ibu);

//void emit_setup_stack_pointer_avr(FILE* fout);

bool compile_and_write_avr(struct AST* ast, struct Ctx* ctx);
