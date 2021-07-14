#include <string.h>
#include <stdbool.h>

#include "ast/ast.h"
#include "ast/util/str_ast.h"

#include "typeinference/typeinfer.h"

#include "tables/symtable/symtable.h"
#include "tables/lvst/lvst.h"
#include "tables/sst/sst.h"
#include "tables/stst/stst.h"

struct MemberAccess {
	struct Type* structType;
	struct Variable* member;
};

static struct Type* infer_in_context(char* filename, struct ST* st, struct MemberAccess* ma);

struct Type* infer_type_variable(char* filename, struct ST* st, struct Variable* v){
	
	struct Type* typeOfVar = infer_type_simplevar(filename, st, v->simple_var);
	
	if(v->member_access == NULL){ return typeOfVar; }

	struct MemberAccess ma = {
		.structType = typeOfVar,
		.member = v->member_access
	};

	return infer_in_context(filename, st, &ma);
}

static struct Type* infer_in_context(char* filename, struct ST* st, struct MemberAccess* ma){
	
	struct Type* structType = ma->structType;
	struct Variable* member = ma->member;
	
	char* structName = structType->m1->simple_type->struct_type->type_name;
	char* memberName = member->simple_var->name;
	
	struct Type* memberType = stst_get_member(st->stst, structName, memberName)->type;
	
	memberType = unwrap_indices(filename, memberType, member->simple_var->count_indices);
	
	if(member->member_access == NULL)
		{ return memberType; }
		
	struct MemberAccess ma2 = {
		.structType = memberType,
		.member = member->member_access
	};
	
	return infer_in_context(filename, st, &ma2);
}

