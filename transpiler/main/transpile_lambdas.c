#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast/ast.h"
#include "ast/util/copy_ast.h"
#include "ast/util/free_ast.h"
#include "ast/visitor/visitor.h"

#include "tables/sst/sst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

#include "transpile_lambdas.h"

#define LAMBDA_NAME_PATTERN "lambda_%d"

static void myvisitor_transpile_lambdas(void* node, enum NODE_TYPE type, void* arg);

static void transpile_lambda(
	struct Namespace* ns, 
	struct Term* term,
	struct Lambda* lambda,
	struct Type* orig_rtype,
	struct Type** orig_arg,
	uint8_t orig_count_arg
);

static struct Method* gen_subr_from_lambda(
	struct Lambda* lambda,
	struct Type* orig_rtype,
	struct Type** orig_arg,
	uint8_t orig_count_arg
);

static void myvisitor_transpile_lambdas_inner(
	struct Call* call, 
	struct Lambda* lambda, 
	struct ST* st,
	struct Term* term,
	uint8_t i
);

void transpileLambdas(struct AST* ast, struct ST* st){
	
	//we need a visitor to obtain lambda locations
	//in the appropriate contexts (AST Node Patters)
	
	visitAST(ast, myvisitor_transpile_lambdas, st);
}

static struct Namespace* current_ns = NULL;

static void myvisitor_transpile_lambdas(void* node, enum NODE_TYPE type, void* arg){

	//look at the context of the lambda
	// - (1) subr call param
	// - (2) assignment to variable
	
	//in case (1): find out the subroutine type signature
	//and insert the lambda subroutine at toplevel
	//in the Namespace
	struct ST* st = (struct ST*) arg;
	
	if(type == NODE_NAMESPACE){
		
		current_ns = node;
		
	} else if(type == NODE_CALL){
		
		struct Call* call = (struct Call*) node;
		
		for(uint8_t i = 0; i < call->count_args; i++){
			
			struct Expr* expr = call->args[i];
			struct Term* term = expr->term1->term;
			
			if(term->kind == 11){
				
				//we have a lambda ^^
				struct Lambda* lambda = term->ptr.m11;
				
				myvisitor_transpile_lambdas_inner(call, lambda, st, term, i);
			}
		}
	}
	
	//in case (2): ignore, deal with later (TODO)
}

static void myvisitor_transpile_lambdas_inner(
	struct Call* call, 
	struct Lambda* lambda, 
	struct ST* st,
	struct Term* term,
	uint8_t i
){
	
	struct SubrType* lambdaType = NULL;
	
	if(lvst_contains(st->lvst, call->name)){
		
		struct LVSTLine* line2 = lvst_get(st->lvst, call->name);
		
		struct Type* type = line2->type;
		
		if(type->m1 == NULL){ 
			printf("Error\n");
			exit(1);
		}
		if(type->m1->subr_type == NULL){
			printf("Error\n");
			exit(1);
		}
		
		struct SubrType* stype = type->m1->subr_type;
		
		struct Type* pre_lambdaType = stype->arg_types[i];
		
		lambdaType = pre_lambdaType->m1->subr_type;
		
	}else if(sst_contains(st->sst, call->name)){
		
		struct SSTLine* line = sst_get(st->sst, call->name);
	
		struct Method* container = line->method;
		
		struct DeclArg* lambdaArg = container->args[i];
		
		//get the subrtype from the argument
		struct Type* pre_lambdaType = lambdaArg->type;
		
		lambdaType = pre_lambdaType->m1->subr_type;
		
	}else{
		
		printf("[Transpiler][Transpile-Lambdas][Error]");
		printf(" Could not find subroutine: %s\n", call->name);
		printf("Exiting.\n");
		exit(1);
	}
	
	//replace the lambda with function reference
	transpile_lambda(
		current_ns, 
		term, 
		lambda,
		lambdaType->return_type,
		lambdaType->arg_types,
		lambdaType->count_arg_types
	);
}

static uint32_t lambda_num = 0;

static void transpile_lambda(
	struct Namespace* ns, 
	struct Term* term,
	struct Lambda* lambda,
	struct Type* orig_rtype,
	struct Type** orig_arg,
	uint8_t orig_count_arg
){
	
	//check that ns has enough capacity for another
	//subroutine, else resize
	if(ns->capacity_methods == ns->count_methods){
	
		ns->capacity_methods *= 2;
		ns->methods = realloc(
				ns->methods, 
				sizeof(struct Method*)*(ns->capacity_methods)
			);
	}
	
	lambda->lambda_num = lambda_num++;
	
	//copy the type and all
	struct Method* lambdaSubr = 
		gen_subr_from_lambda(lambda, orig_rtype, orig_arg, orig_count_arg);
	
	//insert
	ns->methods[ns->count_methods] = lambdaSubr;
	ns->count_methods += 1;
	
	//replace the place the lambda is used 
	//with the name of the generated subr
	struct SimpleVar* lambsimplevar = make(SimpleVar);
	lambsimplevar->count_indices = 0;
	lambsimplevar->indices = NULL;
	sprintf(lambsimplevar->name, LAMBDA_NAME_PATTERN, lambda->lambda_num);
	
	struct Variable* lambvar = make(Variable);
	lambvar->member_access = NULL;
	lambvar->simple_var = lambsimplevar;
	term->ptr.m6 = lambvar;
	term->kind = 6;
	
	//free unused parts of the lambda
	//return expr is USED, identifiers are not used
	for(uint8_t i = 0; i < lambda->count_params; i++){
		freeIdentifier(lambda->params[i]);
	}
	free(lambda);
}


static struct Method* gen_subr_from_lambda(
	struct Lambda* lambda,
	struct Type* orig_rtype,
	struct Type** orig_arg,
	uint8_t orig_count_arg
){
	
	struct Method* lsubr  = make(Method);
	lsubr->is_public       = false;
	lsubr->has_side_effects = false;  //ASSUMPTION TODO: work on it
	lsubr->throws         = false; //ASSUMPTION TODO: work on it
	sprintf(lsubr->name, LAMBDA_NAME_PATTERN, lambda->lambda_num);
	
	struct RetStmt* rstmt = make(RetStmt);
	rstmt->return_value    = lambda->expr;
	
	struct Stmt* stmt = make(Stmt);
	stmt->kind        = 4;
	stmt->ptr.m4      = rstmt;
	stmt->is_continue  = false;
	stmt->is_break     = false;
	
	lsubr->block           = make(StmtBlock);
	lsubr->block->count    = 1;
	lsubr->block->stmts    = malloc(sizeof(struct Stmt*));
	lsubr->block->stmts[0] = stmt;
	
	//copy return type
	lsubr->return_type = copyType(orig_rtype);
	
	//copy declarg 
	lsubr->count_args = orig_count_arg;
	lsubr->args = malloc(sizeof(struct DeclArg*)*orig_count_arg);
	
	for(uint8_t i = 0; i < orig_count_arg; i++){
		
		struct DeclArg* newdeclarg = make(DeclArg);
		
		newdeclarg->type = copyType(orig_arg[i]);
		newdeclarg->has_name = true;
		strcpy(newdeclarg->name, lambda->params[i]->identifier);
		
		lsubr->args[i] = newdeclarg;
	}
	
	return lsubr;
}
