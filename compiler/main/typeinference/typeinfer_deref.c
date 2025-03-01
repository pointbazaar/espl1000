#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "ast/ast.h"

#include "typeinference/typeinfer.h"
#include "typeinference/util/type_str.h"

#include "tables/symtable/symtable.h"

struct Type* infer_type_deref(struct ST* st, struct Deref* d) {

	struct Type* t = infer_type_term(st, d->term);

	// can only deref pointer types
	assert(t->pointer_type);

	return t->pointer_type->element_type;
}
