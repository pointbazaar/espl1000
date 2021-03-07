#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "Range.h"
#include "expr/Expr.h"

#include "ast/util/free_ast.h"

#include "token/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token.h"

struct Range* makeRange(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("Range(...) from ");
		list_print(tokens);
	}
	
	if(list_size(tokens) == 0){ return NULL; }

	struct TokenList* copy = list_copy(tokens);

	struct Range* res = make(Range);

	res->start = makeExpr(copy, debug);
	
	if(res->start == NULL){
		freeTokenListShallow(copy);
		free(res);
		return NULL;
	}
	
	// expect ".."
	
	if(!list_expect(copy, RANGEOP)){
		freeTokenListShallow(copy);
		freeExpr(res->start);
		free(res);
	}
	
	res->end = makeExpr(copy, debug);
	
	if(res->end == NULL){
		freeTokenListShallow(copy);
		freeExpr(res->start);
		free(res);
		return NULL;
	}

	list_set(tokens,copy);
	freeTokenListShallow(copy);

	return res;
}
