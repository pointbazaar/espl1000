#include <stdio.h>
#include <stdlib.h>

#include "../ast.h"
#include "visitor.h"

#define ARG void* arg
static void visit_method_decl	(struct MethodDecl* m, VISITOR, ARG);
static void visit_decl_arg        (struct DeclArg* da, VISITOR, ARG);

//struct
static void visit_struct_member	(struct StructMember* s, VISITOR, ARG);

//other
static void visit_stmt        	(struct Stmt* s, VISITOR, ARG);
static void visit_lambda		(struct Lambda* l, VISITOR, ARG);

//stmts
static void visit_if_stmt      	(struct IfStmt* i, VISITOR, ARG);
static void visit_while_stmt   	(struct WhileStmt* w, VISITOR, ARG);
static void visit_for_stmt     	(struct ForStmt* f, VISITOR, ARG);
static void visit_assign_stmt  	(struct AssignStmt* a, VISITOR, ARG);
static void visit_switch_stmt  	(struct SwitchStmt* s, VISITOR, ARG);
static void visit_case_stmt    	(struct CaseStmt* c, VISITOR, ARG);
static void visit_call           (struct Call* m, VISITOR, ARG);
static void visit_ret_stmt     	(struct RetStmt* r, VISITOR, ARG);
static void visit_massign_stmt  (struct MAssignStmt* m, VISITOR, ARG);
static void visit_break_stmt   	(VISITOR, ARG);
static void visit_continue_stmt	(VISITOR, ARG);

//expr
static void visit_expr        	(struct Expr* e, VISITOR, ARG);
static void visit_op          	(struct Op* o, VISITOR, ARG);
static void visit_un_op_term    (struct UnOpTerm* u, VISITOR, ARG);
static void visit_term       	(struct Term* t, VISITOR, ARG);
static void visit_mdirect       (struct MDirect* m, VISITOR, ARG);

//const
static void visit_bool_const   	(struct BoolConst* b, VISITOR, ARG);
static void visit_int_const    	(struct IntConst* i, VISITOR, ARG);
static void visit_char_const   	(struct CharConst* c, VISITOR, ARG);
static void visit_float_const  	(struct FloatConst* f, VISITOR, ARG);
static void visit_hex_const    	(struct HexConst* h, VISITOR, ARG);
static void visit_bin_const    	(struct BinConst* b, VISITOR, ARG);
static void visit_const_value   (struct ConstValue* cv, VISITOR, ARG);
static void visit_string_const 	(struct StringConst* s, VISITOR, ARG);

//var
static void visit_variable    	(struct Variable* v, VISITOR, ARG);
static void visit_simple_var   	(struct SimpleVar* v, VISITOR, ARG);


void visit_ast(struct AST* ast, VISITOR, void* arg){

	for(int i = 0; i < ast->count_namespaces; i++)
		{ visit_namespace(ast->namespaces[i], visitor, arg); }
}

void visit_namespace(struct Namespace* n, VISITOR, void* arg){

	visitor(n, NODE_NAMESPACE, arg);

	//we do not visit the passthrough include declarations as they
	//are just a workaround for now

	for(int i = 0; i < n->count_structs; i++)
		{ visit_struct_decl(n->structs[i], visitor, arg); }
	
	for(int i = 0; i < n->count_methods; i++)
		{ visit_method(n->methods[i], visitor, arg); }
}

void visit_struct_decl(struct StructDecl* s, VISITOR, void* arg){
	
	visitor(s, NODE_STRUCTDECL, arg);
	
	for(int i = 0; i < s->count_members; i++)
		{ visit_struct_member(s->members[i], visitor, arg); }
}

void visit_method(struct Method* m, VISITOR, void* arg){
	
	visitor(m, NODE_METHOD, arg);

	visit_method_decl(m->decl, visitor, arg);
	visit_stmt_block(m->block, visitor, arg);
}
static void visit_method_decl(struct MethodDecl* m, VISITOR, ARG){

	visitor(m, NODE_METHOD_DECL, arg);

	for(uint32_t i = 0; i < m->count_args; i++){
		visit_decl_arg(m->args[i], visitor, arg);
	}

	visit_type(m->return_type, visitor, arg);
}
void visit_stmt_block(struct StmtBlock* s, VISITOR, void* arg){
	
	visitor(s, NODE_STMTBLOCK, arg);
	
	for(int i = 0; i < s->count; i++)
		{ visit_stmt(s->stmts[i], visitor, arg); }
}

//----------- static functions ---------------

static void visit_decl_arg(struct DeclArg* da, VISITOR, void* arg){
	
	visitor(da, NODE_DECLARG, arg);

	visit_type(da->type, visitor, arg);
}

static void visit_struct_member(struct StructMember* s, VISITOR, void* arg){
	
	visitor(s, NODE_STRUCTMEMBER, arg);
}

