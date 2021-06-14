#include <stdlib.h>

#include "ast/ast.h"
#include "ast/util/free_ast.h"

#include "tables/lvst/lvst.h"
#include "tables/sst/sst.h"
#include "tables/stst/stst.h"
#include "symtable.h"
#include "tables/sst/sst_prefill.h"

struct ST* makeST(){

	struct ST* st = make(ST);
	
	st->inferredTypesCapacity = 200;
	st->inferredTypesCount    = 0;
	
	const int nbytes = 
		sizeof(struct Type*) * st->inferredTypesCapacity;
	
	st->inferredTypes = malloc(nbytes);
	
	st->sst  = makeSST();
	st->stst = makeSTST();
	st->lvst = makeLVST();
	
	sst_prefill(st, st->sst);
	
	return st;
}

void freeST(struct ST* st){

	if(st->sst  != NULL) { freeSST(st->sst);   }
	if(st->lvst != NULL) { freeLVST(st->lvst); }
	if(st->stst != NULL) { freeSTST(st->stst); }

	for(int i = 0; i < st->inferredTypesCount; i++)
		{ freeType(st->inferredTypes[i]); }
		
	free(st->inferredTypes);
	
	free(st);
}
