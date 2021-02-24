#include <stdio.h>
#include <stdlib.h>

#include "AssignStmt.h"
#include "../Expr.h"
#include "../Variable.h"
#include "../typenodes/Type.h"
#include "../../commandline/TokenList.h"
#include "../../commandline/TokenKeys.h"
#include "../../../../token/token.h"
#include "../../../../ast/free_ast.h"

struct AssignStmt* makeAssignStmt(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("AssignStmt(...) from: ");
		list_print(tokens);
	}

	struct AssignStmt* res = malloc(sizeof(struct AssignStmt));
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

	struct Token* tkn_assign = list_head(copy);
	if(tkn_assign->kind != ASSIGNOP){ 
		freeVariable(res->var);
		free(res);
		freeTokenListShallow(copy);
		return NULL;
	}
	list_consume(copy, 1);
	
	//save the assignment operator
	strncpy(res->assign_op, tkn_assign->value_ptr, 2);

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


