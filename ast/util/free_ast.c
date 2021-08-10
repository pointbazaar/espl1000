#include <stdlib.h>
#include <stdio.h>

#include "free_ast.h"

//this file also contains static asserts
//about the sizes of AST structures.
//this is because any program which uses these
//AST structs has to also free them.

void free_ast(struct AST* ast) {

	for(int i=0; i < ast->count_namespaces; i++) {
		free_namespace(ast->namespaces[i]);
	}
	free(ast->namespaces);
	free(ast);
}

void free_bool_const(struct BoolConst* bc) {
	
	free(bc);
}

void free_char_const(struct CharConst* cc) {
	
	free(cc);
}

void free_decl_arg(struct DeclArg* da) {

	free_type(da->type);
	free(da);
}


void free_expr(struct Expr* expr) {

	free_un_op_term(expr->term1);
	if(expr->op != NULL) {
		free_op(expr->op);
		free_un_op_term(expr->term2);
	}
	
	free(expr);
}


void free_float_const(struct FloatConst* fc) { free(fc); }
void free_identifier(struct Id* id) { free(id); }
void free_int_const(struct IntConst* ic) { free(ic); }
void free_hex_const(struct HexConst* hc){ free(hc); }
void free_bin_const(struct BinConst* hc){ free(hc); }
void free_const_value(struct ConstValue* cv){
	switch (cv->kind) {
		case 1: free_bool_const(cv->ptr.m1_bool_const); break;
		case 2: free_int_const(cv->ptr.m2_int_const); break;
		case 3: free_char_const(cv->ptr.m3_char_const); break;
		case 4: free_float_const(cv->ptr.m4_float_const); break;
		case 5: free_hex_const(cv->ptr.m5_hex_const); break;
		case 6: free_bin_const(cv->ptr.m6_bin_const); break;
	}
	free(cv);
}
void free_method(struct Method* m) {
	free_method_decl(m->decl);
	free_stmt_block(m->block);
	free(m);
}
void free_method_decl(struct MethodDecl* m){
	free_type(m->return_type);
	for(int i=0; i < m->count_args; i++) {
		free_decl_arg(m->args[i]);
	}
	free(m->args);
	free(m);
}
void free_externc(struct ExternC* ec){
	if (ec->subr_decl != NULL){ free_method_decl(ec->subr_decl); }
	if (ec->struct_decl != NULL){ free_struct_decl(ec->struct_decl); }
	free(ec);
}
void free_namespace(struct Namespace* ns) {

	for(int i=0; i < ns->count_includes; i++) {
		free(ns->includes[i]);
	}

	for(int i=0; i < ns->count_externc; i++) {
		free_externc(ns->externc[i]);
	}

	for(int i=0; i < ns->count_methods; i++) {
		free_method(ns->methods[i]);
	}

	for(int i=0; i < ns->count_structs; i++) {
		free_struct_decl(ns->structs[i]);
	}

	free(ns->includes);

	free(ns->methods);
	free(ns->structs);
	free(ns->externc);
	
	free(ns->src_path);
	free(ns->ast_path);
	free(ns->token_path);

	free(ns);
}

void free_simple_var(struct SimpleVar* sv) {

	for(int i=0;i < sv->count_indices; i++){
		free_expr(sv->indices[i]);
	}
	free(sv->indices);
	free(sv);
}

void free_stmt_block(struct StmtBlock* block) {

	for(int i=0; i < block->count; i++) {
		free_stmt(block->stmts[i]);
	}
	free(block->stmts);
	free(block);
}

void free_struct_decl(struct StructDecl* sd) {

	free_simple_type(sd->type);
	for(int i=0; i < sd->count_members; i++) {
		free_struct_member(sd->members[i]);
	}
	free(sd->members);
	free(sd);
}

void free_struct_member(struct StructMember* sm) {
	free_type(sm->type);
	free(sm);
}
void free_term(struct Term* t) {

	switch(t->kind){
		case  4: free_call(t->ptr.m4); 	break;
		case  5: free_expr(t->ptr.m5); 	break;
		case  6: free_variable(t->ptr.m6); 	break;
		case  8: free_string_const(t->ptr.m8); 	break;
		case 11: free_lambda(t->ptr.m11);   break;
		case 12: free_const_value(t->ptr.m12); break;
		default:
			printf("Error in free_term(...)\n");
			free(t);
			exit(1);
	}
	free(t);
}
void free_un_op_term(struct UnOpTerm* t) {

	free_op(t->op);
	free_term(t->term);
	free(t);
}

void free_variable(struct Variable* var) {

	free_simple_var(var->simple_var);

	if(var->member_access != NULL){

		free_variable(var->member_access);
	}

	free(var);
}

