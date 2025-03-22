#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../ast.h"
#include "copy_ast.h"

struct Expr* copy_expr(struct Expr* expr) {

	struct Expr* res = make(Expr);
	res->op = OP_NONE;
	res->term2 = NULL;

	res->term1 = copy_un_op_term(expr->term1);
	if (res->op != OP_NONE) {
		res->op = expr->op;
		res->term2 = copy_un_op_term(expr->term2);
	}
	return res;
}

struct Id* copy_identifier(struct Id* id) {

	struct Id* res = make(Id);
	asprintf(&(res->identifier), "%s", id->identifier);
	return res;
}

struct Range* copy_range(struct Range* r) {
	struct Range* res = make(Range);

	res->super.annotations = r->super.annotations;
	res->super.line_num = r->super.line_num;

	res->start = copy_expr(r->start);
	res->end = copy_expr(r->end);
	return res;
}

struct StmtBlock* copy_stmt_block(struct StmtBlock* s) {

	struct StmtBlock* res = make(StmtBlock);

	res->super.annotations = s->super.annotations;
	res->super.line_num = s->super.line_num;

	res->count = s->count;
	res->stmts = malloc(sizeof(void*) * res->count);
	for (int i = 0; i < res->count; ++i) {
		res->stmts[i] = copy_stmt(s->stmts[i]);
	}
	return res;
}

struct ConstValue* copy_const_value(struct ConstValue* cv) {

	struct ConstValue* res = make(ConstValue);
	res->kind = cv->kind;

	switch (res->kind) {
		case 1: res->ptr.m1_bool_const = cv->ptr.m1_bool_const; break;
		case 2: res->ptr.m2_int_const = cv->ptr.m2_int_const; break;
		case 3: res->ptr.m3_char_const = cv->ptr.m3_char_const; break;
		case 5: res->ptr.m5_hex_const = cv->ptr.m5_hex_const; break;
		case 6: res->ptr.m5_hex_const = cv->ptr.m5_hex_const; break;
	}

	res->super.line_num = cv->super.line_num;
	res->super.annotations = cv->super.annotations;

	return res;
}
struct SimpleVar* copy_simple_var(struct SimpleVar* sv) {

	struct SimpleVar* res = make(SimpleVar);

	asprintf(&(res->name), "%s", sv->name);

	res->count_indices = sv->count_indices;
	res->indices = malloc(sizeof(struct Expr*) * res->count_indices);

	for (int i = 0; i < res->count_indices; i++) {
		res->indices[i] = copy_expr(sv->indices[i]);
	}

	return res;
}

struct Term* copy_term(struct Term* t) {

	struct Term* res = make(Term);
	res->kind = t->kind;

	switch (t->kind) {

		case 4: res->ptr.m4 = copy_call(t->ptr.m4); break;
		case 5: res->ptr.m5 = copy_expr(t->ptr.m5); break;
		case 6: res->ptr.m6 = copy_variable(t->ptr.m6); break;
		case 8: res->ptr.m8 = copy_string_const(t->ptr.m8); break;
		case 12: res->ptr.m12 = copy_const_value(t->ptr.m12); break;
		default:
			fprintf(stderr, "[AST][Error] copy_term(...), kind was: %d\n", t->kind);
			return NULL;
	}

	return res;
}
struct AddressOf* copy_address_of(struct AddressOf* ao) {
	struct AddressOf* res = make(AddressOf);
	res->term = copy_term(ao->term);
	return res;
}
struct Deref* copy_deref(struct Deref* d) {
	struct Deref* res = make(Deref);
	res->term = copy_term(d->term);
	return res;
}

struct UnOpTerm* copy_un_op_term(struct UnOpTerm* t) {

	struct UnOpTerm* res = make(UnOpTerm);

	if (t->address_of != NULL) {
		res->address_of = copy_address_of(t->address_of);
	} else if (t->deref != NULL) {
		res->deref = copy_deref(t->deref);
	} else {
		res->op = t->op;
		res->term = copy_term(t->term);
	}

	return res;
}

struct Variable* copy_variable(struct Variable* var) {

	struct Variable* res = make(Variable);

	res->simple_var = copy_simple_var(var->simple_var);

	res->member_access = NULL;

	if (var->member_access != NULL) {

		res->member_access = copy_variable(var->member_access);
	}

	return res;
}

struct LValue* copy_lvalue(struct LValue* lv) {

	struct LValue* res = make(LValue);
	if (!res) {
		return NULL;
	}
	res->deref = lv->deref;
	res->var = lv->var;
	return res;
}

struct StringConst* copy_string_const(struct StringConst* s) {

	struct StringConst* res = make(StringConst);
	res->value = malloc(sizeof(char) * (strlen(s->value) + 1));
	strcpy(res->value, s->value);
	return res;
}

struct Type* copy_type(struct Type* t) {

	struct Type* res = make(Type);

	if (!res) {
		return NULL;
	}

	res->basic_type = NULL;
	res->type_param = NULL;
	res->array_type = NULL;
	res->pointer_type = NULL;
	res->is_anytype = t->is_anytype;

	if (t->basic_type != NULL) { res->basic_type = copy_basic_type(t->basic_type); }
	if (t->type_param != NULL) { res->type_param = copy_type_param(t->type_param); }
	if (t->array_type != NULL) { res->array_type = copy_array_type(t->array_type); }
	if (t->pointer_type != NULL) { res->pointer_type = copy_pointer_type(t->pointer_type); }

	return res;
}

struct TypeParam* copy_type_param(struct TypeParam* t) {

