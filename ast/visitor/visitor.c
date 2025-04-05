#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../ast.h"
#include "visitor.h"

#define ARG void* arg
static void visit_method_decl(struct MethodDecl* m, VISITOR, ARG);
static void visit_decl_arg(struct DeclArg* da, VISITOR, ARG);

//struct
static void visit_struct_member(struct StructMember* s, VISITOR, ARG);

//other

// @returns false on error
static bool visit_stmt(struct Stmt* s, VISITOR, ARG);

//stmts
// @returns false on error
static bool visit_if_stmt(struct IfStmt* i, VISITOR, ARG);
// @returns false on error
static bool visit_while_stmt(struct WhileStmt* w, VISITOR, ARG);
// @returns false on error
static bool visit_for_stmt(struct ForStmt* f, VISITOR, ARG);
// @returns false on error
static bool visit_assign_stmt(struct AssignStmt* a, VISITOR, ARG);
// @returns false on error
static bool visit_call(struct Call* m, VISITOR, ARG);
// @returns false on error
static bool visit_ret_stmt(struct RetStmt* r, VISITOR, ARG);
static void visit_local_var_decl_stmt(struct LocalVarDeclStmt* lvds, VISITOR, ARG);
static void visit_break_stmt(VISITOR, ARG);
static void visit_continue_stmt(VISITOR, ARG);

//expr
// @returns false on error
static bool visit_expr(struct Expr* e, VISITOR, ARG);
static void visit_op(enum OP o, VISITOR, ARG);
// @returns false on error
static bool visit_un_op_term(struct UnOpTerm* u, VISITOR, ARG);
// @returns false on error
static bool visit_address_of(struct AddressOf* u, VISITOR, ARG);
// @returns false on error
static bool visit_deref(struct Deref* d, VISITOR, ARG);
// @returns false on error
static bool visit_lvalue(struct LValue* lv, VISITOR, ARG);

// @returns false on error
static bool visit_term(struct Term* t, VISITOR, ARG);

//const
static void visit_const_value(struct ConstValue* cv, VISITOR, ARG);
static void visit_string_const(struct StringConst* s, VISITOR, ARG);

//var
// @returns false on error
static bool visit_variable(struct Variable* v, VISITOR, ARG);
// @returns false on error
static bool visit_simple_var(struct SimpleVar* v, VISITOR, ARG);

bool visit_ast(struct AST* ast, VISITOR, void* arg) {

	for (int i = 0; i < ast->count_namespaces; i++) {
		if (!visit_namespace(ast->namespaces[i], visitor, arg)) {
			return false;
		}
	}

	return true;
}

bool visit_namespace(struct Namespace* n, VISITOR, void* arg) {

	visitor(n, NODE_NAMESPACE, arg);

	//we do not visit the passthrough include declarations as they
	//are just a workaround for now

	for (int i = 0; i < n->count_structs; i++) {
		visit_struct_decl(n->structs[i], visitor, arg);
	}

	for (int i = 0; i < n->count_methods; i++) {
		if (!visit_method(n->methods[i], visitor, arg)) {
			return false;
		}
	}

	return true;
}

void visit_struct_decl(struct StructDecl* s, VISITOR, void* arg) {

	visitor(s, NODE_STRUCTDECL, arg);

	for (int i = 0; i < s->count_members; i++) {
		visit_struct_member(s->members[i], visitor, arg);
	}
}

bool visit_method(struct Method* m, VISITOR, void* arg) {

	visitor(m, NODE_METHOD, arg);

	visit_method_decl(m->decl, visitor, arg);
	return visit_stmt_block(m->block, visitor, arg);
}
static void visit_method_decl(struct MethodDecl* m, VISITOR, ARG) {

	visitor(m, NODE_METHOD_DECL, arg);

	for (uint32_t i = 0; i < m->count_args; i++) {
		visit_decl_arg(m->args[i], visitor, arg);
	}

	visit_type(m->return_type, visitor, arg);
}
bool visit_stmt_block(struct StmtBlock* s, VISITOR, void* arg) {

	visitor(s, NODE_STMTBLOCK, arg);

	for (int i = 0; i < s->count; i++) {
		if (!visit_stmt(s->stmts[i], visitor, arg)) {
			return false;
		}
	}

	return true;
}

//----------- static functions ---------------

static void visit_decl_arg(struct DeclArg* da, VISITOR, void* arg) {

	visitor(da, NODE_DECLARG, arg);

	visit_type(da->type, visitor, arg);
}

static void visit_struct_member(struct StructMember* s, VISITOR, void* arg) {

	visitor(s, NODE_STRUCTMEMBER, arg);
}

