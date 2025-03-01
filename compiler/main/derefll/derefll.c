#include <stdlib.h>
#include <assert.h>

#include "ast/ast_declare.h"
#include "ast/ast.h"

#include "cli/flags/flags.h"
#include "util/ctx.h"

#include "tables/symtable/symtable.h"
#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"

#include "typeinference/typeinfer.h"

#include "derefll.h"

void derefll_append(struct DerefLL* d1, struct DerefLL* d2) {

	struct DerefLL* current = d1;

	while (current->next != NULL)
		current = current->next;

	current->next = d2;
}

struct DerefLL* derefll_deref() {

	struct DerefLL* res = malloc(sizeof(struct DerefLL));

	if (!res) {
		return NULL;
	}

	res->next = NULL;
	res->action = DEREFLL_DEREF;

	return res;
}

struct DerefLL* derefll_structmember(char* name) {

	struct DerefLL* res = malloc(sizeof(struct DerefLL));

	if (!res) {
		return NULL;
	}

	res->next = NULL;
	res->action = DEREFLL_MEMBER;
	res->member_name = name;

	return res;
}

struct DerefLL* derefll_expr(struct Expr* expr) {

	struct DerefLL* res = malloc(sizeof(struct DerefLL));

	if (!res) {
		return NULL;
	}

	res->next = NULL;
	res->action = DEREFLL_INDEX;
	res->index_expr = expr;

	return res;
}

struct DerefLL* derefll_ctor_simplevar(struct SimpleVar* sv, struct Ctx* ctx) {

	if (flags_debug(ctx_flags(ctx))) {
		printf("[debug] %s\n", __func__);
	}

	struct DerefLL* res = malloc(sizeof(struct DerefLL));

	if (!res) {
		return NULL;
	}

	res->next = NULL;
	res->action = DEREFLL_INIT;
	res->initial = sv;

	// in case of a function ptr, we will not find it in LVST
	struct Type* type = infer_type_simplevar(ctx_tables(ctx), sv);

	if (type->basic_type && type->basic_type->subr_type) {
		return res;
	}

	struct LVST* lvst = ctx_tables(ctx)->lvst;
	struct LVSTLine* line = lvst_get(lvst, sv->name);

	// In case of array type, we need an additional deref,
	// to get the pointer out of the stackframe.
	// Deref is implicit for array type
	if (line->type->array_type) {
		derefll_append(res, derefll_deref());
	}

	for (int i = 0; i < sv->count_indices; i++) {

		derefll_append(res, derefll_expr(sv->indices[i])); //append +expr

		if (i < sv->count_indices - 1) {
			// + deref
			derefll_append(res, derefll_deref());
		}
	}

	return res;
}

struct DerefLL* derefll_ctor_variable(struct Variable* v, struct Ctx* ctx) {

	if (flags_debug(ctx_flags(ctx))) {
		printf("[debug] %s\n", __func__);
	}

	struct LVST* lvst = ctx_tables(ctx)->lvst;

	struct DerefLL* res = derefll_ctor_simplevar(v->simple_var, ctx);

	struct Variable* current = v;

	// for example, a function pointer is not in LVST
	// so we cannot access the LVST in that case
	if (current->member_access == NULL) {
		return res;
	}

	struct LVSTLine* line = lvst_get(lvst, current->simple_var->name);
	struct Type* current_type = line->type;

	while (current->member_access != NULL) {

		assert(current->member_access->simple_var);

		// + structmember_offset
		derefll_append(res, derefll_structmember(current->member_access->simple_var->name));

		struct MemberAccess ma = {
		    .structType = current_type,
		    .member = current->member_access,
		};
		struct Type* member_type = infer_in_context_once(ctx_tables(ctx), &ma);

		// since we use this function for variable and variable
		// address, we do not add the last deref here just yet
		if (current->member_access->member_access) {

			// only deref here if our member has pointer type
			// not a reference type
			const bool is_simple = member_type->basic_type && member_type->basic_type->simple_type != NULL;

			if (!is_simple) {
				// + deref
				derefll_append(res, derefll_deref());
			}
		}

		current_type = member_type;
		current = current->member_access;
	}

	return res;
}

void derefll_annotate_types(struct DerefLL* dll, struct Ctx* ctx, struct Type* prev_type) {

	if (flags_debug(ctx_flags(ctx))) {
		printf("[debug] %s\n", __func__);
	}

	struct DerefLL* current = dll;

	if (current == NULL) return;

	struct STST* stst = ctx_tables(ctx)->stst;

	switch (current->action) {

		case DEREFLL_INIT: {
			struct Type* type = infer_type_simplevar_once(ctx_tables(ctx), current->initial);

			current->type = type;
		} break;

		case DEREFLL_INDEX:
			current->type = prev_type;
			break;

		case DEREFLL_MEMBER: {
			assert(prev_type != NULL);
			assert(prev_type->basic_type != NULL);

			char* struct_name = prev_type->basic_type->simple_type->struct_type->type_name;

			struct StructMember* sm = stst_get_member(stst, struct_name, current->member_name);

			current->type = sm->type;
		} break;

		case DEREFLL_DEREF:
			//prev_type must be array/pointer type, since we're dereferencing
			assert(prev_type->array_type || prev_type->pointer_type);
			if (prev_type->array_type) {
				current->type = prev_type->array_type->element_type;
			}
			if (prev_type->pointer_type) {
				current->type = prev_type->pointer_type->element_type;
			}
			break;
	}

	derefll_annotate_types(current->next, ctx, current->type);
}

void derefll_dtor(struct DerefLL* dll) {

	if (dll->next != NULL)
		derefll_dtor(dll->next);

	free(dll);
}
