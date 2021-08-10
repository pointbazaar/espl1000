#include "ast/visitor/visitor.h"
#include "ast/util/copy_ast.h"
#include "ast/util/free_ast.h"

#include "tables/stst/stst.h"

#include "typeinfer.h"

static struct Type* substitute_type_parameters(struct Type* struct_type, struct Type* member_type);

struct Type* infer_in_context(struct ST* st, struct MemberAccess* ma){

    struct Type* structType = ma->structType;
    struct Variable* member = ma->member;

    char* structName = structType->basic_type->simple_type->struct_type->type_name;
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
		if (instance->type_param != NULL){

			free_type_param(instance->type_param);
			instance->type_param = NULL;

			if (type_to_substitute->basic_type != NULL){
				instance->basic_type = type_to_substitute->basic_type;
			}
			if (type_to_substitute->type_param != NULL){
				instance->type_param = type_to_substitute->type_param;
			}
		}
	}
}

static struct Type* substitute_type_parameters(struct Type* struct_type, struct Type* member_type) {

	if (struct_type->basic_type == NULL){return member_type;}
	if (struct_type->basic_type->simple_type == NULL){return member_type;}
	if (struct_type->basic_type->simple_type->struct_type == NULL){return member_type;}
	if (struct_type->basic_type->simple_type->struct_type->count_type_params != 1){
		return member_type;
	}

	struct Type* res = copy_type(member_type);

	//st_register_inferred_type(sym_tables, res);

	struct Type* type_to_substitute = struct_type->basic_type->simple_type->struct_type->type_params[0];

	visit_type(res, type_substitution_visitor, type_to_substitute);

	return res;
}

