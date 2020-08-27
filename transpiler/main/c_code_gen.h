#ifndef CCODEGEN
#define CCODEGEN

#include "../../ast/ast.h"
#include "flags.h"

void transpileAndWrite(char* filename, struct AST_Whole_Program* ast, struct Flags* flags);

#endif
