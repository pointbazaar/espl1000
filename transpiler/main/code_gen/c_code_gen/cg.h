#ifndef CCODEGEN
#define CCODEGEN

struct Flags;
struct Ctx;

extern unsigned int label_count;

bool transpile_and_write(char* c_filename, char* h_filename, struct AST* ast, struct Flags* flags);


void transpileStmtBlock(struct StmtBlock* block, struct Ctx* ctx);


#endif
