#include <stdio.h>
#include <stdlib.h>

#include "parser/main/util/parse_astnode.h"

#include "IfStmt.h"
#include "Stmt.h"
#include "expr/Expr.h"
#include "../StmtBlock.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct IfStmt* initIfStmt();
void freeIncomplete(struct IfStmt* ifstmt);
//--------------------------

struct IfStmt* makeIfStmt(struct TokenList* tokens, bool debug) {

	if(list_size(tokens) < 3){ return NULL; }

	struct TokenList* copy = list_copy(tokens);

	struct IfStmt* res = initIfStmt();
	parse_astnode(copy, &(res->super));
	
	if(!list_expect(copy, IF)){
		return NULL;
	}
	
	res->super.line_num    = list_head(tokens)->line_num;
	res->super.annotations = 0;

	if((res->condition = makeExpr(copy,debug)) == NULL){
		freeIncomplete(res);
		return NULL;
	}

	if((res->block = makeStmtBlock(copy, debug)) == NULL){
		freeIncomplete(res);
		return NULL;
	}

	if (list_expect(copy, ELSE)) {

		if((res->else_block = makeStmtBlock(copy, debug)) == NULL){
			freeIncomplete(res);
			return NULL;
		}
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}

struct IfStmt* initIfStmt(){
	
	struct IfStmt* res = make(IfStmt);
	
	res->condition = NULL;
	res->block     = NULL;
	res->else_block = NULL;
	
	return res;
}

void freeIncomplete(struct IfStmt* is){
	//free an IfStmt, even if it has not been
	//completely initialized
	if(is->condition != NULL){
		freeExpr(is->condition);
	}
	if(is->block != NULL){
		freeStmtBlock(is->block);
	}
	if(is->else_block != NULL){
		freeStmtBlock(is->else_block);
	}
	free(is);
}
