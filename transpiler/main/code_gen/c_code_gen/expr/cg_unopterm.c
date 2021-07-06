#include <stdio.h>
#include <assert.h>

#include "ast/ast.h"

#include "util/ctx.h"

#include "cg_unopterm.h"
#include "cg_op.h"
#include "cg_term.h"

void transpileUnOpTerm(struct UnOpTerm* t, struct Ctx* ctx){
		
	if(t->op != NULL){ transpileOp(t->op, ctx); }
	
	transpileTerm(t->term, ctx);
}
