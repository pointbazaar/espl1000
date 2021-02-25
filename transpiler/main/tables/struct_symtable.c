#include <stdlib.h>
#include <stdio.h>

#include "../../../ast/ast.h"

#include "tables/struct_symtable.h"

struct STST* makeStructSymTable(struct Namespace* ns, bool debug){
	
	if(debug){ printf("makeStructSymTable(...)\n"); }
	
	struct STST* stst = malloc(sizeof(struct STST));
	
	stst->lines = NULL;
	stst->count = 0;
	stst->capacity = 0;
	
	//TODO: implement
	
	return stst;
}

void freeStructSymTable(struct STST* stst){
	
	//TODO: implement
	
	free(stst);
}
