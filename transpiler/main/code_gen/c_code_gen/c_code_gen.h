#ifndef CCODEGEN
#define CCODEGEN

struct AST_Whole_Program;
struct Flags;
struct Ctx;

extern unsigned int label_count;

bool transpileAndWrite(char* c_filename, char* h_filename, struct AST* ast, struct Flags* flags);


void transpileStmtBlock(struct StmtBlock* block, struct Ctx* ctx);


#endif
