#include "ast/visitor/visitor.h"
#include "ast/util/copy_ast.h"
#include "ast/util/free_ast.h"

#include <assert.h>
#include <stdlib.h>

#include "ast/util/str_ast.h"
#include "tables/stst/stst.h"

#include "typeinfer.h"

struct Type* infer_in_context_once(struct ST* st, struct MemberAccess* ma) {

	struct Type* structType = ma->structType;
	struct Variable* member = ma->member;

	// We can do member access through a pointer.
	// The result will be the element type
	if (structType->pointer_type != NULL) {
		structType = structType->pointer_type->element_type;
	}

	if (structType->basic_type == NULL) {
		char* s1 = str_type(structType);
		fprintf(stderr, "was not struct type: %s\n", s1);
		free(s1);
		return NULL;
	}

	struct BasicType* bt = structType->basic_type;

	assert(structType->basic_type->simple_type != NULL);

	struct SimpleType* simple_type = bt->simple_type;

	if (simple_type->struct_type == NULL) {
		char* s = str_simple_type(simple_type);
		fprintf(stderr, "was not struct type: %s\n", s);
		free(s);
		return NULL;
	}

	struct StructType* struct_type = simple_type->struct_type;

	assert(struct_type->type_name != NULL);

	char* structName = struct_type->type_name;
	char* memberName = member->simple_var->name;

	struct Type* memberType = stst_get_member(st->stst, structName, memberName)->type;

	memberType = unwrap_indices(memberType, member->simple_var->count_indices);

	return memberType;
}

struct Type* infer_in_context(struct ST* st, struct MemberAccess* ma) {

	struct Type* structType = ma->structType;
	struct Variable* member = ma->member;

	struct Type* memberType = infer_in_context_once(st, ma);

	if (member->member_access == NULL) { return memberType; }

	struct MemberAccess ma2 = {
	    .structType = memberType,
	    .member = member->member_access};

	return infer_in_context(st, &ma2);
}
