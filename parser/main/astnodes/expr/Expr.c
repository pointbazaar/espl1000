#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Expr.h"
#include "Term.h"
#include "UnOpTerm.h"
#include "Op.h"

#include "ast/util/free_ast.h"
#include "ast/util/copy_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

//how many operators there are
#define nops 15
struct Expr* fullTreeTransformation(
	struct Op** ops, 
	int opsc,
	struct UnOpTerm** terms, 
	int termsc,
	bool debug
);
void performTreeTransformation(
	struct Op*** ops, int* opsc,
	struct UnOpTerm*** terms, int* termsc,
	bool debug,
	int max_op_index
);
void insertOperatorChaining(
	struct Op*** ops, int* opsc,
	struct UnOpTerm*** terms, int* termsc,
	bool debug
);
bool isComparisonOp(struct Op* op);
//-----------------------------

struct Expr* makeExpr_1(struct UnOpTerm* term) {
	struct Expr* res = make(Expr);
	
	res->super.line_num    = term->super.line_num;
	res->super.annotations = term->super.annotations;
	
	res->term1 = term;
	res->op    = NULL;
	res->term2 = NULL;
	
	return res;
}

struct Expr* makeExpr_3(struct UnOpTerm* leftTerm, struct Op* op, struct UnOpTerm* rightTerm) {

	struct Expr* res = make(Expr);
	
	res->super.line_num    = leftTerm->super.line_num;
	res->super.annotations = leftTerm->super.annotations;
	
	res->term1 = leftTerm;
	res->op    = op;
	res->term2 = rightTerm;
	return res;
}

struct Expr* makeExpr(struct TokenList* tokens, bool debug) {

	//we assume they never have more than 200 terms

	struct Op** ops = malloc(sizeof(struct Op*)*200);
	int opsc = 0;

	struct UnOpTerm** terms = malloc(sizeof(struct UnOpTerm*)*200);;
	int termsc = 0;

	struct TokenList* copy = list_copy(tokens);
	
	struct UnOpTerm* myterm2 = makeUnOpTerm(copy,debug);
	if(myterm2 == NULL){
		free(ops);
		free(terms);
		return NULL;
	}

	terms[termsc++] = myterm2;
	
	while (list_size(copy) >= 2) {
		struct TokenList* copy2 = list_copy(copy);

		struct Op* myop = makeOp(copy2,debug);
		if(myop == NULL){
			freeTokenListShallow(copy2);
			break;
		}

		struct UnOpTerm* myterm = makeUnOpTerm(copy2,debug);
		if(myterm == NULL){
			freeTokenListShallow(copy2);
			break;
		}

		ops[opsc++] = myop;
		terms[termsc++] = myterm;

		list_set(copy, copy2);
		freeTokenListShallow(copy2);
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);
	
	//----------------------------------
	
	struct Expr* res = fullTreeTransformation
	(ops, opsc, terms, termsc, debug);
	
	res->super.line_num    = myterm2->super.line_num;
	res->super.annotations = 0;
	
	free(terms);
	free(ops);
	
	return res;
}

struct Expr* fullTreeTransformation(
	struct Op** ops, 
	int opsc,
	struct UnOpTerm** terms, 
	int termsc,
	bool debug
){
	
	struct Expr* res = make(Expr);
	res->term1 = NULL;
	res->op    = NULL;
	res->term2 = NULL;
	
	struct Op** myops = ops;
	struct UnOpTerm** myterms = terms;

	//only group up to index 6, because the 
	//comparison operators need to be chained later on
	performTreeTransformation(
		&myops, &opsc, &myterms, &termsc, debug, 6
	);
	
	//do comparison operator chaining
	insertOperatorChaining
	(&myops, &opsc, &myterms, &termsc, debug);
	
	//now group the rest
	performTreeTransformation(
		&myops, &opsc, &myterms, &termsc, debug, nops-1
	);
	
	//now only 2 terms left
	res->term1 = myterms[0];
	//in case of more than one term
	if(opsc > 0) {
		res->op = myops[0];
		res->term2 = myterms[1];
	}
	
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

	//arithmetic
	a[0] = "*";
	a[1] = "/";
	a[2] = "%";
	a[3] = "+";
	a[4] = "-";
	a[5] = "<<";
	a[6] = ">>";
	
	//comparison
	a[7] = "!=";
	a[8] = "==";
	a[9] = ">=";
	a[10] = "<=";
	a[11] = ">";
	a[12] = "<";
	
	//logic
	a[13] = "&&";
	a[14] = "||";

	for(int i=0;i<nops;i++){
		if(strcmp(op,a[i])==0){ return i; }
	}

	return -1;
}

bool isComparisonOp(struct Op* op){
	
	const int p = prec_index(op->op);
	return p >= 7 && p <= 12;
}

int find(void** arr, int size, void* element){

	for(int i = 0; i < size; i++){

		if(arr[i] == element){
			return i;
		}
	}
	return -1;
}

