#include <string.h>
#include <stdbool.h>
#include <ast/util/copy_ast.h>

#include "ast/ast.h"

#include "typeinference/typeinfer.h"

#include "tables/symtable/symtable.h"
#include "tables/stst/stst.h"


struct Type* infer_type_variable(struct ST* st, struct Variable* v){
	
	struct Type* typeOfVar = infer_type_simplevar(st, v->simple_var);
	
	if(v->member_access == NULL){ return typeOfVar; }

	struct MemberAccess ma = {
		.structType = typeOfVar,
		.member = v->member_access
	};

	return infer_in_context(st, &ma);
}
