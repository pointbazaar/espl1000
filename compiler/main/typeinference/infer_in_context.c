#include "ast/visitor/visitor.h"
#include "ast/util/copy_ast.h"
#include "ast/util/free_ast.h"

#include "tables/stst/stst.h"

#include "typeinfer.h"

struct Type* infer_in_context(struct ST* st, struct MemberAccess* ma) {

	struct Type* structType = ma->structType;
	struct Variable* member = ma->member;

	char* structName = structType->basic_type->simple_type->struct_type->type_name;
	char* memberName = member->simple_var->name;

	struct Type* memberType = stst_get_member(st->stst, structName, memberName)->type;

	memberType = unwrap_indices(memberType, member->simple_var->count_indices);

	if (member->member_access == NULL) { return memberType; }

	struct MemberAccess ma2 = {
	    .structType = memberType,
	    .member = member->member_access};

	return infer_in_context(st, &ma2);
}
