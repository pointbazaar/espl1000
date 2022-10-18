#include <stdbool.h>
#include <stdio.h>

//AST Includes
#include "ast/ast.h"
#include "ast/visitor/visitor.h"

//Table Includes
#include "tables/cc/cc.h"
#include "tables/sst/sst.h"
#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

#include "halts.h"
#include "halt_analyzer.h"

static void myvisitor(void* node, enum NODE_TYPE type, void* arg);

static bool all_callees_terminate(struct SST* sst, struct CC* cc);

static bool terminates_by_construction(struct StmtBlock* block);

static bool inner(struct SST* sst);

void analyze_termination(struct ST* st){
	
	bool changed = true;
	
	while(changed){
		
		changed = inner(st->sst);
	}
}

static bool inner(struct SST* sst){
	
	bool changed = false;
	
	for(uint32_t i = 0; i < sst_size(sst); i++){
			
		struct SSTLine* line = sst_at(sst, i);
		
		if(line->halts != HALTS_UNKNOWN){ continue; }
		
		if(!all_callees_terminate(sst, line->cc))
			{ continue; }
		
		//all callees terminate
		
		if(line->method == NULL){
			printf("%s\n", line->name);
		}
		
		if(terminates_by_construction(line->method->block)){
		
			line->halts = HALTS_ALWAYS;
			changed = true;
			break;
		}
	}
	
	return changed;
}

static bool all_callees_terminate(struct SST* sst, struct CC* cc){
	
	struct CCNode* node = cc->callees;
	
	//at this point, halting information
	//is not contained in the type of a subroutine,
	//so it would be possible that the called
	//function does not halt.
	if(cc->calls_fn_ptrs)
		{ return false; }
	
	while(node != NULL){
		
		char* callee_name = cc_name(node);
		
		struct SSTLine* callee = sst_get(sst, callee_name);
		
		if(callee->halts != HALTS_ALWAYS)
			{ return false; }
	
		node = cc_next(node);
	}
	
	return true;
}

static bool terminates_by_construction(struct StmtBlock* block){
	
	//we can use a visitor to see if any
	//potentially non-terminating statements are used
	//such as: WhileStmt
	
	bool hasWhile = false;

	visit_stmt_block(block, myvisitor, &hasWhile);
	
	return !hasWhile;
}

static void myvisitor(void* node, enum NODE_TYPE type, void* arg){
	
	if(node == NULL){ };
	
	bool* hasWhile = (bool*) arg;
	
	if(type == NODE_WHILESTMT){ *hasWhile = true; }
}
