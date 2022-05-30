#ifndef CCODEGEN
#define CCODEGEN

struct Flags;
struct Ctx;

bool transpile_and_write_c_headers(char* h_filename, struct AST* ast, struct Flags* flags, struct Ctx* ctx);

#endif
