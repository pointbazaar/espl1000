#ifndef SMALLDRAGON_X86_H
#define SMALLDRAGON_X86_H

struct Flags;
struct Ctx;

bool compile_and_write_x86(char* asm_file_filename, struct AST* ast, struct Flags* flags);

#endif
