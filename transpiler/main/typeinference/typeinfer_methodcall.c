#include "ast/ast.h"

#include "typeinference/typeinfer.h"

#include "tables/symtable/symtable.h"
#include "tables/sst/sst.h"

struct Type* infer_type_methodcall(struct ST* st, struct Call* m){
	
	//TODO: use st_get_subr(sst, lvst, m->name);
	//to also infer it if the subr being called
	//was a local var or arg to the subroutine
	
	if(lvst_contains(st->lvst, m->name)){
		
		struct LVSTLine* line2 = lvst_get(st->lvst, m->name);
		
		struct Type* type = line2->type;
		
		if(type->m1 == NULL){ 
			//ERROR
			goto other;
		}
		if(type->m1->subrType == NULL){ 
			//ERROR
			goto other;
		}
		
		struct SubrType* stype = type->m1->subrType;
		
		return stype->returnType;
	}
	
	other:
	
	return sst_get(st->sst, m->name)->returnType;
}
