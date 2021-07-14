#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "analyzer/lv/lv_analyzer.h"
#include "annotation_analyzer.h"
#include "analyzer/halts/halts.h"

#include "ast/ast.h"
#include "ast/visitor/visitor.h"

#include "tables/symtable/symtable.h"
#include "tables/sst/sst.h"
#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"
#include "tables/cc/cc.h"

#include "token/TokenKeys.h"

static void myvisitor_annotations(void* node, enum NODE_TYPE type, void* arg);

//-------------------
static void annot_halts(struct SST* sst, struct Method* m);
static void annot_deprecated(struct SST* sst, struct Method* m);
static void annot_private(struct SST* sst, struct Call* call);
static void annot_private_struct(struct STST* stst, struct StructType* t);
static void annot_private_struct_member(struct ST* st, struct Variable* var);
//-------------------

#define COLOR_ORANGE "\033[33m"

static void setcolor(char* color){ printf("%s", color); }
static void resetcolor(){ printf("\033[39m"); }
static void print_analyzer_warning(){
	printf("[Analyzer][Annotations]");
	setcolor(COLOR_ORANGE);
	printf("[Warning]");
	resetcolor();
}

static struct Namespace* current_namespace = NULL;
static struct Method*    current_subr      = NULL;

//------------------------------------------------------

void analyze_annotations(struct ST* st, struct AST* ast){

	visit_ast(ast, myvisitor_annotations, st);
}

static void myvisitor_annotations(void* node, enum NODE_TYPE type, void* arg){
	 
	struct ST* st = (struct ST*) arg;
	struct SST* mysst = st->sst;
	
	if(type == NODE_NAMESPACE){
		current_namespace = (struct Namespace*) node;
	}
	
	if(type == NODE_METHOD){ 
		
		struct Method* m = (struct Method*) node;
		current_subr = m;
		
		annot_halts(mysst, m);
		annot_deprecated(mysst, m);
	}
	
	if(type == NODE_STRUCTTYPE){
		
		annot_private_struct(st->stst, (struct StructType*) node);
	}
	
	if(type == NODE_CALL){
		annot_private(mysst, (struct Call*) node);
	}
	
	if(type == NODE_VARIABLE){
		annot_private_struct_member(st, (struct Variable*)node);
	}
}

static void annot_private_struct(struct STST* stst, struct StructType* t){
	
	char* tname = t->type_name;
	
	struct STSTLine* line = stst_get(stst, tname);
	
	if(line->is_private){
		
		if(strcmp(current_namespace->name, line->_namespace) != 0){
			
			//struct type is referenced outside of it's own
			//namespace
			
			print_analyzer_warning();
			
			printf(" struct %s has @private Annotation in %s, but was referenced in %s\n", tname, line->_namespace, current_namespace->name);
		}
	}
}

static void annot_private_struct_member(struct ST* st, struct Variable* var){
	
	struct STST* stst = st->stst;
	struct LVST* lvst = st->lvst;
	
	//check @private struct member access
	if(var->member_access == NULL){ return; }
		
	struct Variable* member = var->member_access;
	
	lvst_clear(st->lvst);
	lvst_fill(current_subr, st);
	
	//is it a local variable or are we already
	//down the member access chain? 
	//(would cause failure to find in lvst)
	if(!lvst_contains(lvst, var->simple_var->name)){ return; }
	
	//get the type of struct being accessed
	struct LVSTLine* line = lvst_get(lvst, var->simple_var->name);
	
	if(line->type->m1 == NULL){ return; }
	struct BasicType* btw = line->type->m1;
	if(btw->simple_type == NULL){ return; }
	if(btw->simple_type->struct_type == NULL){ return; }
	
	struct StructType* stype = btw->simple_type->struct_type;
	
	char* struct_name = stype->type_name;
	char* member_name = member->simple_var->name;
	
	//get info about the struct member
	struct STSTLine* line2 = stst_get(stst, struct_name);
	
	struct StructMember* memberDecl = stst_get_member(stst, struct_name, member_name);
	
	if(has_annotation(memberDecl->super.annotations, ANNOT_PRIVATE)){
		
		if(strcmp(line2->_namespace, current_namespace->name) == 0){ return; }
		
		print_analyzer_warning();			
		printf(
			" member %s of struct %s has @private Annotation in %s, ",
			member_name, 
			struct_name, 
			line2->_namespace
		);
		printf("but was referenced in %s\n",  current_namespace->name);
	}
}

static void annot_halts(struct SST* sst, struct Method* m){
	
	//figure out if it has @halts annotation
	if(has_annotation(m->super.annotations, ANNOT_HALTS)){
		
		struct SSTLine* line = sst_get(sst, m->name);
		
		if(line->halts != HALTS_ALWAYS){
			
			print_analyzer_warning();
			
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
			
			print_analyzer_warning();
			
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
			print_analyzer_warning();
			
			printf(" subroutine %s is @private in namespace %s, but was called in namespace %s\n", call->name, orig_ns, call_ns);
		}
	}
}
