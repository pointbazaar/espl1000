#include "ast/visitor/visitor.h"
#include "ast/util/copy_ast.h"
#include "ast/util/free_ast.h"

#include "tables/stst/stst.h"

#include "typeinfer.h"

static struct Type* substitute_type_parameters(struct Type* struct_type, struct Type* member_type);

struct Type* infer_in_context(struct ST* st, struct MemberAccess* ma){

    struct Type* structType = ma->structType;
    struct Variable* member = ma->member;

    char* structName = structType->m1->simple_type->struct_type->type_name;
    char* memberName = member->simple_var->name;

    struct Type* memberType = stst_get_member(st->stst, structName, memberName)->type;

    //if the struct type has a parameterization,
    //then we can substitute that onto the member type
    memberType = substitute_type_parameters(structType, memberType);

    memberType = unwrap_indices(memberType, member->simple_var->count_indices);

    if(member->member_access == NULL)
    { return memberType; }

    struct MemberAccess ma2 = {
            .structType = memberType,
            .member = member->member_access
    };

    return infer_in_context(st, &ma2);
}

static void type_substitution_visitor(void* node, enum NODE_TYPE node_type, void* arg){

	struct Type* type_to_substitute = (struct Type*)arg;

	if (node_type == NODE_TYPE){
		struct Type* instance = (struct Type*)node;
		if (instance->m2 != NULL){

			free_type_param(instance->m2);
			instance->m2 = NULL;

			if (type_to_substitute->m1 != NULL){
				instance->m1 = type_to_substitute->m1;
			}
			if (type_to_substitute->m2 != NULL){
				instance->m2 = type_to_substitute->m2;
			}
		}
	}
}

static struct Type* substitute_type_parameters(struct Type* struct_type, struct Type* member_type) {

	if (struct_type->m1 == NULL){return member_type;}
	if (struct_type->m1->simple_type == NULL){return member_type;}
	if (struct_type->m1->simple_type->struct_type == NULL){return member_type;}
	if (struct_type->m1->simple_type->struct_type->count_type_params != 1){
		return member_type;
	}

	struct Type* res = copy_type(member_type);

	struct Type* type_to_substitute = struct_type->m1->simple_type->struct_type->type_params[0];

	visit_type(res, type_substitution_visitor, type_to_substitute);

	return res;
}

