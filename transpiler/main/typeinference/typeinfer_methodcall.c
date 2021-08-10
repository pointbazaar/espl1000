#include "ast/ast.h"

#include "typeinference/typeinfer.h"

#include "tables/symtable/symtable.h"

struct Type* infer_type_methodcall(struct ST* st, struct Call* m){

    struct Type* type = infer_type_variable(st, m->callable);

    return type->basic_type->subr_type->return_type;
}
