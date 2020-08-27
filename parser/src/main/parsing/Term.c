#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../commandline/TokenList.h"
#include "../commandline/Token.h"
#include "../commandline/TokenKeys.h"
#include "Term.h"
#include "CharConst.h"
#include "IntConst.h"
#include "BoolConst.h"
#include "FloatConst.h"
#include "StringConst.h"
#include "Expr.h"
#include "Variable.h"
#include "statements/MethodCall.h"
#include "../../../../util/util.h"

struct Term* makeTerm_other(struct Expr* expr){
	struct Term* res = smalloc(sizeof(struct Term));

	res->m1 = NULL;
	res->m2 = NULL;
	res->m3 = NULL;
	res->m4 = NULL;
	res->m5 = NULL;
	res->m6 = NULL;
	res->m7 = NULL;
	res->m8 = NULL;

	res->m5 = expr;
	return res;
}

struct Term* makeTerm(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("Term(...) from ");
		list_print(tokens);
	}
	
	if(list_size(tokens) == 0){return NULL;}

	struct Term* res = smalloc(sizeof(struct Term));
	
	res->m1 = NULL;
	res->m2 = NULL;
	res->m3 = NULL;
	res->m4 = NULL;
	res->m5 = NULL;
	res->m6 = NULL;
	res->m7 = NULL;
	res->m8 = NULL;

	struct TokenList* copy = list_copy(tokens);

	if(list_head(copy)->kind == LPARENS){
		
		if(debug){
			printf("try to parse Expr in Term\n");
		}
		
		list_consume(copy, 1);

		res->m5 = makeExpr(copy,debug);
		if(res->m5 == NULL){
			free(res);
			freeTokenListShallow(copy);
			printf("expected an Expression, but got :");
			list_print(copy);
			exit(1);
		}
		
		if(!list_expect(copy, RPARENS)){
			//this part can be parsed deterministically
			printf("expected ')', but was: ");
			list_print(copy);
			exit(1);
		}
	}else if(list_head(copy)->kind == STRINGCONST){
		
		res->m8 = makeStringConst(copy, debug);
		if(res->m8 == NULL){
			free(res);
			freeTokenListShallow(copy);
			return NULL;
		}
		
	}else{

		if(debug){
			printf("try to parse ordinary Term\n");
		}
		
		   res->m2 = makeIntConst(copy,debug);
		if(res->m2 == NULL){
			
			   res->m7 = makeFloatConst(copy,debug);
			if(res->m7 == NULL){
				
				   res->m4 = makeMethodCall(copy,debug);
				if(res->m4 == NULL){
					
					   res->m1 = makeBoolConst(copy,debug);
					if(res->m1 == NULL){
						
						   res->m6 = makeVariable(copy,debug);
						if(res->m6 == NULL){
							
							   res->m3 = makeCharConst(copy,debug);
							if(res->m3 == NULL){
								
								free(res);
								freeTokenListShallow(copy);
								return NULL;
							}
						}
					}
				}
			}
		}
	}
	
	if(debug){
		printf("sucess parsing Term\n");
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}

void freeTerm(struct Term* t){
	
	if(t->m1 != NULL){
		freeBoolConst(t->m1);
	}else if(t->m2 != NULL){
		freeIntConst(t->m2);
	}else if(t->m3 != NULL){
		freeCharConst(t->m3);
	}else if(t->m4 != NULL){
		freeMethodCall(t->m4);
	}else if(t->m5 != NULL){
		freeExpr(t->m5);
	}else if(t->m6 != NULL){
		freeVariable(t->m6);
	}else if(t->m7 != NULL){
		freeFloatConst(t->m7);
	}else if(t->m8 != NULL){
		freeStringConst(t->m8);
	}else{
		printf("Error in freeTerm(...)\n");
		exit(1);
	}
	free(t);
}

