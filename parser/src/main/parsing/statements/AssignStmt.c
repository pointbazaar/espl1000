#include <stdio.h>
#include <stdlib.h>

#include "AssignStmt.h"
#include "../../commandline/TokenList.h"
#include "../../../../../token/token.h"
#include "../../commandline/TokenKeys.h"
#include "../typenodes/Type.h"
#include "../Variable.h"
#include "../Expr.h"
#include "../../../../../util/util.h"
#include "../../../../../ast/free_ast.h"

struct AssignStmt* makeAssignStmt(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("AssignStmt(...) from: ");
		list_print(tokens);
	}

	struct AssignStmt* res = smalloc(sizeof(struct AssignStmt));

	res->optType = NULL;

	struct TokenList* copy = list_copy(tokens);

	struct TokenList* copy2 = list_copy(copy);
	res->optType = makeType2(copy2,debug);
	if(res->optType != NULL){
		list_set(copy, copy2);
	}
	freeTokenListShallow(copy2);

	if(debug){
		printf("%s\n", list_code(copy, debug));
		printf("DEBUG\n");
	}

	res->var = makeVariable(copy,debug);
	if(res->var == NULL){
		free(res);
		freeTokenListShallow(copy);
		return NULL;
	}

	struct Token* tkeq = makeToken2(EQ,"=");
	if(!list_expect_2(copy, tkeq)){ 
		freeToken(tkeq);
		freeVariable(res->var);
		free(res);
		freeTokenListShallow(copy);
		return NULL;
	}
	freeToken(tkeq);

	res->expr = makeExpr(copy,debug);
	if(res->expr == NULL){
		freeVariable(res->var);
		free(res);
		freeTokenListShallow(copy);
		return NULL;
	}

	if(!list_expect(copy, SEMICOLON)){
		freeExpr(res->expr);
		freeVariable(res->var);
		free(res);
		freeTokenListShallow(copy);
		return NULL;
	}
	
	if(debug){
		printf("sucess parsing AssignStmt\n");
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}


