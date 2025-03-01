#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "ast/ast.h"
#include "ast/util/copy_ast.h"

#include "typeinference/typeinfer.h"

#include "tables/symtable/symtable.h"

struct Type* infer_type_address_of(struct ST* st, struct AddressOf* ao) {

	struct Type* t = infer_type_term(st, ao->term);

	struct PointerType* pt = make(PointerType);
	pt->element_type = copy_type(t);

	struct Type* res = make(Type);

	res->basic_type = NULL;
	res->type_param = NULL;
	res->array_type = NULL;
	res->pointer_type = pt;

	st_register_inferred_type(st, res);

	return t;
}
