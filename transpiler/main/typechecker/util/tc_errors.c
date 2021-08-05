#include <stdio.h>
#include <stdlib.h>

#include "tc_errors.h"
#include "typechecker/tcctx.h"

#define COLOR_RED "\033[31m"

void print_tc_error(struct TCError* tc_err);

static void setcolor(char* color){
	printf("%s", color);
}

static void resetcolor(){
	
	printf("\033[39m");
}

static struct TCError* tc_error_ctor(){
    struct TCError* res = malloc(sizeof(struct TCError));
    res->next = NULL;
    res->msg = NULL;
    res->filename = NULL;
    res->err_kind = TC_ERR_OTHER;
    return res;
}

void error(struct TCCtx* tcctx, char* msg, enum TC_ERR_KIND err_kind){

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

    next->filename = tcctx->current_filename;
    next->msg = msg;
    next->line_num = tcctx->current_line_num;
    next->err_kind = err_kind;

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
    printf(" %s\n", tc_err->msg);
    printf("------------------------------------------\n");
}