static bool visit_stmt(struct Stmt* s, VISITOR, void* arg) {

	visitor(s, NODE_STMT, arg);

	switch (s->kind) {

		case STMT_KIND_CALL:
			return visit_call(s->ptr.call, visitor, arg);
		case STMT_KIND_WHILE:
			return visit_while_stmt(s->ptr.while_stmt, visitor, arg);
		case STMT_KIND_IF:
			return visit_if_stmt(s->ptr.if_stmt, visitor, arg);
		case STMT_KIND_RETURN:
			return visit_ret_stmt(s->ptr.return_stmt, visitor, arg);
		case STMT_KIND_ASSIGN:
			return visit_assign_stmt(s->ptr.assign_stmt, visitor, arg);
		case STMT_KIND_FOR:
			return visit_for_stmt(s->ptr.for_stmt, visitor, arg);
		case STMT_KIND_LOCAL_VAR_DECL:
			visit_local_var_decl_stmt(s->ptr.local_var_decl_stmt, visitor, arg);
			break;
		case STMT_KIND_BREAK:
			visit_break_stmt(visitor, arg);
			break;
		case STMT_KIND_CONTINUE:
			visit_continue_stmt(visitor, arg);
			break;
		default:
			fprintf(stderr, "[Visitor] Fatal error in %s: kind == %d\n", __func__, s->kind);
			return false;
	}

	return true;
}

static bool visit_if_stmt(struct IfStmt* i, VISITOR, void* arg) {

	visitor(i, NODE_IFSTMT, arg);

	if (!visit_expr(i->condition, visitor, arg)) {
		return false;
	}
	if (!visit_stmt_block(i->block, visitor, arg)) {
		return false;
	}

	if (i->else_block != NULL) {
		if (!visit_stmt_block(i->else_block, visitor, arg)) {
			return false;
		}
	}

	return true;
}

static bool visit_while_stmt(struct WhileStmt* w, VISITOR, void* arg) {

	visitor(w, NODE_WHILESTMT, arg);

	return visit_stmt_block(w->block, visitor, arg);
}

static bool visit_for_stmt(struct ForStmt* f, VISITOR, void* arg) {

	visitor(f, NODE_FORSTMT, arg);

	return visit_stmt_block(f->block, visitor, arg);
}

static bool visit_assign_stmt(struct AssignStmt* a, VISITOR, void* arg) {

	visitor(a, NODE_ASSIGNSTMT, arg);

	if (a->opt_type != NULL) { visit_type(a->opt_type, visitor, arg); }

	if (!visit_lvalue(a->lvalue, visitor, arg)) {
		return false;
	}

	return visit_expr(a->expr, visitor, arg);
}

static bool visit_call(struct Call* m, VISITOR, void* arg) {

	visitor(m, NODE_CALL, arg);

	if (!visit_variable(m->callable, visitor, arg)) {
		return false;
	}

	for (int i = 0; i < m->count_args; i++) {
		if (!visit_expr(m->args[i], visitor, arg)) {
			return false;
		}
	}

	return true;
}

static bool visit_ret_stmt(struct RetStmt* r, VISITOR, void* arg) {

	visitor(r, NODE_RETSTMT, arg);

	return visit_expr(r->return_value, visitor, arg);
}

static void visit_local_var_decl_stmt(struct LocalVarDeclStmt* l, VISITOR, ARG) {

	visitor(l, NODE_LOCAL_VAR_DECL_STMT, arg);

	visit_type(l->type, visitor, arg);
}

static void visit_break_stmt(VISITOR, void* arg) {

	visitor(NULL, NODE_BREAKSTMT, arg);
}

static void visit_continue_stmt(VISITOR, void* arg) {

	visitor(NULL, NODE_CONTINUESTMT, arg);
}

static bool visit_expr(struct Expr* e, VISITOR, void* arg) {

	visitor(e, NODE_EXPR, arg);

	if (!visit_un_op_term(e->term1, visitor, arg)) {
		return false;
	}

	if (e->op == OP_NONE) { return true; }

	visit_op(e->op, visitor, arg);
	return visit_un_op_term(e->term2, visitor, arg);
}

static void visit_op(enum OP o, VISITOR, void* arg) {

	visitor(&o, NODE_OP, arg);
}

static bool visit_un_op_term(struct UnOpTerm* u, VISITOR, void* arg) {

	visitor(u, NODE_UNOPTERM, arg);

	if (u->address_of) {
		return visit_address_of(u->address_of, visitor, arg);
	}
	if (u->deref) {
		return visit_deref(u->deref, visitor, arg);
	}

	visit_op(u->op, visitor, arg);

	return visit_term(u->term, visitor, arg);
}

