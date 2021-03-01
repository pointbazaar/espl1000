#ifndef AST_WHOLE_PROGRAM_PARSE
#define AST_WHOLE_PROGRAM_PARSE

#include <stdbool.h>
#include "ast/ast.h"

struct TokenList;

struct AST_Whole_Program* makeAST_Whole_Program(struct Namespace* myNamespace);
struct AST_Whole_Program* makeAST_Whole_Program3(struct TokenList* tokens, char* myNamespace, bool debug);

#endif
