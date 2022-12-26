#include <stdlib.h>

#include "ast/ast_declare.h"
#include "ast/ast.h"

#include "util/ctx.h"

#include "tables/symtable/symtable.h"
#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"

#include "derefll.h"

void derefll_append(struct DerefLL* d1, struct DerefLL* d2){

	struct DerefLL* current = d1;

	while(current->next != NULL)
		current = current->next;

	current->next = d2;
}

struct DerefLL* derefll_deref(){

	struct DerefLL* res = exit_malloc(sizeof(struct DerefLL));

	res->next = NULL;
	res->action = DEREFLL_DEREF;

	return res;
}

struct DerefLL* derefll_structmember(char* name){

	struct DerefLL* res = exit_malloc(sizeof(struct DerefLL));

	res->next = NULL;
	res->action = DEREFLL_MEMBER;
	res->member_name = name;

	return res;
}

struct DerefLL* derefll_expr(struct Expr* expr){

	struct DerefLL* res = exit_malloc(sizeof(struct DerefLL));

	res->next = NULL;
	res->action = DEREFLL_INDEX;
	res->index_expr = expr;

	return res;
}

struct DerefLL* derefll_ctor_simplevar(struct SimpleVar* sv){

	struct DerefLL* res = exit_malloc(sizeof(struct DerefLL));

	res->next = NULL;
	res->action = DEREFLL_INIT;
	res->initial = sv;

	derefll_append(res, derefll_deref()); //NEW

	for(int i = 0; i < sv->count_indices; i++){

		derefll_append(res, derefll_expr(sv->indices[i])); //append +expr

		derefll_append(res, derefll_deref()); //append deref
	}

	return res;
}

struct DerefLL* derefll_ctor_variable(struct Variable* v){

	struct DerefLL* res = derefll_ctor_simplevar(v->simple_var);

	struct Variable* current = v;

	while(current->member_access != NULL){

		// + structmember_offset
		derefll_append(res, derefll_structmember(current->member_access->simple_var->name));

		// + deref
		derefll_append(res, derefll_deref());

		current = current->member_access;
	}

	return res;
}

static struct Type* arraytype_wrap(struct Type* type){

	struct Type* res = exit_malloc(sizeof(struct Type));

	res->basic_type = NULL;
	res->type_param = NULL;

	struct ArrayType* at = exit_malloc(sizeof(struct ArrayType));
	at->element_type = type;
	res->array_type = at;

	return res;
}

void derefll_annotate_types(struct DerefLL* dll, struct Ctx* ctx, struct Type* prev_type){

	struct DerefLL* current = dll;

	if(current == NULL) return;

	struct STST* stst = ctx_tables(ctx)->stst;

	switch(current->action){

	case DEREFLL_INIT:
		{
		struct LVSTLine* line = lvst_get(ctx_tables(ctx)->lvst, current->initial->name);

		//make an ArrayType of that type, as the first element
		//of a DerefLL is always address
		current->type = arraytype_wrap(line->type);
		}
		break;

	case DEREFLL_INDEX:
		current->type = prev_type;
		break;

	case DEREFLL_MEMBER:
		{
		if(prev_type == NULL) exit(1); //should not happen
		if(prev_type->basic_type == NULL) exit(1); //should not happen

		char* struct_name = prev_type->basic_type->simple_type->struct_type->type_name;

		struct StructMember* sm = stst_get_member(stst, struct_name, current->member_name);

		current->type = arraytype_wrap(sm->type);
		}
		break;

	case DEREFLL_DEREF:
		//prev_type must be array type, since we're dereferencing
		current->type = prev_type->array_type->element_type;
		break;
	}

	derefll_annotate_types(current->next, ctx, current->type);
}

void derefll_dtor(struct DerefLL* dll){

	if(dll->next != NULL)
		derefll_dtor(dll->next);

	free(dll);
}