void performTreeTransformation(
		struct Op*** ops, 
		int* opsc,
		struct UnOpTerm*** terms, 
		int* termsc,
		bool debug,
		
		//the index of the last operator
		//that will still be grouped in this iteration
		int max_op_index 
){

	if(debug){
		printf("performTreeTransformation(..., %d, ..., %d, %d)\n", *opsc, *termsc, debug);
	}

	//transform the list into a tree, 
	//respecting operator precedence
	
	/*
	Algorithm:

	while there are more than 2 terms then
		Find an operator with largest precedence and make a new Expression from the 2 Terms surrounding it.
		Put that new Expression back into the list.

	assign the 2 terms to this node
	 */
	 
	while ((*termsc) > 2){
		struct Op* opWithLargestPrecedence = (*ops)[0];
		
		//last index into the operators 
		//(lowest precedence)
		int lowest = nops-1;	
		int indexOfFoundOp = (*opsc)-1;

		for(int i = 0;i < (*opsc);i++){
			struct Op* o1 = (*ops)[i];
			if(prec_index(o1->op) < lowest){
				lowest = prec_index(o1->op);
				opWithLargestPrecedence = o1;
				indexOfFoundOp = i;
			}
		}
		
		if(lowest > max_op_index){
			//not enough precedence,
			//is comparison operator or higher
			break;
		}

		const int indexOfOp = indexOfFoundOp;
		
		//index of left and right term
		const int leftTermIndex = indexOfOp;
		const int rightTermIndex = indexOfOp+1;

		struct UnOpTerm* leftTerm = (*terms)[leftTermIndex];
		struct UnOpTerm* rightTerm = (*terms)[rightTermIndex];

		struct Expr* expr = 
			makeExpr_3(
				leftTerm,
				opWithLargestPrecedence,
				rightTerm
			);
			
		if(expr == NULL){
			printf("idk\n");
			exit(1);
			return;
		}

		//simplify ------------------------------
		*terms = (struct UnOpTerm**)
		erase((void**)(*terms),leftTermIndex, *termsc);
		(*termsc)-=1;
		//---------------------------------------
		//because elements have shifted 1 to the left
		//and the terms were adjacent
		const int i2 = leftTermIndex;
		
		*terms = (struct UnOpTerm**)
		erase((void**)(*terms),i2, *termsc);
		(*termsc)-=1;
		//---------------------------------------
		
		*ops = (struct Op**)
		erase((void**)(*ops),indexOfOp, *opsc);
		(*opsc)-=1;
		//------------------------------------------

		//create term from our expression
		//(must create a term because it's a list of terms)
		struct UnOpTerm* ttmp = make(UnOpTerm);
		ttmp->op = NULL;
		ttmp->term = makeTerm_other(expr);
		
		if(ttmp == NULL){ 
			printf("idk\n");
			exit(1);
			return;
		}

		//insert newly created expression
		*terms = (struct UnOpTerm**)insert((void**)(*terms), indexOfOp, (void*)ttmp, *termsc);
		(*termsc)+=1; //because we inserted
	}
}

void insertOperatorChaining(
	struct Op*** ops, int* opsc,
	struct UnOpTerm*** terms, int* termsc,
	bool debug
){
	if(debug){ printf("insertOperatorChaining(...)\n"); }
	/*
	Algorithm:
	look for pattern:  OP1 TERM OP2
	where OP1 and OP2 are comparison operators
		if found:
			generate OP1 TERM && TERM OP2
			
	*/
	//pattern length is 3,
	//so i+1 should also be accessible
	//(for the second operator)
	for(int i=0;i < (*opsc)-1; i++){
		
		const uint16_t lOpIndex    = i;
		const uint16_t termIndex   = i+1;
		const uint16_t rOpIndex    = i+1;
		
		//compare to pattern
		
		struct Op* lOp    = (*ops)[lOpIndex];
		struct UnOpTerm* term = (*terms)[termIndex];
		struct Op* rOp    = (*ops)[rOpIndex];
		
		if(debug){
			printf("looking at %s %s %s\n", lOp->op, "EXPR", rOp->op);
		}
		
		if(isComparisonOp(lOp) && isComparisonOp(rOp)){
			//they are comparison operators
			
			if(debug){ 
				printf("chaining comparison operators\n"); 
			}
			
			//generate the pattern
			struct Op* andOp = make(Op);
			
			andOp->is_arithmetic = false;
			andOp->is_relational = false;
			andOp->is_logical    = true;
			andOp->is_bitwise    = false;
			
			strcpy(andOp->op, "&&");
			
			//insert &&
			(*ops) = (struct Op**)insert((void**)(*ops), lOpIndex+1, (void*)andOp, *opsc);
			(*opsc) += 1;
			
			//deep copy the term
			//so that later freeing the AST
			//causes no double free
			struct UnOpTerm* termCopy = copy_un_op_term(term);
			
			//insert EXPR
			(*terms) = (struct UnOpTerm**)insert((void**)(*terms), termIndex, (void*)termCopy, *termsc);
			(*termsc) += 1;
		}
	}
}

// ------------------- UTILITY SUBROUTINES -----------
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