static bool visit_address_of(struct AddressOf* ao, VISITOR, ARG) {

	visitor(ao, NODE_ADDRESSOF, arg);

	return visit_term(ao->term, visitor, arg);
}

static bool visit_deref(struct Deref* d, VISITOR, ARG) {

	visitor(d, NODE_DEREF, arg);

	return visit_term(d->term, visitor, arg);
}

static bool visit_lvalue(struct LValue* lv, VISITOR, ARG) {

	visitor(lv, NODE_LVALUE, arg);

	if (lv->var) {
		return visit_variable(lv->var, visitor, arg);
	}
	if (lv->deref) {
		return visit_deref(lv->deref, visitor, arg);
	}
	return false;
}

static bool visit_term(struct Term* t, VISITOR, void* arg) {

	visitor(t, NODE_TERM, arg);

	switch (t->kind) {
		case 4:
			return visit_call(t->ptr.m4, visitor, arg);
		case 5:
			return visit_expr(t->ptr.m5, visitor, arg);
		case 6:
			return visit_variable(t->ptr.m6, visitor, arg);
		case 8:
			visit_string_const(t->ptr.m8, visitor, arg);
			break;
		case 12:
			visit_const_value(t->ptr.m12, visitor, arg);
			break;
		default:
			fprintf(stderr, "[Visitor][Error] Fatal(2)\n");
			return false;
			break;
	}

	return true;
}

static void visit_const_value(struct ConstValue* cv, VISITOR, void* arg) {

	visitor(cv, NODE_CONSTVALUE, arg);
}

static void visit_string_const(struct StringConst* s, VISITOR, void* arg) {

	visitor(s, NODE_STRINGCONST, arg);
}

static bool visit_variable(struct Variable* v, VISITOR, void* arg) {

	visitor(v, NODE_VARIABLE, arg);

	if (!visit_simple_var(v->simple_var, visitor, arg)) {
		return false;
	}

	if (v->member_access != NULL) {
		if (!visit_variable(v->member_access, visitor, arg)) {
			return false;
		}
	}

	return true;
}

static bool visit_simple_var(struct SimpleVar* v, VISITOR, void* arg) {

	visitor(v, NODE_SIMPLEVAR, arg);

	for (int i = 0; i < v->count_indices; i++) {
		if (!visit_expr(v->indices[i], visitor, arg)) {
			return false;
		}
	}

	return true;
}

void visit_type(struct Type* t, VISITOR, void* arg) {

	visitor(t, NODE_TYPE, arg);

	if (t->basic_type != NULL) { visit_basic_type(t->basic_type, visitor, arg); }

	if (t->type_param != NULL) { visit_type_param(t->type_param, visitor, arg); }

	if (t->array_type != NULL) { visit_array_type(t->array_type, visitor, arg); }

	if (t->pointer_type != NULL) { visit_pointer_type(t->pointer_type, visitor, arg); }
}

void visit_array_type(struct ArrayType* a, VISITOR, void* arg) {

	visitor(a, NODE_ARRAYTYPE, arg);

	visit_type(a->element_type, visitor, arg);
}

void visit_pointer_type(struct PointerType* ptt, VISITOR, ARG) {

	visitor(ptt, NODE_POINTERTYPE, arg);

	visit_type(ptt->element_type, visitor, arg);
}

void visit_subr_type(struct SubrType* s, VISITOR, void* arg) {

	visitor(s, NODE_SUBRTYPE, arg);

	visit_type(s->return_type, visitor, arg);

	for (int i = 0; i < s->count_arg_types; i++) {
		visit_type(s->arg_types[i], visitor, arg);
	}
}

void visit_basic_type(struct BasicType* b, VISITOR, void* arg) {

	visitor(b, NODE_BASICTYPE, arg);

	if (b->simple_type != NULL) { visit_simple_type(b->simple_type, visitor, arg); }

	if (b->subr_type != NULL) { visit_subr_type(b->subr_type, visitor, arg); }
}

void visit_simple_type(struct SimpleType* s, VISITOR, void* arg) {

	visitor(s, NODE_SIMPLETYPE, arg);

	if (s->primitive_type != NULL) { visit_primitive_type(s->primitive_type, visitor, arg); }

	if (s->struct_type != NULL) { visit_struct_type(s->struct_type, visitor, arg); }
}

void visit_primitive_type(struct PrimitiveType* p, VISITOR, void* arg) {

	visitor(p, NODE_PRIMITIVETYPE, arg);
}

void visit_struct_type(struct StructType* s, VISITOR, void* arg) {

	visitor(s, NODE_STRUCTTYPE, arg);
}

void visit_type_param(struct TypeParam* t, VISITOR, void* arg) {

	visitor(t, NODE_TYPEPARAM, arg);
}
