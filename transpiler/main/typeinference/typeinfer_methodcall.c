#include "ast/ast.h"

#include "typeinference/typeinfer.h"

#include "tables/symtable/symtable.h"
#include "tables/sst/sst.h"

struct Type* infer_type_methodcall(struct ST* st, struct Call* m){

	//to also infer it if the subr being called
	//was a local var or arg to the subroutine
	
	if(lvst_contains(st->lvst, m->callable->simple_var->name)){
		
		struct LVSTLine* line2 = lvst_get(st->lvst, m->callable->simple_var->name);
		
		struct Type* type = line2->type;
		
		if(type->basic_type == NULL){
			//ERROR
			goto other;
		}
		if(type->basic_type->subr_type == NULL){
			//ERROR
			goto other;
		}
		
		struct SubrType* stype = type->basic_type->subr_type;
		
		return stype->return_type;
	}
	
	other:
	
	return sst_get(st->sst, m->callable->simple_var->name)->return_type;
}