	struct TypeParam* res = make(TypeParam);

	res->index = t->index;

	return res;
}

struct SubrType* copy_subr_type(struct SubrType* s) {

	struct SubrType* res = make(SubrType);

	res->has_side_effects = s->has_side_effects;
	res->return_type = copy_type(s->return_type);

	res->count_arg_types = s->count_arg_types;

	res->arg_types = malloc(sizeof(struct Type*) * (s->count_arg_types));

	for (uint8_t i = 0; i < res->count_arg_types; i++) {

		res->arg_types[i] = copy_type(s->arg_types[i]);
	}

	return res;
}

struct SimpleType* copy_simple_type(struct SimpleType* s) {

	struct SimpleType* res = make(SimpleType);

	res->primitive_type = NULL;
	res->struct_type = NULL;

	if (s->primitive_type != NULL) { res->primitive_type = copy_primitive_type(s->primitive_type); }
	if (s->struct_type != NULL) { res->struct_type = copy_struct_type(s->struct_type); }

	return res;
}

struct StructType* copy_struct_type(struct StructType* s) {

	struct StructType* res = make(StructType);

	asprintf(&(res->type_name), "%s", s->type_name);

	return res;
}

struct PrimitiveType* copy_primitive_type(struct PrimitiveType* p) {

	struct PrimitiveType* res = make(PrimitiveType);

	memcpy(res, p, sizeof(struct PrimitiveType));

	return res;
}

struct BasicType* copy_basic_type(struct BasicType* b) {

	struct BasicType* res = make(BasicType);

	res->simple_type = NULL;
	res->subr_type = NULL;

	if (b->simple_type != NULL) { res->simple_type = copy_simple_type(b->simple_type); }
	if (b->subr_type != NULL) { res->subr_type = copy_subr_type(b->subr_type); }

	return res;
}

struct ArrayType* copy_array_type(struct ArrayType* a) {

	struct ArrayType* res = make(ArrayType);

	res->element_type = copy_type(a->element_type);

	return res;
}

struct PointerType* copy_pointer_type(struct PointerType* pt) {

	struct PointerType* res = make(PointerType);

	if (!res) {
		return NULL;
	}

	res->element_type = copy_type(pt->element_type);

	return res;
}

struct Call* copy_call(struct Call* c) {

	struct Call* res = make(Call);
	res->callable = copy_variable(c->callable);

	res->args = malloc(sizeof(struct Expr*) * c->count_args);
	res->count_args = c->count_args;
	for (int i = 0; i < c->count_args; i++) {
		res->args[i] = copy_expr(c->args[i]);
	}

	return res;
}
struct Stmt* copy_stmt(struct Stmt* stmt) {

	struct Stmt* res = make(Stmt);

	res->super.line_num = stmt->super.line_num;
	res->super.annotations = stmt->super.annotations;

	res->kind = stmt->kind;
	switch (stmt->kind) {
		case 1: res->ptr.m1 = copy_call(stmt->ptr.m1); break;
		case 2: res->ptr.m2 = copy_while_stmt(stmt->ptr.m2); break;
		case 3: res->ptr.m3 = copy_if_stmt(stmt->ptr.m3); break;
		case 4: res->ptr.m4 = copy_ret_stmt(stmt->ptr.m4); break;
		case 5: res->ptr.m5 = copy_assign_stmt(stmt->ptr.m5); break;
		case 7: res->ptr.m7 = copy_for_stmt(stmt->ptr.m7); break;
		default:
			res->is_break = stmt->is_break;
			res->is_continue = stmt->is_continue;
			break;
	}

	return res;
}

struct IfStmt* copy_if_stmt(struct IfStmt* i) {

	struct IfStmt* res = make(IfStmt);
	res->else_block = NULL;

	res->super.line_num = i->super.line_num;
	res->super.annotations = i->super.annotations;

	res->condition = copy_expr(i->condition);
	res->block = copy_stmt_block(i->block);

	if (i->else_block != NULL) {
		res->else_block = copy_stmt_block(i->else_block);
	}

	return res;
}

struct AssignStmt* copy_assign_stmt(struct AssignStmt* a) {
	struct AssignStmt* res = make(AssignStmt);

	res->super.line_num = a->super.line_num;
	res->super.annotations = a->super.annotations;

	res->opt_type = NULL;
	if (a->opt_type != NULL) {
		res->opt_type = copy_type(a->opt_type);
	}

	res->expr = copy_expr(a->expr);
	res->lvalue = copy_lvalue(a->lvalue);

	return res;
}

struct RetStmt* copy_ret_stmt(struct RetStmt* r) {
	struct RetStmt* res = make(RetStmt);

	res->super.line_num = r->super.line_num;
	res->super.annotations = r->super.annotations;

	res->return_value = copy_expr(r->return_value);
	return res;
}

struct WhileStmt* copy_while_stmt(struct WhileStmt* w) {
	struct WhileStmt* res = make(WhileStmt);

	res->super.line_num = w->super.line_num;
	res->super.annotations = w->super.annotations;

	res->condition = copy_expr(w->condition);
	res->block = copy_stmt_block(w->block);

	return res;
}

struct ForStmt* copy_for_stmt(struct ForStmt* f) {
	struct ForStmt* res = make(ForStmt);

	res->super.line_num = f->super.line_num;
	res->super.annotations = f->super.annotations;

	asprintf(&(res->index_name), "%s", f->index_name);
	res->range = copy_range(f->range);
	res->block = copy_stmt_block(f->block);

	return res;
}
