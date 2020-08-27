#include <stdio.h>

#include "code_gen_util.h"

void indent(struct Ctx* ctx){
	
	for(int i=0;i < ctx->indentLevel; i++){
		fprintf(ctx->file, "\t");
	}
}
