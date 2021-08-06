
#include "tables/stst/stst.h"
#include "ast/util/copy_ast.h"
#include "typeinfer.h"

struct Type* infer_in_context(struct ST* st, struct MemberAccess* ma){

    struct Type* structType = ma->structType;
    struct Variable* member = ma->member;

    char* structName = structType->m1->simple_type->struct_type->type_name;
    char* memberName = member->simple_var->name;

    struct Type* memberType = stst_get_member(st->stst, structName, memberName)->type;

    //if the struct type has a parameterization,
    //then we can substitute that onto the member type
    //TODO: fix this, this is broken/workaround
    if(memberType->m2 != NULL){
        if(structType->m1 != NULL && structType->m1->simple_type != NULL && structType->m1->simple_type->struct_type != NULL){
            struct StructType* myStructType = structType->m1->simple_type->struct_type;
            if(myStructType->count_type_params != 0){
                memberType = copy_type(myStructType->type_params[0]);
            }
        }
    }

    memberType = unwrap_indices(memberType, member->simple_var->count_indices);

    if(member->member_access == NULL)
    { return memberType; }

    struct MemberAccess ma2 = {
            .structType = memberType,
            .member = member->member_access
    };

    return infer_in_context(st, &ma2);
}

