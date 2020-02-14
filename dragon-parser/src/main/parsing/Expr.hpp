#ifndef EXPR
#define EXPR

#include <vector>

class Op;
struct Term;
#include "../commandline/TokenList.hpp"

using namespace std;

struct Expr {

	struct Term* term1 = NULL;

	//these 2 may be NULL
	struct Op* op = NULL;
	struct Term* term2 = NULL;
};


//DragonExpressionNode should be similar to jack expression
//an expression should be anything that returns a value or computes to a value

struct Expr* makeExpr(TokenList* tkl, bool debug);
struct Expr* makeExpr(struct Term* myterm);
struct Expr* makeExpr(struct Term* leftTerm, struct Op* op, struct Term* rightTerm);
struct Expr* performTreeTransformation(vector<struct Op*> ops,vector<struct Term*> terms);

#endif