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

static struct Type* infer_in_context(struct ST* st, struct MemberAccess* ma);

struct Type* infer_type_variable(struct ST* st, struct Variable* v){
	
	struct Type* typeOfVar = infer_type_simplevar(st, v->simpleVar);
	
	if(v->memberAccess == NULL){ return typeOfVar; }

	struct MemberAccess ma = {
		.structType = typeOfVar,
		.member = v->memberAccess
	};

	return infer_in_context(st, &ma);
}

static struct Type* infer_in_context(struct ST* st, struct MemberAccess* ma){
	
	struct Type* structType = ma->structType;
	struct Variable* member = ma->member;
	
	char* structName = structType->m1->simpleType->structType->typeName;
	char* memberName = member->simpleVar->name;
	
	struct Type* memberType = stst_get_member_type(st->stst, structName, memberName);
	
	memberType = unwrap_indices(memberType, member->simpleVar->count_indices);
	
	if(member->memberAccess == NULL)
		{ return memberType; }
		
	struct MemberAccess ma2 = {
		.structType = memberType,
		.member = member->memberAccess
	};
	
	return infer_in_context(st, &ma2);
}

