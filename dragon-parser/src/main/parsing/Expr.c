#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../commandline/TokenList.h"
#include "Expr.h"
#include "Term.h"
#include "Op.h"

struct Expr* makeExpr_1(struct Term* term) {
	struct Expr* res = malloc(sizeof(struct Expr));
	res->term1 = term;
	res->op    = NULL;
	res->term2 = NULL;
	return res;
}

struct Expr* makeExpr(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("Expr(...) from %s\n", list_code(tokens, debug));
	}

	//we assume they never have more than 200 terms

	struct Op** ops = malloc(sizeof(struct Op*)*200);
	int opsc = 0;

	struct Term** terms = malloc(sizeof(struct Term*)*200);;
	int termsc = 0;

	if(terms == NULL || ops == NULL){
		return NULL;
	}

	struct TokenList* copy = list_copy(tokens);
	
	struct Term* myterm2 = makeTerm(copy,debug);
	if(myterm2 == NULL){return NULL;}
	terms[termsc++] = myterm2;
	
	bool fail = false;
	while (list_size(copy) >= 2 && !fail) {
		struct TokenList* copy2 = list_copy(copy);

		struct Op* myop = makeOp(copy2,debug);
		if(myop == NULL){fail = true; break;}

		struct Term* myterm = makeTerm(copy2,debug);
		if(myterm == NULL){fail = true; break;}

		ops[opsc++] = myop;
		terms[termsc++] = myterm;

		list_set(copy, copy2);
	}

	list_set(tokens, copy);

	return performTreeTransformation(ops,opsc, terms,termsc);
}

struct Expr* makeExpr_3(struct Term* leftTerm, struct Op* op, struct Term* rightTerm) {

	struct Expr* res = malloc(sizeof(struct Expr));
	res->term1 = leftTerm;
	res->op    = op;
	res->term2 = rightTerm;
	return res;
}

int prec_index(char* op){
	/*
	Operator Precedences (lower number means higher precedence)
	3   *,/,%
	4   +,-
	5   <<,>>
	7   !=,==
	11  &&
	12  ||
	 */
	const char* a[nops];

	a[0] = "*";
	a[1] = "/";
	a[2] = "%";
	a[3] = "+";
	a[4] = "-";
	a[5] = "<<";
	a[6] = ">>";
	a[7] = "!=";
	a[8] = "==";
	a[9] = "&&";
	a[10] = "||";

	for(int i=0;i<nops;i++){
		if(strcmp(op,a[i])==0){ return i; }
	}

	return -1;
}

int find(void** arr, int size, void* element){

	for(int i = 0; i < size; i++){

		if(arr[i] == element){
			return i;
		}
	}
	return -1;
}

struct Expr* performTreeTransformation(
		struct Op** ops, int opsc,
		struct Term** terms, int termsc
){
	//transform the list into a tree, respecting operator precedence

	struct Expr* res = malloc(sizeof(struct Expr));
	res->term1 = NULL;
	res->op    = NULL;
	res->term2 = NULL;

	/*
	Algorithm:

	while there are more than 2 terms then
		Find an operator with largest precedence and make a new Expression from the 2 Terms surrounding it.
		Put that new Expression back into the list.

	assign the 2 terms to this node
	 */

	while (termsc > 2){
		struct Op* opWithLargestPrecedence = ops[0];
		int lowest = nops-1;	//last index into the operators (lowest precedence)

		for(int i = 0;i < opsc;i++){
			struct Op* o1 = ops[i];
			if(prec_index(o1->op) < lowest){
				lowest = prec_index(o1->op);
				opWithLargestPrecedence = o1;
			}
		}

		int indexOfOp = find((void**)ops,opsc,opWithLargestPrecedence);


		struct Term* leftTerm = terms[indexOfOp];
		struct Term* rightTerm = terms[indexOfOp+1];

		struct Expr* expr = makeExpr_3(leftTerm,opWithLargestPrecedence,rightTerm);
		if(expr == NULL){return NULL;}

		//simplify
		int i1;
		i1 = find((void**)terms,termsc,leftTerm);
		
		terms = (struct Term**)erase((void**)terms,i1, termsc);
		termsc--;

		i1 = find((void**)terms,termsc,rightTerm);
		
		terms = (struct Term**)erase((void**)terms,i1, termsc);
		termsc--;

		const int i2 = find((void**)ops,opsc,opWithLargestPrecedence);
		ops = (struct Op**)erase((void**)ops,i2,opsc);
		opsc--;

		//insert newly created expression
		struct Term* ttmp = makeTerm_other(expr);
		if(ttmp == NULL){ return NULL; }

		//list_insert occurs here only once,
		//so i do not implement special function
		//list_insert(terms, indexOfOp, ttmp);
		terms = (struct Term**)insert((void**)terms, indexOfOp, (void*)ttmp, termsc);
		termsc++; //because we inserted
	}

	//now only 2 terms left
	res->term1 = terms[0];

	//in case of only one term
	if(opsc > 0) {
		res->op = ops[0];
		res->term2 = terms[1];
	}

	return res;
}

void** insert(void** arr, int index, void* element, int size_before){
	//insert 'element' into 'arr' at index 'index'
	void** res = malloc(sizeof(void*)*(size_before+1));

	for(int i=0;i<size_before+1;i++){
		if(i < index){
			res[i] = arr[i];
		}else if(i == index){
			res[i] = element;
		}else{
			res[i] = arr[i-1];
		}
	}
	return res;
}

void** erase(void** arr, int index, int size_before){
	//erase the element at 'index'
	void** res = malloc(sizeof(void*)*(size_before-1));

	int i1 = 0;
	for(int i=0;i<size_before-1;i++){
		if(i1 == index){
			i1++;
		}
		res[i] = arr[i1++];
	}
	return res;
}