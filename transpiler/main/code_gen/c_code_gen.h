#ifndef CCODEGEN
#define CCODEGEN

struct AST_Whole_Program;
struct Flags;

bool transpileAndWrite(char* filename, struct AST_Whole_Program* ast, struct Flags* flags);

#endif
