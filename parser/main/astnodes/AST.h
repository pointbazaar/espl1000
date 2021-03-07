#ifndef AST_WHOLE_PROGRAM_PARSE
#define AST_WHOLE_PROGRAM_PARSE

#include <stdbool.h>
#include "ast/ast.h"

struct TokenList;

struct AST* makeAST(struct Namespace* myNamespace);
struct AST* makeAST3(struct TokenList* tokens, char* myNamespace, bool debug);

#endif