void free_assign_stmt(struct AssignStmt* as) {

	if(as->opt_type != NULL) {
		free_type(as->opt_type);
	}
	free_variable(as->var);
	free_expr(as->expr);

	free(as);
}


void free_if_stmt(struct IfStmt* is) {

	free_expr(is->condition);

	free_stmt_block(is->block);
	if(is->else_block != NULL) {
		free_stmt_block(is->else_block);
	}

	free(is);
}

void free_call(struct Call* mc) {

	for(int i=0; i < mc->count_args; i++) {
		free_expr(mc->args[i]);
	}
	free(mc->args);

	free(mc);
}

void free_ret_stmt(struct RetStmt* rs) {

	if(rs->return_value != NULL) {
		free_expr(rs->return_value);
	}
	free(rs);
}

void free_stmt(struct Stmt* s) {

	switch(s->kind){
		
		case 99: /* nothing to do here */  break;
		case 1:
			free_call(s->ptr.m1);       break;
		case 2:
			free_while_stmt(s->ptr.m2);  break;
		case 3:
			free_if_stmt(s->ptr.m3);     break;
		case 4:
			free_ret_stmt(s->ptr.m4);    break;
		case 5:
			free_assign_stmt(s->ptr.m5); break;
		case 6:
			free_try_catch_stmt(s->ptr.m6); break;
		case 7:
			free_for_stmt(s->ptr.m7);    break;
		case 8:
			free_switch_stmt(s->ptr.m8); break;
		default:
			printf("Error in free_stmt\n");
			free(s);
			exit(1);
	}
	free(s);
}


void free_while_stmt(struct WhileStmt* ws) {
	free_expr(ws->condition);
	free_stmt_block(ws->block);
	free(ws);
}

void free_array_type(struct ArrayType* at) {
	free_type(at->element_type);
	free(at);
}

void free_basic_type(struct BasicType* btw) {

	if(btw->simple_type != NULL) {
		free_simple_type(btw->simple_type);

	} else if(btw->subr_type != NULL) {
		free_subr_type(btw->subr_type);
	}

	free(btw);
}

void free_simple_type(struct SimpleType* st) {
	
	if(st->primitive_type != NULL)
		{ free_primitive_type(st->primitive_type); }
		
	if(st->struct_type != NULL)
		{ free_struct_type(st->struct_type); }
	
	free(st);
}

void free_subr_type(struct SubrType* st) {

	free_type(st->return_type);
	for(int i=0; i < st->count_arg_types; i++) {
		free_type(st->arg_types[i]);
	}
	free(st->arg_types);
	free(st);
}

void free_type(struct Type* t) {

	if(t->basic_type != NULL) {
		free_basic_type(t->basic_type);
	} else if(t->type_param != NULL) {
		free_type_param(t->type_param);
	} else if(t->array_type != NULL) {
		free_array_type(t->array_type);
	}
	free(t);
}

void free_type_param(struct TypeParam* tp) { free(tp); }

void free_primitive_type(struct PrimitiveType* p){ free(p); }

void free_struct_type(struct StructType* s){

    for(int i=0; i < s->count_type_params; i++){
        free_type(s->type_params[i]);
    }
	if (s->count_type_params > 0){
		free(s->type_params);
	}

	free(s);
}

void free_op(struct Op* op){ free(op); }

void free_string_const(struct StringConst* s){
	free(s->value);
	free(s);
}

void free_range(struct Range* range){
	free_expr(range->start);
	free_expr(range->end);
	free(range);
}

void free_lambda(struct Lambda* l){
	for(uint8_t i = 0; i < l->count_params; i++){
		free_identifier(l->params[i]);
	}
	free_expr(l->expr);
	free(l);
}

void free_for_stmt(struct ForStmt* fstmt){
	free_range(fstmt->range);
	free_stmt_block(fstmt->block);
	free(fstmt);
}

void free_switch_stmt(struct SwitchStmt* sstmt){

	free_expr(sstmt->expr);
	for(int i=0; i < sstmt->count_cases; i++){
		free_case_stmt(sstmt->cases[i]);
	}
	free(sstmt->cases);
	free(sstmt);
}

void free_case_stmt(struct CaseStmt* cstmt){

	free_const_value(cstmt->const_value);

	if(cstmt->block != NULL){
		free_stmt_block(cstmt->block);
	}

	free(cstmt);
}

void free_try_catch_stmt(struct TryCatchStmt* tcs){

	free_stmt_block(tcs->try_block);
	free_stmt_block(tcs->catch_block);
	free(tcs);
}
