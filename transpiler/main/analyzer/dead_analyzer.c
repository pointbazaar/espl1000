#include <stdio.h>
#include <stdlib.h>

#include "ast/ast.h"
#include "ast/visitor/visitor.h"

#include "tables/sst/sst.h"
#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

#include "dead.h"
#include "dead_analyzer.h"

static void mark_live(struct SST* sst, char* name);

static void set_all(struct SST* sst, enum DEAD dead);

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
 
static void myvisitor_dead(void* node, enum NODE_TYPE type, void* arg);

void analyze_dead_code(struct ST* st, struct AST* ast){
	
	struct SST* sst = st->sst;
	
	//set all functions to live
	set_all(sst, DEAD_ISLIVE);
	
	if(!sst_contains(sst, "main")){ return; }
	
	set_all(sst, DEAD_UNKNOWN);
	
	visitAST(ast, myvisitor_dead, sst);
	
	mark_live(sst, "main");
}

static void mark_live(struct SST* sst, char* name){
	
	struct SSTLine* line = sst_get(sst, name);
	
	if(line->dead != DEAD_UNKNOWN){ return; }
	
	line->dead = DEAD_ISLIVE;
	
	struct CCNode* callee = line->cc->callees;
	
	while(callee != NULL){
		
		mark_live(sst, cc_name(callee));
		
		callee = cc_next(callee);
	}
}

static void set_all(struct SST* sst, enum DEAD dead){

	for(uint32_t i = 0; i < sst_size(sst); i++){
		
		struct SSTLine* line = sst_at(sst, i);
		
		line->dead = dead;
	}
}

static void myvisitor_dead(void* node, enum NODE_TYPE type, void* arg){
	
	/* if we are dealing with a variable that
	 * is a function pointer, the function
	 * pointed to is assumed to be live.
	 */
	 
	struct SST* mysst = (struct SST*) arg;
	
	if(type == NODE_SIMPLEVAR){ 
		
		struct SimpleVar* v = (struct SimpleVar*) node;
		
		if(sst_contains(mysst, v->name)){
			
			mark_live(mysst, v->name);
		}
	}
}
