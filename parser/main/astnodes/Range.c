#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "Range.h"
#include "expr/Expr.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct Range* makeRange(struct TokenList* tokens) {
	
	if(list_size(tokens) == 0){ return NULL; }

	struct TokenList* copy = list_copy(tokens);

	struct Range* res = make(Range);
	
	res->super.line_num    = list_head(copy)->line_num;
	res->super.annotations = 0;

	res->start = makeExpr(copy);
	
	if(res->start == NULL){
		freeTokenListShallow(copy);
		free(res);
		return NULL;
	}
	
	// expect ".."
	
	if(!list_expect(copy, RANGEOP)){
		freeTokenListShallow(copy);
		free_expr(res->start);
		free(res);
	}
	
	res->end = makeExpr(copy);
	
	if(res->end == NULL){
		freeTokenListShallow(copy);
		free_expr(res->start);
		free(res);
		return NULL;
	}

	list_set(tokens,copy);
	freeTokenListShallow(copy);

	return res;
}
