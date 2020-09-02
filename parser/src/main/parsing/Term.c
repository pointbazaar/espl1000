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
#include "../../../../ast/free_ast.h"

// --- private subroutines ---

struct Term* initTerm();
void tryInitExpr(struct Term* res, struct TokenList* copy, bool debug);
void tryInitStringConst(struct Term* res, struct TokenList* copy, bool debug);
// ---------------------------

struct Term* makeTerm_other(struct Expr* expr){
	
	struct Term* res = initTerm();

	res->m5 = expr;
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
		
		if( (res->m2 = makeIntConst(copy,debug)) == NULL){
			if( (res->m7 = makeFloatConst(copy,debug)) == NULL){
				if( (res->m4 = makeMethodCall(copy,debug)) == NULL){
					if( (res->m1 = makeBoolConst(copy,debug)) == NULL){
						if( (res->m6 = makeVariable(copy,debug)) == NULL){
							if( (res->m3 = makeCharConst(copy,debug)) == NULL){
								
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
	
	struct Term* res = smalloc(sizeof(struct Term));
	
	res->m1 = NULL;
	res->m2 = NULL;
	res->m3 = NULL;
	res->m4 = NULL;
	res->m5 = NULL;
	res->m6 = NULL;
	res->m7 = NULL;
	res->m8 = NULL;
	
	return res;
}

void tryInitExpr(struct Term* res, struct TokenList* copy, bool debug){
	
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
}

void tryInitStringConst(struct Term* res, struct TokenList* copy, bool debug){
	
	res->m8 = makeStringConst(copy, debug);
	if(res->m8 == NULL){
		
		printf("expected an String, but got :");
		list_print(copy);
		
		free(res);
		freeTokenListShallow(copy);
		
		exit(1);
	}
}
