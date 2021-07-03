#include <string.h>
#include <stdlib.h>

//AST Includes
#include "ast/ast.h"
#include "ast/util/str_ast.h"
#include "ast/util/equals_ast.h"

//Table Includes
#include "tables/sst/sst.h"
#include "tables/symtable/symtable.h"

//Typeinference Includes
#include "transpiler/main/typeinference/typeinfer.h"

//Typechecker Includes
#include "typecheck_errors.h"
#include "typecheck_utils.h"
#include "typecheck_stmts.h"
#include "typecheck_methodcall.h"
#include "typecheck.h"
#include "tcctx.h"

void tc_methodcall(struct Call* m, struct TCCtx* tcctx){

	tcctx->current_line_num = m->super.line_num;

	struct Type** expect_types = NULL;
	uint8_t expect_args        = 0;
	
	if(sst_contains(tcctx->st->sst, m->name)){
		//look into SST
		struct SSTLine* line = sst_get(tcctx->st->sst, m->name);
		
		if(line->method == NULL){
			if(line->isLibC){
				//we do not have the AST for libC 
				//subroutines, so we cannot typecheck the call
				return;
			}			
			char msg[150];
			sprintf(msg, "SUBR NOT FOUND IN SST: %s\n", m->name);
			strcat(msg, ERR_SUBR_NOT_FOUND);
			error(tcctx, msg);
		}
		
		struct Method* method = line->method;
		expect_args = method->count_args;
		
		expect_types = malloc(sizeof(struct Type*)*expect_args);
		for(uint8_t i=0; i < expect_args; i++){
			expect_types[i] = method->args[i]->type;
		}
	}else if(lvst_contains(tcctx->st->lvst, m->name)){
		//look into LVST
		
		struct LVSTLine* line2 = lvst_get(tcctx->st->lvst, m->name);
		
		struct Type* type = line2->type;
		if(type->m1 == NULL || type->m1->subrType == NULL){ 
			error(tcctx, "SUBR HAD WRONG TYPE IN LVST");
		}
		struct SubrType* stype = type->m1->subrType;
		
		expect_args  = stype->count_argTypes;
		expect_types = malloc(sizeof(struct Type*)*expect_args);
		for(uint8_t i=0; i < expect_args; i++){
			expect_types[i] = stype->argTypes[i];
		}
	}else{
		
		error(tcctx, "SUBR neither in SST nor LVST");
	}
	
	const uint8_t actual_args = m->count_args;
	
	if(actual_args != expect_args){
		
		char* s1 = strCall(m);
		
		char msg[200];
		sprintf(msg, "\t%s\nexpected: %d args\n", s1, expect_args);
		strcat(msg, ERR_NUM_ARGS);
		
		free(s1);
		free(expect_types);
		
		error(tcctx, msg);
	}
	
	for(uint8_t i = 0; i < expect_args; i++){
	
		struct Type* expect_type = expect_types[i];
		
		struct Type* actual_type = 
			infer_type_expr(tcctx->current_filename, tcctx->st, m->args[i]);
			
		if(is_integer_type(expect_type) 
		&& is_integer_type(actual_type))
			{ continue; }
		
		if(!eq_type(expect_type, actual_type)){
			
			char* s1 = strCall(m);
			char* s2 = strExpr(m->args[i]);
			
			char* sTypeActual   = strType(actual_type);
			char* sTypeExpected = strType(expect_type);
			
			char msg[200];
			sprintf(msg, "\t%s\n%s, (of type %s), but expected type %s\n", s1, s2, sTypeActual, sTypeExpected);
			strcat(msg, ERR_ARG_TYPES);
			
			free(s1);
			free(s2);
			free(sTypeActual);
			free(sTypeExpected);
			
			free(expect_types);
			error(tcctx, msg);
		}
	}
	
	free(expect_types);
}
