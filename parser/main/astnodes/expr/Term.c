#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Term.h"
#include "const/CharConst.h"
#include "const/IntConst.h"
#include "const/HexConst.h"
#include "const/BinConst.h"
#include "const/BoolConst.h"
#include "const/FloatConst.h"
#include "const/StringConst.h"
#include "Expr.h"
#include "var/Variable.h"
#include "statements/Call.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

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
	
	const int tk_kind = list_head(copy)->kind;

	if(tk_kind == LPARENS){
		
		tryInitExpr(res, copy, debug);
		
	}else if(tk_kind == STRINGCONST){
		
		tryInitStringConst(res, copy, debug);
		
	}else if(tk_kind == HEXCONST){
		
		res->ptr.m9 = makeHexConst(copy, debug);
		res->kind = 9;
		
	}else if(tk_kind == BINCONST){
		
		res->ptr.m10 = makeBinConst(copy, debug);
		res->kind = 10;
		
	}else if(tk_kind == CCONST){
		
		res->ptr.m3 = makeCharConst(copy,debug);
		res->kind = 3;
		
	}else if(tk_kind == BCONST_TRUE || tk_kind == BCONST_FALSE ){
		
		res->ptr.m1 = makeBoolConst(copy, debug);
		res->kind = 1;
		
	}else{
		goto other_term;
	}
	
	//something matched
	goto end;
	
other_term:

	if( (res->ptr.m2 = makeIntConst(copy,debug)) != NULL){
		res->kind = 2;
		goto end;
	}

	if( (res->ptr.m7 = makeFloatConst(copy,debug)) != NULL){
		res->kind = 7;
		goto end;
	}
	
	if( (res->ptr.m4 = makeCall(copy,debug)) != NULL){				
		res->kind = 4;
		goto end;
	}
	
	if( (res->ptr.m6 = makeVariable(copy,debug)) != NULL){
		res->kind = 6;
		goto end;
	}
	
		
	free(res);
	freeTokenListShallow(copy);
	return NULL;		

	
end:
	
	if(debug){
		printf("sucess parsing Term\n");
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}

struct Term* initTerm(){
	
	struct Term* res = make(Term);
	
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
