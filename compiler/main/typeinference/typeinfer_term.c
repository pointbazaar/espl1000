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

		case 4: return infer_type_methodcall(st, t->ptr.m4);
		case 5: return infer_type_expr(st, t->ptr.m5);
		case 6: return infer_type_variable(st, t->ptr.m6);
		case 8: return typeFromStrArray(st, "char");
		case 11:
			printf("cannot infer type of lambda");
			printf("[Typeinference][Error] Fatal. (in typeinfer_term.c).\n");
			return NULL;

		case 12: return infer_type_constvalue(st, t->ptr.m12);

		//MDirect is of type uint8
		case 13: return typeFromStrPrimitive(st, "uint8");

		default:
			fprintf(stderr, "[Typeinference][Error] Fatal. (in typeinfer_term.c).\n");
			return NULL;
	}
}
