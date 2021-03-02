#ifndef CCODEGEN
#define CCODEGEN

struct AST_Whole_Program;
struct Flags;

bool transpileAndWrite(char* filename, struct AST* ast, struct Flags* flags);

#endif
