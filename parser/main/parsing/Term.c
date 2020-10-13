#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../commandline/TokenList.h"
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
#include "../../../token/token.h"
#include "../../../ast/free_ast.h"

// --- private subroutines ---

struct Term* initTerm();
void tryInitExpr(struct Term* res, struct TokenList* copy, bool debug);
void tryInitStringConst(struct Term* res, struct TokenList* copy, bool debug);
// ---------------------------

struct Term* makeTerm_other(struct Expr* expr){
	
	struct Term* res = initTerm();

	res->kind = 5;
	res->ptr.m5 = expr;
	return res;
}

struct Term* makeTerm(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("Term(...) from ");
		list_print(tokens);
	}
	
	if(list_size(tokens) == 0){return NULL;}

	struct Term* res = initTerm();

	struct TokenList* copy = list_copy(tokens);

	if(list_head(copy)->kind == LPARENS){
		
		tryInitExpr(res, copy, debug);
		
	}else if(list_head(copy)->kind == STRINGCONST){
		
		tryInitStringConst(res, copy, debug);
		
	}else{
		
		res->kind = 2;
		if( (res->ptr.m2 = makeIntConst(copy,debug)) == NULL){
			res->kind = 7;
			if( (res->ptr.m7 = makeFloatConst(copy,debug)) == NULL){
				res->kind = 4;
				if( (res->ptr.m4 = makeMethodCall(copy,debug)) == NULL){
					res->kind = 1;
					if( (res->ptr.m1 = makeBoolConst(copy,debug)) == NULL){
						res->kind = 6;
						if( (res->ptr.m6 = makeVariable(copy,debug)) == NULL){
							res->kind = 3;
							if( (res->ptr.m3 = makeCharConst(copy,debug)) == NULL){
								
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

struct Term* initTerm(){
	
	struct Term* res = malloc(sizeof(struct Term));
	
	res->kind = 0;
	res->ptr.m1 = NULL;
	
	return res;
}

void tryInitExpr(struct Term* res, struct TokenList* copy, bool debug){
	
	list_consume(copy, 1);

	res->kind = 5;
	res->ptr.m5 = makeExpr(copy,debug);
	if(res->ptr.m5 == NULL){
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
}

void tryInitStringConst(struct Term* res, struct TokenList* copy, bool debug){
	
	res->kind = 8;
	res->ptr.m8 = makeStringConst(copy, debug);
	if(res->ptr.m8 == NULL){
		
		printf("expected an String, but got :");
		list_print(copy);
		
		free(res);
		freeTokenListShallow(copy);
		
		exit(1);
	}
}
