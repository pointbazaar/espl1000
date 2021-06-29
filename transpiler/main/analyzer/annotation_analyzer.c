#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "ast/ast.h"
#include "ast/visitor/visitor.h"

#include "tables/symtable/symtable.h"
#include "tables/sst/sst.h"

#include "analyzer/annotation_analyzer.h"
#include "analyzer/halts.h"

#include "token/TokenKeys.h"

static void myvisitor_annotations(void* node, enum NODE_TYPE type, void* arg);

static bool has_annotation(struct ASTNode* node, uint32_t annotation);

void analyze_annotations(struct ST* st, struct AST* ast){
	
	struct SST* sst = st->sst;
	
	visitAST(ast, myvisitor_annotations, sst);
}

static void myvisitor_annotations(void* node, enum NODE_TYPE type, void* arg){
	 
	struct SST* mysst = (struct SST*) arg;
	
	if(type == NODE_METHOD){ 
		
		struct Method* m = (struct Method*) node;
		
		//figure out if it has @halts annotation
		if(has_annotation(&(m->super), ANNOT_HALTS)){
			
			struct SSTLine* line = sst_get(mysst, m->name);
			
			if(line->halts != HALTS_ALWAYS){
				
				printf("[Analyzer][Annotations][Warning]");
				
				printf(" subroutine %s has @halts Annotation, but could not be proven to terminate\n", m->name);
			}
		}
		
		//figure out if it has @deprecated annotation
		if(has_annotation(&(m->super), ANNOT_DEPRECATED)){
			
			struct SSTLine* line = sst_get(mysst, m->name);
			
			//functions analyzer should have run before
			if(cc_size(line->cc->callers) > 0){
				
				//function still has callers
				
				printf("[Analyzer][Annotations][Warning]");
				printf(" subroutine %s is marked as @deprecated, but still has callers:\n", m->name);
				
				struct CCNode* caller = line->cc->callers;
				
				while(caller != NULL){
					
					char* caller_name = cc_name(caller);
					
					printf(" - called from: %s \n", caller_name);
					
					caller = cc_next(caller);
				}
			}
		}
		
		
		//TODO: @private
		//TODO: @public
	}
}

static bool has_annotation(struct ASTNode* node, uint32_t annotation){
	
	uint32_t which = (annotation - _ANNOT_START_);
	return (node->annotations & ( 1 << which )) != 0;
}
