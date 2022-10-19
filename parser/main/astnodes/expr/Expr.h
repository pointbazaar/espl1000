#ifndef EXPR
#define EXPR

#include <string.h>
#include <stdbool.h>

#include "ast/ast.h"

struct TokenList;

//an expression should be anything that returns a value or computes to a value

struct Expr* makeExpr(struct TokenList* tokens);
struct Expr* makeExpr_1(struct UnOpTerm* myterm);
struct Expr* makeExpr_3(struct UnOpTerm* leftTerm, struct Op* op, struct UnOpTerm* rightTerm);

int find(void** arr, int size, void* element);

int prec_index(char* op);

void** insert(void** arr, int index, void* element, int size_before);

void** erase(void** arr, int index, int size_before);


#endif
