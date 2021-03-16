#include "ast/ast.h"

#include "tables/sst/sst.h"
#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

#include "ast/visitor/visitor.h"

#include "fn_analyzer.h"

static void myvisitor(void* node, enum NODE_TYPE type);

static struct ST* myst;
static struct SST* mysst;
static struct Method* currentFn = NULL;

void analyze_functions(struct ST* st, struct AST* ast){

	myst  = st;
	mysst = st->sst;
	
	visitAST(ast, myvisitor);
}

static void myvisitor(void* node, enum NODE_TYPE type){
	
	if(type == NODE_METHOD){ 
		currentFn = (struct Method*) node;
		
		struct SSTLine* myline = sst_get(mysst, currentFn->name);
		cc_set_calls_fn_ptrs(myline->cc, false);
	}
	
	if(type != NODE_METHODCALL){ return; }
	
	struct MethodCall* m = (struct MethodCall*) node;
	
	struct SSTLine* line;
	
	//calling a function pointer?
	//unfortunately, the LVST currently 
	//only gets created during code generation, 
	//and only for one method at a time.
	//so we have to abort if we cannot find the method
	
	//if(lvst_contains(myst->lvst, m->methodName))
	//	{ return; }
	
	if(!sst_contains(mysst, m->methodName)){
		//maybe it is a function ptr
		struct SSTLine* myline = sst_get(mysst, currentFn->name);
		cc_set_calls_fn_ptrs(myline->cc, true);
		return; 
	}
	
	line = sst_get(mysst, currentFn->name);
	cc_add_callee(line->cc, m->methodName);
	
	
	line = sst_get(mysst, m->methodName);
	cc_add_caller(line->cc, currentFn->name);
}
