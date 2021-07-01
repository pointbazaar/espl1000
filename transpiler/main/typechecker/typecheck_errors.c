#include <stdio.h>
#include <stdlib.h>

#include "typecheck_errors.h"
#include "tcctx.h"

#define COLOR_RED "\033[31m"

static void setcolor(char* color){
	printf("%s", color);
}

static void resetcolor(){
	
	printf("\033[39m");
}

void error(struct TCCtx* tcctx, char* msg){

	tcctx->tcErrCount += 1;

	printf("[Typecheck]");
	setcolor(COLOR_RED);
	printf("[Error]");
	resetcolor();
	printf("[%s:%d]", tcctx->current_filename, tcctx->current_line_num);
	printf(" %s\n", msg);
	printf("------------------------------------------\n");
}
