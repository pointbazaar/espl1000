#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "ast/ast.h"

#include "typeinference/typeinfer.h"
#include "typeinference/util/type_str.h"

#include "tables/symtable/symtable.h"

struct Type* infer_type_unopterm(struct ST* st, struct UnOpTerm* t) {

	if (t->term) {
		return infer_type_term(st, t->term);
	}
	if (t->deref) {
		return infer_type_deref(st, t->deref);
	}
	if (t->address_of) {
		return infer_type_address_of(st, t->address_of);
	}

	fprintf(stderr, "[Typeinference][Error] %s\n", __func__);
	return NULL;
}

struct Type* infer_type_term(struct ST* st, struct Term* t) {

	switch (t->kind) {

		case TERM_KIND_CALL: return infer_type_methodcall(st, t->ptr.call_term);
		case TERM_KIND_EXPR: return infer_type_expr(st, t->ptr.expr_term);
		case TERM_KIND_VAR: return infer_type_variable(st, t->ptr.var_term);
		case TERM_KIND_STRINGCONST: return typeFromStrArray(st, "char");
		case TERM_KIND_CONSTVALUE: return infer_type_constvalue(st, t->ptr.constvalue_term);
		case TERM_KIND_ENUM_VALUE: return typeFromStrPrimitive(st, "uint64");

		default:
			fprintf(stderr, "[Typeinference][Error] Fatal. (in typeinfer_term.c).\n");
			return NULL;
	}
}
