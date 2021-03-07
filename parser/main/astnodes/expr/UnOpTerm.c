#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "UnOpTerm.h"

#include "Op.h"
#include "Term.h"

#include "ast/util/free_ast.h"

#include "token/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token.h"

struct UnOpTerm* makeUnOpTerm(struct TokenList* tokens, bool debug){
	
	if(debug){
		printf("UnOpTerm(...) from ");
		list_print(tokens);
	}
	
	if(list_size(tokens) < 1){ return NULL; }
	
	struct UnOpTerm* res = make(UnOpTerm);
	
	struct TokenList* copy = list_copy(tokens);
	
	res->op = makeOp(copy, debug);
	//res->op may be NULL, it is not a problem,
	//as the unary operator is optional
	
	if(res->op != NULL){
		
		//check if it is unary operator.
		//sometimes it inserts a space 
		//so check for that case also
		if(
			strcmp(res->op->op, "!") != 0 && 
			strcmp(res->op->op, "! ") != 0 && 
			strcmp(res->op->op, "~") != 0 &&
			strcmp(res->op->op, "~ ") != 0 &&
			strcmp(res->op->op, "-") != 0 &&
			strcmp(res->op->op, "- ") != 0
		){
			//the operator was not unary,
			//so we do not have an unary op term
			freeTokenListShallow(copy);
			free(res);
			return NULL;
		}
	}
	
	res->term = makeTerm(copy, debug);
	
	if(res->term == NULL){
		freeOp(res->op);
		freeTokenListShallow(copy);
		free(res);
		return NULL;
	}
	
	list_set(tokens, copy);
	
	freeTokenListShallow(copy);
	
	if(debug){
		printf("sucess parsing UnOpTerm\n");
	}
	
	return res;
}