static void visit_stmt(struct Stmt* s, VISITOR, void* arg){
	
	visitor(s, NODE_STMT, arg);
	
	switch(s->kind){

		case 1:
			visit_call(s->ptr.m1, visitor, arg);   break;
		case 2:
			visit_while_stmt(s->ptr.m2, visitor, arg);   break;
		case 3:
			visit_if_stmt(s->ptr.m3, visitor, arg);   break;
		case 4:
			visit_ret_stmt(s->ptr.m4, visitor, arg);   break;
		case 5:
			visit_assign_stmt(s->ptr.m5, visitor, arg);   break;
		case 7:
			visit_for_stmt(s->ptr.m7, visitor, arg);   break;
		case 8:
			visit_switch_stmt(s->ptr.m8, visitor, arg);   break;
        case 9:
            visit_massign_stmt(s->ptr.m9, visitor, arg); break;
		case 99:
			if(s->is_break)   { visit_break_stmt(visitor, arg); }
			if(s->is_continue){ visit_continue_stmt(visitor, arg); }
			break;
		
		default: 
			printf("[Visitor] Fatal (1)\n"); exit(1);
			break;
	}
}

static void visit_lambda (struct Lambda* l, VISITOR, void* arg){

	visitor(l, NODE_LAMBDA, arg);

	visit_expr(l->expr, visitor, arg);
}

static void visit_if_stmt(struct IfStmt* i, VISITOR, void* arg){
	
	visitor(i, NODE_IFSTMT, arg);

	visit_expr(i->condition, visitor, arg);
	visit_stmt_block(i->block, visitor, arg);
	
	if(i->else_block != NULL)
		{ visit_stmt_block(i->else_block, visitor, arg); }
}

static void visit_while_stmt(struct WhileStmt* w, VISITOR, void* arg){
	
	visitor(w, NODE_WHILESTMT, arg);

	visit_stmt_block(w->block, visitor, arg);
}

static void visit_for_stmt(struct ForStmt* f, VISITOR, void* arg){
	
	visitor(f, NODE_FORSTMT, arg);

	visit_stmt_block(f->block, visitor, arg);
}

static void visit_assign_stmt(struct AssignStmt* a, VISITOR, void* arg){
	
	visitor(a, NODE_ASSIGNSTMT, arg);
	
	if(a->opt_type != NULL)
		{ visit_type(a->opt_type, visitor, arg); }

	visit_variable(a->var, visitor, arg);

	visit_expr(a->expr, visitor, arg);
}

static void visit_switch_stmt(struct SwitchStmt* s, VISITOR, void* arg){
	
	visitor(s, NODE_SWITCHSTMT, arg);

	visit_expr(s->expr, visitor, arg);
	
	for(int i = 0; i < s->count_cases; i++)
		{ visit_case_stmt(s->cases[i], visitor, arg); }
}

static void visit_case_stmt(struct CaseStmt* c, VISITOR, void* arg){
	
	visitor(c, NODE_CASESTMT, arg);

	visit_const_value(c->const_value, visitor, arg);
	
	if(c->block != NULL)
		{ visit_stmt_block(c->block, visitor, arg); }
}

static void visit_call(struct Call* m, VISITOR, void* arg){
	
	visitor(m, NODE_CALL, arg);

    visit_variable(m->callable, visitor, arg);
	
	for(int i = 0; i < m->count_args; i++)
		{ visit_expr(m->args[i], visitor, arg); }
}

static void visit_ret_stmt(struct RetStmt* r, VISITOR, void* arg){
	
	visitor(r, NODE_RETSTMT, arg);

	visit_expr(r->return_value, visitor, arg);
}

static void visit_massign_stmt(struct MAssignStmt* m, VISITOR, ARG){

    visitor(m, NODE_MASSIGNSTMT, arg);

    visit_mdirect(m->lhs, visitor, arg);
    visit_expr(m->expr, visitor, arg);
}

static void visit_break_stmt(VISITOR, void* arg){
	
	visitor(NULL, NODE_BREAKSTMT, arg);
}

static void visit_continue_stmt(VISITOR, void* arg){
	
	visitor(NULL, NODE_CONTINUESTMT, arg);
}

static void visit_expr(struct Expr* e, VISITOR, void* arg){
	
	visitor(e, NODE_EXPR, arg);

	visit_un_op_term(e->term1, visitor, arg);
	
	if(e->op == NULL)
		{ return; }

	visit_op(e->op, visitor, arg);
	visit_un_op_term(e->term2, visitor, arg);
}

static void visit_op(struct Op* o, VISITOR, void* arg){
	
	visitor(o, NODE_OP, arg);
}

static void visit_un_op_term(struct UnOpTerm* u, VISITOR, void* arg){
	
	visitor(u, NODE_UNOPTERM, arg);

	visit_op(u->op, visitor, arg);
	visit_term(u->term, visitor, arg);
}

static void visit_term(struct Term* t, VISITOR, void* arg){
	
	visitor(t, NODE_TERM, arg);
	
	switch(t->kind){
		case 4:
			visit_call(t->ptr.m4, visitor, arg);       break;
		case 5:
			visit_expr(t->ptr.m5, visitor, arg);       break;
		case 6:
			visit_variable(t->ptr.m6, visitor, arg);   break;
		case 8:
			visit_string_const(t->ptr.m8, visitor, arg);break;
		case 11:
			visit_lambda(t->ptr.m11, visitor, arg);   break;
		case 12:
			visit_const_value(t->ptr.m12, visitor, arg); break;
        case 13:
            visit_mdirect(t->ptr.m13, visitor, arg); break;
		default:
			printf("[Visitor][Error] Fatal(2)\n");
			exit(1);
			break;
	}
}

