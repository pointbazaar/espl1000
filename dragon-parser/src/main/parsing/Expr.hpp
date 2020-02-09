#ifndef EXPR
#define EXPR

#include <optional>
#include <vector>

class Op;
class Term;
#include "../commandline/TokenList.hpp"

using namespace std;

class Expr {

public:
	//DragonExpressionNode should be similar to jack expression
	//an expression should be anything that returns a value or computes to a value

	Expr(TokenList tkl, bool debug);
	Expr(Term* myterm);
	Expr(Term* leftTerm, Op* op, Term* rightTerm);

	void performTreeTransformation(
		vector<Op*> ops,
		vector<Term*> terms
	);

	Term* term1 = NULL;
	optional<Op*> op = nullopt;
	optional<Term*> term2 = nullopt;
};
#endif