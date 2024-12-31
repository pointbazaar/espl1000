#pragma once

struct Ctx;
struct IBuffer;
struct RAT;
struct TAC;

void emit_asm_x86_single_tac(struct RAT* rat, struct TAC* tac, struct Ctx* ctx, struct IBuffer* ibu, char* current_function_name);
