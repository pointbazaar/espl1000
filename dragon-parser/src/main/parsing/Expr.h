#ifndef EXPR
#define EXPR

#include <string.h>
#include <stdbool.h>

struct TokenList;
struct Op;
struct Term;

struct Expr {

	struct Term* term1;

	//these 2 may be NULL
	struct Op* op;
	struct Term* term2;
};

//how many operators there are
const int nops = 11;	

//DragonExpressionNode should be similar to jack expression
//an expression should be anything that returns a value or computes to a value

struct Expr* makeExpr(struct TokenList* tkl, bool debug);
struct Expr* makeExpr_1(struct Term* myterm);
struct Expr* makeExpr_3(struct Term* leftTerm, struct Op* op, struct Term* rightTerm);

struct Expr* performTreeTransformation(
	struct Op** ops, int opsc,
	struct Term** terms, int termsc
);

int find(void** arr, int size, void* element);

int prec_index(char* op);

void** insert(void** arr, int index, void* element, int size_before);

void** erase(void** arr, int index, int size_before);

#endif