#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast/ast.h"

#include "tables/sst/sst.h"
#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

#include "ast/visitor/visitor.h"

#include "fn_analyzer.h"

void myvisitor(void* node, enum NODE_TYPE type);

static struct ST* myst;

void analyze_functions(struct ST* st, struct AST* ast){

	myst = st;

	printf("analyze_callees and callers\n");

	visitAST(ast, myvisitor);
}

//---------------------------------------------------------

static struct Method* currentFn = NULL;

void myvisitor(void* node, enum NODE_TYPE type){
	
	if(type == NODE_METHOD){ 
		currentFn = (struct Method*) node;
	}
	
	if(type != NODE_METHODCALL){ return; }
	
	struct MethodCall* m = (struct MethodCall*) node;
	
	struct SSTLine* line;
	
	line = sst_get(myst->sst, currentFn->name);
	cc_add_callee(line->cc, m->methodName);
	
	
	line = sst_get(myst->sst, m->methodName);
	cc_add_caller(line->cc, currentFn->name);
}
