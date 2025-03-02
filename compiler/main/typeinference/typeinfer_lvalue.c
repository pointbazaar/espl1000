#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "ast/ast.h"

#include "typeinference/typeinfer.h"
#include "typeinference/util/type_str.h"

#include "tables/symtable/symtable.h"

struct Type* infer_type_lvalue(struct ST* st, struct LValue* lv) {

	if (lv->deref) {
		return infer_type_deref(st, lv->deref);
	}
	if (lv->var) {
		return infer_type_variable(st, lv->var);
	}
	return NULL;
}
