#ifndef JAVA_CODE_GEN
#define JAVA_CODE_GEN

#include "../ast/ast.h"

int gen_java_code(struct AST_Whole_Program* ast, char* fname_out);

void gen_java_namespace(struct Namespace* ns, FILE* file);

#endif
