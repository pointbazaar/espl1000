#include <stdio.h>
#include <stdlib.h>

#include "typecheck_errors.h"
#include "tcctx.h"

void error(struct TCCtx* tcctx, char* msg){

	tcctx->tcErrCount += 1;

	printf("[Typecheck][Error] %s\n", msg);
}
