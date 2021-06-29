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

//-------------------
static void annot_halts(struct SST* sst, struct Method* m);
static void annot_deprecated(struct SST* sst, struct Method* m);
static void annot_private(struct SST* sst, struct Call* call);
//-------------------

static struct Namespace* current_namespace = NULL;

void analyze_annotations(struct ST* st, struct AST* ast){
	
	struct SST* sst = st->sst;
	
	visitAST(ast, myvisitor_annotations, sst);
}

static void myvisitor_annotations(void* node, enum NODE_TYPE type, void* arg){
	 
	struct SST* mysst = (struct SST*) arg;
	
	if(type == NODE_NAMESPACE){
		current_namespace = (struct Namespace*) node;
	}
	
	if(type == NODE_METHOD){ 
		
		struct Method* m = (struct Method*) node;
		
		annot_halts(mysst, m);
		annot_deprecated(mysst, m);
	}
	
	if(type == NODE_CALL){
		annot_private(mysst, (struct Call*) node);
	}
}

static void annot_halts(struct SST* sst, struct Method* m){
	
	//figure out if it has @halts annotation
	if(has_annotation(m->super.annotations, ANNOT_HALTS)){
		
		struct SSTLine* line = sst_get(sst, m->name);
		
		if(line->halts != HALTS_ALWAYS){
			
			printf("[Analyzer][Annotations][Warning]");
			
			printf(" subroutine %s has @halts Annotation, but could not be proven to terminate\n", m->name);
		}
	}
}

static void annot_deprecated(struct SST* sst, struct Method* m){
	
	//figure out if it has @deprecated annotation
	if(has_annotation(m->super.annotations, ANNOT_DEPRECATED)){
		
		struct SSTLine* line = sst_get(sst, m->name);
		
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
}

static void annot_private(struct SST* sst, struct Call* call){
	
	//figure out if called method has @private annotation
	
	//this is only important if the method we are
	//calling is not passed to us as an argument,
	//at which point we cannot know which method 
	//it is.
	
	if(!sst_contains(sst, call->name)){ return; }
	
	struct SSTLine* line = sst_get(sst, call->name);
	
	if(line->is_private){
		
		//look if the call occurred in the same namespace
		//as the called method
		
		if(current_namespace == NULL){
			printf("Fatal\n"); exit(1);
		}
		
		char* call_ns = current_namespace->name;
		char* orig_ns = line->_namespace;
		
		if(strcmp(call_ns, orig_ns) != 0){
			
			//call occurred in different namespace
			
			printf("[Analyzer][Annotations][Warning]");
			printf(" subroutine %s is @private in namespace %s, but was called in namespace %s\n", call->name, orig_ns, call_ns);
		}
	}
}
