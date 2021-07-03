
#include <stdlib.h>
#include <inttypes.h>

#include "sst_fill.h"
#include "../symtable/symtable.h"
#include "ast/util/copy_ast.h"

void sst_fill(struct ST* st, struct SST* sst, struct Namespace* ns){
	
	for(int i = 0; i < ns->count_methods; i++){
		
		struct Method* m = ns->methods[i];
		
		struct Type* t = method_to_subrtype(m);
		
		st_register_inferred_type(st, t);
		
		struct SSTLine* line = makeSSTLine2(m, t, ns->name);
		
		sst_add(sst, line);
	}
}

struct Type* method_to_subrtype(struct Method* m){
	
	struct Type* t         = make(Type);
	struct BasicType* bt   = make(BasicType);
	struct SubrType* stype = make(SubrType);
	
	stype->super.annotations = 0;
	stype->super.line_num    = m->super.line_num;
	
	stype->returnType     = copyType(m->returnType);
	stype->hasSideEffects = m->hasSideEffects;
	stype->count_argTypes = m->count_args;
	
	stype->argTypes = malloc(sizeof(void*)*stype->count_argTypes);
	
	for(uint32_t i = 0; i < stype->count_argTypes; i++){
		stype->argTypes[i] = copyType(m->args[i]->type);
	}
	
	bt->simpleType = NULL;
	bt->subrType   = stype;
	t->m1 = bt;
	t->m2 = NULL;
	t->m3 = NULL;
	
	return t;
}
