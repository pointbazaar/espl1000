#include <stdio.h>
#include <stdlib.h>

#include "tc_errors.h"
#include "typechecker/tcctx.h"

#define COLOR_RED "\033[31m"

const char* tc_err_messages[TC_ERR_END] = {
	[TC_ERR_ARG_NUM_MISMATCH]        = "number of args does not match",
	[TC_ERR_ARG_TYPE_MISMATCH]       = "type of arg does not match",
	
	[TC_ERR_WRONG_RETURN_TYPE]       = "return type does not match returned type",
	
	[TC_ERR_BINOP_TYPE_MISMATCH]     = "type mismatch with binary operator",
	
	[TC_ERR_SUBR_NOT_FOUND]          = "subroutine was not found",
	[TC_ERR_VAR_NOT_FOUND]           = "variable was not found",
	[TC_ERR_VAR_READONLY]            = "variable is read-only",
	
	[TC_ERR_ASSIGN_TYPE_MISMATCH]    = "type mismatch in assignment",
	
	[TC_ERR_CALLED_THROWING_WRONG]   = "wrong usage for calling a throwing subroutine",
	
	[TC_ERR_RANGE_REQUIRES_INT]      = "range requires 2 integer values",
	
	[TC_ERR_THROW_WRONG_USAGE]       = "cannot use 'throw' outside try-block or 'throws' subroutine.",
	
	[TC_ERR_CONDITION_REQUIRES_BOOL] = "condition must be of type bool",
	
	[TC_ERR_SWITCH_REQUIRES_PRIMITIVE_TYPE] = "switch requires a primitive type",
	[TC_ERR_SWITCH_CASE_TYPE_MISMATCH] = "case must have same type as in switch",
	
	[TC_ERR_WRONG_OP_UNOP]           = "illegal unary operator",
	
	[TC_ERR_INDEX_NOT_INTEGER_TYPE]  = "index is not of an integer type",
	[TC_ERR_TOO_MANY_INDICES]        = "too many indices",
	
	[TC_ERR_LOCAL_VAR_NOT_A_SUBROUTINE] = "variable is not of a function type",
	
	[TC_ERR_SIDE_EFFECT_IN_PURE_CONTEXT] = "side effect in a pure context",
	
	[TC_ERR_OTHER]                   = "other type error (unspecified)",
};

void print_tc_error(struct TCError* tc_err);

static void setcolor(char* color){
	printf("%s", color);
}

static void resetcolor(){
	printf("\033[39m");
}

static struct TCError* tc_error_ctor(){
    struct TCError* res = malloc(sizeof(struct TCError));
    res->next        = NULL;
    res->opt_msg         = NULL;
    res->filename    = NULL;
    res->err_kind    = TC_ERR_OTHER;
    res->opt_snippet = NULL;
    return res;
}

static struct TCError* obtain_next_error(struct TCCtx* tcctx){
	
	struct TCError* next;

    if(tcctx->tc_first_err == NULL){
        tcctx->tc_first_err = tc_error_ctor();
        tcctx->tc_last_err  = tcctx->tc_first_err;

        next = tcctx->tc_last_err;
    }else{
        next = tc_error_ctor();
        tcctx->tc_last_err->next = next;
        tcctx->tc_last_err = next;
    }

	tcctx->tc_err_count += 1;
	return next;
}

void error(struct TCCtx* tcctx, char* msg, enum TC_ERR_KIND err_kind){

	error_snippet_and_msg(tcctx, NULL, msg, err_kind);
}

void error_snippet(struct TCCtx* tcctx, char* snippet, enum TC_ERR_KIND err_kind){
	
	error_snippet_and_msg(tcctx, snippet, NULL, err_kind);
}


void error_snippet_and_msg(struct TCCtx* tcctx, char* snippet, char* msg, enum TC_ERR_KIND err_kind){
	
	struct TCError* next = obtain_next_error(tcctx);

    next->filename = tcctx->current_filename;
    next->opt_msg  = msg;
    next->line_num = tcctx->current_line_num;
    next->err_kind = err_kind;
    
    next->opt_snippet = snippet;

    if(tcctx->print_errors){
        print_tc_error(next);
    }
}

void print_tc_error(struct TCError* tc_err){
	
    printf("[Typecheck]");
    setcolor(COLOR_RED);
    printf("[Error]");
    resetcolor();
    printf("[%s:%d]", tc_err->filename, tc_err->line_num);
    
    //print a standard message according to the error kind
    printf(" %s\n", tc_err_messages[tc_err->err_kind]);
    
    if(tc_err->opt_msg != NULL){
		printf(" %s\n", tc_err->opt_msg);
	}
    
    //optionally, print a code snippet (generated from AST)
    if(tc_err->opt_snippet != NULL){
		
		printf(" % 3d |  %s  \n", tc_err->line_num, tc_err->opt_snippet);
	}

    printf("\n");
    //printf("------------------------------------------\n");
}
