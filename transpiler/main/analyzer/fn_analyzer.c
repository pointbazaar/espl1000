#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast/ast.h"

#include "tables/sst.h"
#include "tables/stst.h"
#include "tables/lvst.h"
#include "tables/symtable.h"

#include "visitor/visitor.h"

#include "fn_analyzer.h"

static void analyze_callees(struct ST* st, struct AST* ast);
static void analyze_callers(struct ST* st, struct AST* ast);

void myvisitor_callees(void* node, enum NODE_TYPE type);

void analyze_functions(struct ST* st, struct AST* ast){

	analyze_callees(st, ast);
	analyze_callers(st, ast);
}

static void analyze_callees(struct ST* st, struct AST* ast){

	//DEBUG
	printf("analyze_callees\n");
	
	visitAST(ast, myvisitor_callees);
}

static void analyze_callers(struct ST* st, struct AST* ast){
	
	//TODO
	//DEBUG
	printf("analyze_callers\n");
}

//---------------------------------------------------------

static struct Method* currentFn = NULL;

void myvisitor_callees(void* node, enum NODE_TYPE type){
	
	if(type == NODE_METHOD){ 
		currentFn = (struct Method*) node;
	}
	
	//calls occur in struct MethodCall
	if(type == NODE_METHODCALL){ return; }
	
	//struct MethodCall* m = (struct MethodCall*) node;
	
	//TODO: extract the callee
	
}
