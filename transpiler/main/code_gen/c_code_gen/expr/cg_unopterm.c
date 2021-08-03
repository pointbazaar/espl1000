#include <stdio.h>

#include "ast/ast.h"

#include "util/ctx.h"

#include "../_cg.h"

void transpileUnOpTerm(struct UnOpTerm* t, struct Ctx* ctx){
		
	if(t->op != NULL){ transpileOp(t->op, ctx); }
	
	transpileTerm(t->term, ctx);
}