static void visit_mdirect(struct MDirect* m, VISITOR, ARG){

    visitor(m, NODE_MDIRECT, arg);

    visit_expr(m->expr, visitor, arg);
}

static void visit_bool_const(struct BoolConst* b, VISITOR, void* arg){
	
	visitor(b, NODE_BOOLCONST, arg);
}

static void visit_int_const(struct IntConst* i, VISITOR, void* arg){
	
	visitor(i, NODE_INTCONST, arg);
}

static void visit_char_const(struct CharConst* c, VISITOR, void* arg){
	
	visitor(c, NODE_CHARCONST, arg);
}

static void visit_float_const(struct FloatConst* f, VISITOR, void* arg){
	
	visitor(f, NODE_FLOATCONST, arg);
}

static void visit_hex_const(struct HexConst* h, VISITOR, void* arg){
	
	visitor(h, NODE_HEXCONST, arg);
}

static void visit_bin_const(struct BinConst* b, VISITOR, void* arg){
	
	visitor(b, NODE_BINCONST, arg);
}

static void visit_const_value(struct ConstValue* cv, VISITOR, void* arg){

	switch (cv->kind) {
		case 1: visit_bool_const(cv->ptr.m1_bool_const, visitor, arg);
		case 2: visit_int_const(cv->ptr.m2_int_const, visitor, arg);
		case 3: visit_char_const(cv->ptr.m3_char_const, visitor, arg);
		case 4: visit_float_const(cv->ptr.m4_float_const, visitor, arg);
		case 5: visit_hex_const(cv->ptr.m5_hex_const, visitor, arg);
		case 6: visit_bin_const(cv->ptr.m6_bin_const, visitor, arg);
	}

	visitor(cv, NODE_CONSTVALUE, arg);
}

static void visit_string_const(struct StringConst* s, VISITOR, void* arg){
	
	visitor(s, NODE_STRINGCONST, arg);
}

static void visit_variable(struct Variable* v, VISITOR, void* arg){
	
	visitor(v, NODE_VARIABLE, arg);

	visit_simple_var(v->simple_var, visitor, arg);
	
	if(v->member_access != NULL)
		{ visit_variable(v->member_access, visitor, arg); }
}

static void visit_simple_var(struct SimpleVar* v, VISITOR, void* arg){
	
	visitor(v, NODE_SIMPLEVAR, arg);
	
	for(int i = 0; i < v->count_indices; i++)
		{ visit_expr(v->indices[i], visitor, arg); }
}

void visit_type(struct Type* t, VISITOR, void* arg){
		
	visitor(t, NODE_TYPE, arg);
	
	if(t->basic_type != NULL)
		{ visit_basic_type(t->basic_type, visitor, arg); }
		
	if(t->type_param != NULL)
		{ visit_type_param(t->type_param, visitor, arg); }
		
	if(t->array_type != NULL)
		{ visit_array_type(t->array_type, visitor, arg); }
}

void visit_array_type(struct ArrayType* a, VISITOR, void* arg){
		
	visitor(a, NODE_ARRAYTYPE, arg);

	visit_type(a->element_type, visitor, arg);
}

void visit_subr_type(struct SubrType* s, VISITOR, void* arg){
		
	visitor(s, NODE_SUBRTYPE, arg);

	visit_type(s->return_type, visitor, arg);
	
	for(int i = 0; i < s->count_arg_types; i++)
		{ visit_type(s->arg_types[i], visitor, arg); }
}

void visit_basic_type(struct BasicType* b, VISITOR, void* arg){
	
	visitor(b, NODE_BASICTYPE, arg);
	
	if(b->simple_type != NULL)
		{ visit_simple_type(b->simple_type, visitor, arg); }
		
	if(b->subr_type != NULL)
		{ visit_subr_type(b->subr_type, visitor, arg); }
}

void visit_simple_type(struct SimpleType* s, VISITOR, void* arg){
		
	visitor(s, NODE_SIMPLETYPE, arg);
	
	if(s->primitive_type != NULL)
		{ visit_primitive_type(s->primitive_type, visitor, arg); }
		
	if(s->struct_type != NULL)
		{ visit_struct_type(s->struct_type, visitor, arg); }
}

void visit_primitive_type(struct PrimitiveType* p, VISITOR, void* arg){
		
	visitor(p, NODE_PRIMITIVETYPE, arg);
}

void visit_struct_type(struct StructType* s, VISITOR, void* arg){
		
	visitor(s, NODE_STRUCTTYPE, arg);

	for(uint32_t i = 0; i < s->count_type_params; i++){
        visit_type(s->type_params[i], visitor, arg);
	}
}

void visit_type_param(struct TypeParam* t, VISITOR, void* arg){
		
	visitor(t, NODE_TYPEPARAM, arg);
}

