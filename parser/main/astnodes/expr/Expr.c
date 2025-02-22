#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Expr.h"
#include "Term.h"
#include "UnOpTerm.h"
#include "Op.h"

#include "ast/util/copy_ast.h"

#include "token/list/TokenList.h"
#include "token/token/token.h"

struct Expr* fullTreeTransformation(
    enum OP* ops,
    struct UnOpTerm** terms,
    int count,
    int start_terms,
    int end_terms);

struct Expr* makeExpr_1(struct UnOpTerm* term) {
	struct Expr* res = make(Expr);

	res->super.line_num = term->super.line_num;
	res->super.annotations = term->super.annotations;

	res->term1 = term;
	res->op = OP_NONE;
	res->term2 = NULL;

	return res;
}

struct Expr* makeExpr_3(struct UnOpTerm* leftTerm, enum OP op, struct UnOpTerm* rightTerm) {

	struct Expr* res = make(Expr);

	res->super.line_num = leftTerm->super.line_num;
	res->super.annotations = leftTerm->super.annotations;

	res->term1 = leftTerm;
	res->op = op;
	res->term2 = rightTerm;
	return res;
}

struct Expr* makeExpr(struct TokenList* tokens) {

	int capacity = 10; //initial capacity
	int count = 0;

	enum OP* ops = malloc(sizeof(enum OP) * capacity);
	struct UnOpTerm** terms = malloc(sizeof(struct UnOpTerm*) * capacity);
	;

	struct TokenList* copy = list_copy(tokens);

	struct UnOpTerm* first_term = makeUnOpTerm(copy);
	if (first_term == NULL) {
		free(ops);
		free(terms);
		goto error;
	}

	terms[count++] = first_term;

	while (list_size(copy) >= 2) {

		struct TokenList* copy2 = list_copy(copy);

		enum OP myop = makeOp(copy2);
		if (myop == OP_NONE) {
			freeTokenListShallow(copy2);
			break;
		}

		struct UnOpTerm* myterm = makeUnOpTerm(copy2);
		if (myterm == NULL) {
			freeTokenListShallow(copy2);
			break;
		}

		if (count >= capacity) {

			capacity *= 2;
			ops = realloc(ops, sizeof(enum OP) * capacity);
			terms = realloc(terms, sizeof(struct UnOpTerm*) * capacity);
		}

		ops[count - 1] = myop;
		terms[count++] = myterm;

		list_set(copy, copy2);
		freeTokenListShallow(copy2);
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	struct Expr* res = fullTreeTransformation(ops, terms, count, 0, count - 1);

	free(terms);
	free(ops);

	return res;

error:
	freeTokenListShallow(copy);
	return NULL;
}

int prec_index(enum OP op) {

	//Operator Precedences (lower number means higher precedence)
	//these should be only the binary operators
	//between UnOpTerm s
	enum OP a[] = {

	    //arithmetic (5)
	    OP_MULTIPLY,
	    OP_PLUS,
	    OP_MINUS,
	    OP_SHIFT_LEFT,
	    OP_SHIFT_RIGHT,

	    //relational (6)
	    OP_NEQ,
	    OP_EQ,
	    OP_GE,
	    OP_LE,
	    OP_GT,
	    OP_LT,

	    //logical (3)
	    OP_AND,
	    OP_OR,
	    OP_XOR,
	};

	for (int i = 0; i < 14; i++) {
		if (op == a[i]) return i;
	}

	//should never happen, but we return a valid value.
	return 0;
}

static int indexWithLeastPrecedence(enum OP* ops, int start, int end) {

	//last index into the operators (lowest precedence)
	int least = 0;
	int indexOfFoundOp = 0;

	for (int i = start; i <= end; i++) {

		enum OP o1 = ops[i];

		if (prec_index(o1) > least) {
			least = prec_index(o1);
			indexOfFoundOp = i;
		}
	}

	return indexOfFoundOp;
}

static struct UnOpTerm* unopterm_from_expr(struct Expr* expr) {
	struct UnOpTerm* ttmp = make(UnOpTerm);
	ttmp->op = OP_NONE;
	ttmp->term = makeTerm_other(expr);
	return ttmp;
}

struct Expr* fullTreeTransformation(
    enum OP* ops,
    struct UnOpTerm** terms,
    int count,
    int start_terms, //inclusive
    int end_terms //inclusive
) {

	//transform the list into a tree, respecting operator precedence
	//this is a recursive divide-and-conquer approach.
	//we find an operator with least precedence and split the list there

	//we only have 1 term left here
	if (end_terms - start_terms == 0)
		return makeExpr_1(terms[start_terms]);

	//search for the operator with the least precedence.
	const int index_least_precedence = indexWithLeastPrecedence(ops, start_terms, end_terms - 1);

	enum OP op = ops[index_least_precedence];

	//if theres only 1 term left on either side, we do not create expr

	struct UnOpTerm* uo1;
	struct UnOpTerm* uo2;

	int start1 = start_terms;
	int end1 = index_least_precedence;

	int start2 = index_least_precedence + 1;
	int end2 = end_terms;

	if (end1 - start1 == 0) {
		uo1 = terms[start1];
	} else {
		struct Expr* e1 = fullTreeTransformation(ops, terms, count, start1, end1);
		uo1 = unopterm_from_expr(e1);
	}

	if (end2 - start2 == 0) {
		uo2 = terms[start2];
	} else {
		struct Expr* e2 = fullTreeTransformation(ops, terms, count, start2, end2);
		uo2 = unopterm_from_expr(e2);
	}

	return makeExpr_3(uo1, op, uo2);
}
