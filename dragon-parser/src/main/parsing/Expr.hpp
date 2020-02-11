#ifndef EXPR
#define EXPR

#include <vector>

class Op;
class Term;
#include "../commandline/TokenList.hpp"

using namespace std;

struct Expr {

	Term* term1 = NULL;

	//these 2 may be NULL
	Op* op = NULL;
	Term* term2 = NULL;
};


//DragonExpressionNode should be similar to jack expression
//an expression should be anything that returns a value or computes to a value

struct Expr* makeExpr(TokenList* tkl, bool debug);
struct Expr* makeExpr(Term* myterm);
struct Expr* makeExpr(Term* leftTerm, Op* op, Term* rightTerm);
struct Expr* performTreeTransformation(vector<Op*> ops,vector<Term*> terms);

#endif