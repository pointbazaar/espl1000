#ifndef CCODEGEN
#define CCODEGEN

struct Flags;
struct Ctx;

extern unsigned int label_count;

bool transpile_and_write_c_code(char* c_filename, char* h_filename, struct AST* ast, struct Flags* flags, struct Ctx* ctx);


void transpileStmtBlock(struct StmtBlock* block, struct Ctx* ctx);


#endif
