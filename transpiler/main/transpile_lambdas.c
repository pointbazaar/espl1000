#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast/ast.h"
#include "ast/util/copy_ast.h"
#include "ast/util/free_ast.h"
#include "ast/visitor/visitor.h"

#include "tables/sst/sst.h"
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

void transpileLambdas(struct AST* ast, struct SST* sst){
	
	//we need a visitor to obtain lambda locations
	//in the appropriate contexts (AST Node Patters)
	
	visitAST(ast, myvisitor_transpile_lambdas, sst);
}

static void myvisitor_transpile_lambdas(void* node, enum NODE_TYPE type, void* arg){

	//look at the context of the lambda
	// - (1) subr call param
	// - (2) assignment to variable
	
	//in case (1): find out the subroutine type signature
	//and insert the lambda subroutine at toplevel
	//in the Namespace
	struct SST* sst = (struct SST*) arg;
	
	static struct Namespace* current_ns = NULL;
	
	if(type == NODE_NAMESPACE){
		
		current_ns = node;
		
	} else if(type == NODE_CALL){
		
		struct Call* call = (struct Call*) node;
		
		for(uint8_t i = 0; i < call->count_args; i++){
			
			struct Expr* expr = call->args[i];
			struct Term* term = expr->term1->term;
			
			if(term->kind == 11){
				
				//we have a lambda ^^ 
				struct Lambda* l = term->ptr.m11;
				
				//use SST
				struct SSTLine* line = sst_get(sst, call->name);
				
				struct Method* container = line->method;
				
				struct DeclArg* lambdaArg = container->args[i];
				
				//get the subrtype from the argument
				struct Type* pre_lambdaType = lambdaArg->type;
				struct SubrType* lambdaType = pre_lambdaType->m1->subrType;
				
				//replace the lambda with function reference
				transpile_lambda(
					current_ns, 
					term, 
					l,
					lambdaType->returnType,
					lambdaType->argTypes,
					lambdaType->count_argTypes
				);
			}
		}
	}
	
	//in case (2): ignore, deal with later (TODO)
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
	lambvar->memberAccess = NULL;
	lambvar->simpleVar = lambsimplevar;
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
	lsubr->isPublic       = false;
	lsubr->hasSideEffects = true;
	sprintf(lsubr->name, LAMBDA_NAME_PATTERN, lambda->lambda_num);
	
	struct RetStmt* rstmt = make(RetStmt);
	rstmt->returnValue    = lambda->expr;
	
	struct Stmt* stmt = make(Stmt);
	stmt->kind        = 4;
	stmt->ptr.m4      = rstmt;
	stmt->isContinue  = false;
	stmt->isBreak     = false;
	
	lsubr->block           = make(StmtBlock);
	lsubr->block->count    = 1;
	lsubr->block->stmts    = malloc(sizeof(struct Stmt*));
	lsubr->block->stmts[0] = stmt;
	
	//copy return type
	lsubr->returnType = copyType(orig_rtype);
	
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
