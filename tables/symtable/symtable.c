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
	
	st->inferred_types_capacity = 100;
	st->inferred_types_count    = 0;
	
	const uint32_t nbytes = 
		sizeof(struct Type*) * st->inferred_types_capacity;
	
	st->inferred_types = malloc(nbytes);
	
	st->sst  = makeSST();
	st->stst = makeSTST();
	st->lvst = makeLVST();
	
	sst_prefill(st, st->sst);
	
	return st;
}

void freeST(struct ST* st){

	if(st->sst  != NULL) { freeSST (st->sst);  }
	if(st->lvst != NULL) { freeLVST(st->lvst); }
	if(st->stst != NULL) { freeSTST(st->stst); }

	for(int i = 0; i < st->inferred_types_count; i++)
		{ freeType(st->inferred_types[i]); }
		
	free(st->inferred_types);
	
	free(st);
}

void st_register_inferred_type(struct ST* st, struct Type* t){
	
	if(st->inferred_types_count >= st->inferred_types_capacity){
		
		//increase capacity
		st->inferred_types_capacity *= 2;
		
		uint64_t nbytes = (st->inferred_types_capacity)*sizeof(struct Type*);
		st->inferred_types = realloc(st->inferred_types, nbytes);
	}
	
	st->inferred_types[st->inferred_types_count] = t;
	st->inferred_types_count += 1;
}
