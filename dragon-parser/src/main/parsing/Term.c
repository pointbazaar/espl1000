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
#include "Expr.h"
#include "Variable.h"
#include "statements/MethodCall.h"

struct Term* makeTerm_other(struct Expr* expr){
	struct Term* res = malloc(sizeof(struct Term));

	res->m1 = NULL;
	res->m2 = NULL;
	res->m3 = NULL;
	res->m4 = NULL;
	res->m5 = NULL;
	res->m6 = NULL;
	res->m7 = NULL;

	res->m5 = expr;
	return res;
}

struct Term* makeTerm(struct TokenList* tokens, bool debug) {

	/*
	Term := IntConst | CharConst | FloatConst | MethodCall | BoolConst | Variable | '(' Expr ')'
	*/

	if(debug){
		printf("Term(...) from %s\n", list_code(tokens, debug));
	}

	struct Term* res = malloc(sizeof(struct Term));
	
	res->m1 = NULL;
	res->m2 = NULL;
	res->m3 = NULL;
	res->m4 = NULL;
	res->m5 = NULL;
	res->m6 = NULL;
	res->m7 = NULL;

	struct TokenList* copy = list_copy(tokens);

	if(list_head(copy)->kind == LPARENS){
		
		if(debug){
			printf("try to parse Expr in Term\n");
		}
		
		list_consume(copy, 1);

		res->m5 = makeExpr(copy,debug);
		if(res->m5 == NULL){return NULL;}
		
		if(!list_expect(copy, RPARENS)){
			//this part can be parsed deterministically
			printf("expected ')', but was: %s\n", list_code(copy, debug));
			exit(1);
			return NULL;
		}

	}else{

		if(debug){
			printf("try to parse ordinary Term\n");
		}

		bool fail = false;
		
		res->m2 = makeIntConst(copy,debug);
		if(res->m2 == NULL){fail = true;}

		if(fail) {
			fail = false;
		
			//a string constant is syntatic sugar.
			//in the parsing stage it is converted to an array of char constants
			//inline the stringConstant and its syntatic sugar
			//struct Token* token = list_get(tokens, 0);
			
			//"Error: could not read stringConstant syntatic sugar";
			fail = true;
				
			if(fail){
				fail=false;
				res->m7 = makeFloatConst(copy,debug);
				if(res->m7 == NULL){fail = true;}

				if(fail){
					fail=false;
					res->m4 = makeMethodCall(copy,debug);
					if(res->m4 == NULL){fail = true;}

					if(fail && debug){
						printf("parsing MethodCall failed\n");
					}

					if(fail) {
						fail=false;
						res->m1 = makeBoolConst(copy,debug);
						if(res->m1 == NULL){fail = true;}

						if(fail) {
							fail = false;
							res->m6 = makeVariable(copy,debug);
							if(res->m6 == NULL){fail=true;}

							if(fail) {
								res->m3 = makeCharConst(copy,debug);
								if(res->m3 == NULL){return NULL;}
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
	}
	free(t);
}

