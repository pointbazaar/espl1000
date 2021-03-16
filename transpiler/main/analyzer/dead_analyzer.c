#include <stdio.h>
#include <stdlib.h>

#include "ast/ast.h"
#include "ast/visitor/visitor.h"

#include "tables/sst/sst.h"
#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

#include "dead_analyzer.h"

static struct SST* mysst;

static void mark_live(struct SST* sst, char* name);

static void set_all(struct SST* sst, bool is_dead, bool dead_visited);

/* BUG: when passing a function pointer
 * to another function, the function pointed to
 * is marked as dead, as there are no calls to it.
 * but it is not dead. we could assume that all 
 * functions which are referred to as function 
 * pointers could be live. so every simpleVar
 * in the program must be analyzed, if it is the name
 * of a function. that function may then be marked
 * as live, as it could be live.
 */
 
static void myvisitor_dead(void* node, enum NODE_TYPE type);

void analyze_dead_code(struct ST* st, struct AST* ast){
	
	struct SST* sst = st->sst;
	
	mysst = sst;
	
	//set all functions to live
	set_all(sst, false, true);
	
	if(!sst_contains(sst, "main")){ return; }
	
	set_all(sst, true, false);
	
	visitAST(ast, myvisitor_dead);
	
	mark_live(sst, "main");
}

static void mark_live(struct SST* sst, char* name){
	
	struct SSTLine* line = sst_get(sst, name);
	
	if(line->dead_known){ return; }
	
	line->dead_known   = true;
	line->is_dead      = false;
	
	struct CCNode* callee = line->cc->callees;
	
	while(callee != NULL){
		
		mark_live(sst, cc_name(callee));
		
		callee = cc_next(callee);
	}
}

static void set_all(struct SST* sst, bool is_dead, bool dead_visited){

	for(int i = 0; i < sst_size(sst); i++){
		
		struct SSTLine* line = sst_at(sst, i);
		
		line->is_dead      = is_dead;
		line->dead_known   = dead_visited;
	}
}

static void myvisitor_dead(void* node, enum NODE_TYPE type){
	
	/* if we are dealing with a variable that
	 * is a function pointer, the function
	 * pointed to is assumed to be live.
	 */
	
	if(type == NODE_SIMPLEVAR){ 
		
		struct SimpleVar* v = (struct SimpleVar*) node;
		
		if(sst_contains(mysst, v->name)){
			
			mark_live(mysst, v->name);
		}
	}
}
