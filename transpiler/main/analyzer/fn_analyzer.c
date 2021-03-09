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

void myvisitor_callees(void* node, char* name);

void analyze_functions(struct ST* st, struct AST* ast){

	analyze_callees(st, ast);
	analyze_callers(st, ast);
}

static void analyze_callees(struct ST* st, struct AST* ast){

	//DEBUG
	printf("analyze_callees\n");
	
	visit_ast(ast, myvisitor_callees);
}

static void analyze_callers(struct ST* st, struct AST* ast){
	
	//TODO
	//DEBUG
	printf("analyze_callers\n");
}

//---------------------------------------------------------

static struct Method* currentFn = NULL;

void myvisitor_callees(void* node, char* name){
	
	if(strcmp(name, "Method") == 0){ 
		currentFn = (struct Method*) node;
	}
	
	//calls occur in struct MethodCall
	if(strcmp(name, "MethodCall") != 0){ return; }
	
	//struct MethodCall* m = (struct MethodCall*) node;
	
	//TODO: extract the callee
	
}
