#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../ast.h"
#include "copy_ast.h"

struct BoolConst* copy_bool_const(struct BoolConst* bc){
	
	struct BoolConst* res = make(BoolConst);
	res->value = bc->value;
	return res;
}

struct CharConst* copy_char_const(struct CharConst* cc){
	
	struct CharConst* res = make(CharConst);
	res->value = cc->value;
	return res;
}

struct Expr* copy_expr(struct Expr* expr){
	
	struct Expr* res = make(Expr);
	res->op = NULL;
	res->term2 = NULL;
	
	res->term1 = copy_un_op_term(expr->term1);
	if(res->op != NULL){
		res->op = copy_op(expr->op);
		res->term2 = copy_un_op_term(expr->term2);
	}
	return res;
}

struct FloatConst* copy_float_const(struct FloatConst* fc){
	
	struct FloatConst* res = make(FloatConst);
	res->value = fc->value;
	return res;
}

struct Id* copy_identifier(struct Id* id){
	
	struct Id* res = make(Id);
	strcpy(res->identifier, id->identifier);
	return res;
}

struct Range* copy_range(struct Range* r){
    struct Range* res = make(Range);

    res->super.annotations = r->super.annotations;
    res->super.line_num = r->super.line_num;

    res->start = copy_expr(r->start);
    res->end = copy_expr(r->end);
    return res;
}

struct StmtBlock*  copy_stmt_block(struct StmtBlock* s){

	struct StmtBlock* res = make(StmtBlock);

	res->super.annotations = s->super.annotations;
	res->super.line_num = s->super.line_num;

	res->count = s->count;
	res->stmts = malloc(sizeof (void*)*res->count);
	for (int i = 0; i < res->count; ++i) {
		res->stmts[i] = copy_stmt(s->stmts[i]);
	}
	return res;
}
struct Lambda* copy_lambda(struct Lambda* l){

	struct Lambda* res = make(Lambda);
	res->count_params = l->count_params;
	for(uint8_t i = 0; i < l->count_params; i++){
		res->params[i] = copy_identifier(l->params[i]);
	}
	
	res->expr = copy_expr(l->expr);
	return res;
}

struct IntConst* copy_int_const(struct IntConst* ic){
	
	struct IntConst* res = make(IntConst);
	res->value = ic->value;
	return res;
}

struct HexConst* copy_hex_const(struct HexConst* hc){
	
	struct HexConst* res = make(HexConst);
	res->value = hc->value;
	return res;
}

struct BinConst* copy_bin_const(struct BinConst* hc){
	
	struct BinConst* res = make(BinConst);
	res->value = hc->value;
	return res;
}
struct ConstValue*  copy_const_value(struct ConstValue* cv){

	struct ConstValue* res = make(ConstValue);
	res->kind = cv->kind;

	switch (res->kind) {
		case 1: res->ptr.m1_bool_const = copy_bool_const(cv->ptr.m1_bool_const); break;
		case 2: res->ptr.m2_int_const = copy_int_const(cv->ptr.m2_int_const); break;
		case 3: res->ptr.m3_char_const = copy_char_const(cv->ptr.m3_char_const); break;
		case 4: res->ptr.m4_float_const = copy_float_const(cv->ptr.m4_float_const); break;
		case 5: res->ptr.m5_hex_const= copy_hex_const(cv->ptr.m5_hex_const); break;
		case 6: res->ptr.m6_bin_const = copy_bin_const(cv->ptr.m6_bin_const); break;
	}

	res->super.line_num = cv->super.line_num;
	res->super.annotations = cv->super.annotations;

	return res;
}
struct SimpleVar* copy_simple_var(struct SimpleVar* sv){
	
	struct SimpleVar* res = make(SimpleVar);
	
	strcpy(res->name, sv->name);
	res->count_indices = sv->count_indices;
	res->indices = malloc(sizeof(struct Expr*)*res->count_indices);
	
	for(int i = 0;i < res->count_indices; i++){
		res->indices[i] = copy_expr(sv->indices[i]);
	}
	
	return res;
}

struct Term* copy_term(struct Term* t){
	
	struct Term* res = make(Term);
	res->kind = t->kind;
	
	switch(t->kind){

		case  4: res->ptr.m4 = copy_call(t->ptr.m4);        break;
		case  5: res->ptr.m5 = copy_expr(t->ptr.m5);        break;
		case  6: res->ptr.m6 = copy_variable(t->ptr.m6);    break;
		case  8: res->ptr.m8 = copy_string_const(t->ptr.m8); break;
		case 11: res->ptr.m11 = copy_lambda(t->ptr.m11); break;
		case 12: res->ptr.m12 = copy_const_value(t->ptr.m12); break;
		default:
			printf("[AST][Error] copy_term(...), kind was: %d\n", t->kind);
			exit(1);
	}
	
	return res;
}

struct UnOpTerm* copy_un_op_term(struct UnOpTerm* t){
	
	struct UnOpTerm* res = make(UnOpTerm);
	
	res->op = NULL;
	
	if(t->op != NULL){ res->op = copy_op(t->op); }
	
	res->term = copy_term(t->term);
	
	return res;
}

struct Variable* copy_variable(struct Variable* var){

	struct Variable* res = make(Variable);
	
	res->simple_var = copy_simple_var(var->simple_var);

	res->member_access = NULL;

	if(var->member_access != NULL){

		res->member_access = copy_variable(var->member_access);
	}
	
	return res;
}

struct Op* copy_op(struct Op* op){
	
	struct Op* res = make(Op);
	strcpy(res->op, op->op);
	return res;
}

struct StringConst* copy_string_const(struct StringConst* s){
	
	struct StringConst* res = make(StringConst);
	res->value = malloc(sizeof(char)*(strlen(s->value)+1));
	strcpy(res->value, s->value);
	return res;
}

struct Type* copy_type(struct Type* t){
	
	struct Type* res = make(Type);
	res->m1 = NULL;
	res->m2 = NULL;
	res->m3 = NULL;
	
	if(t->m1 != NULL){ res->m1 = copy_basic_type(t->m1); }
	if(t->m2 != NULL){ res->m2 = copy_type_param(t->m2); }
	if(t->m3 != NULL){ res->m3 = copy_array_type(t->m3); }
	
	return res;
}

struct TypeParam* copy_type_param(struct TypeParam* t){
	
	struct TypeParam* res = make(TypeParam);
	
	res->index = t->index;
	
	return res;
}

struct SubrType* copy_subr_type(struct SubrType* s){
	
	struct SubrType* res = make(SubrType);
	
	res->has_side_effects = s->has_side_effects;
	res->throws         = s->throws;
	res->return_type = copy_type(s->return_type);
	
	res->count_arg_types = s->count_arg_types;
	
	res->arg_types = malloc(sizeof(struct Type*) * (s->count_arg_types));
	
	for(uint8_t i = 0; i < res->count_arg_types; i++){
	
		res->arg_types[i] = copy_type(s->arg_types[i]);
	}
	
	return res;
}

struct SimpleType* copy_simple_type(struct SimpleType* s){
	
	struct SimpleType* res = make(SimpleType);
	
	res->primitive_type = NULL;
	res->struct_type    = NULL;
	
	if(s->primitive_type != NULL){ res->primitive_type = copy_primitive_type(s->primitive_type); }
	if(s->struct_type != NULL){ res->struct_type    = copy_struct_type(s->struct_type); }
	
	return res;
}

struct StructType* copy_struct_type(struct StructType* s){
	
	struct StructType* res = make(StructType);
	
	strcpy(res->type_name, s->type_name);
	
	res->count_type_params = s->count_type_params;
	
	res->type_params = malloc(sizeof(void*) * (s->count_type_params));

	for(int i=0; i < res->count_type_params; i++){
	    res->type_params[i] = copy_type(s->type_params[i]);
	}

	return res;
}

struct PrimitiveType* copy_primitive_type(struct PrimitiveType* p){
	
	struct PrimitiveType* res = make(PrimitiveType);
	
	memcpy(res, p, sizeof(struct PrimitiveType));
	
	return res;
	
}

struct BasicType* copy_basic_type(struct BasicType* b){

	struct BasicType* res = make(BasicType);
	
	res->simple_type = NULL;
	res->subr_type   = NULL;
	
	if(b->simple_type != NULL){ res->simple_type = copy_simple_type(b->simple_type); }
	if(b->subr_type != NULL){ res->subr_type   = copy_subr_type(b->subr_type); }
	
	return res;
}

struct ArrayType* copy_array_type(struct ArrayType* a){
	
	struct ArrayType* res = make(ArrayType);
	
	res->element_type = copy_type(a->element_type);
	
	return res;
}

struct Call* copy_call(struct Call* c){
	
	struct Call* res = make(Call);
	strcpy(res->name, c->name);
	
	res->args = malloc(sizeof(struct Expr*)*c->count_args);
	res->count_args = c->count_args;
	for(int i = 0;i < c->count_args;i++){
		res->args[i] = copy_expr(c->args[i]);
	}
	
	return res;
}
struct Stmt* copy_stmt(struct Stmt* stmt){

    struct Stmt* res = make(Stmt);

    res->super.line_num = stmt->super.line_num;
    res->super.annotations = stmt->super.annotations;

    res->kind = stmt->kind;
    switch(stmt->kind){
        case 1: res->ptr.m1 = copy_call(stmt->ptr.m1); break;
        case 2: res->ptr.m2 = copy_while_stmt(stmt->ptr.m2); break;
        case 3: res->ptr.m3 = copy_if_stmt(stmt->ptr.m3); break;
        case 4: res->ptr.m4 = copy_ret_stmt(stmt->ptr.m4); break;
        case 5: res->ptr.m5 = copy_assign_stmt(stmt->ptr.m5); break;
        case 6: res->ptr.m6 = copy_try_catch_stmt(stmt->ptr.m6); break;
        case 7: res->ptr.m7 = copy_for_stmt(stmt->ptr.m7); break;
        case 8: res->ptr.m8 = copy_switch_stmt(stmt->ptr.m8); break;
        default:
            res->is_break = stmt->is_break;
            res->is_continue = stmt->is_continue;
            res->is_throw = stmt->is_throw;
            break;
    }

    return res;
}

struct IfStmt* copy_if_stmt(struct IfStmt* i){

    struct IfStmt* res = make(IfStmt);
    res->else_block = NULL;

    res->super.line_num = i->super.line_num;
    res->super.annotations = i->super.annotations;

    res->condition = copy_expr(i->condition);
    res->block = copy_stmt_block(i->block);

    if(i->else_block != NULL){
        res->else_block = copy_stmt_block(i->else_block);
    }

    return res;
}

struct AssignStmt* copy_assign_stmt(struct AssignStmt* a){
    struct AssignStmt* res = make(AssignStmt);

    res->super.line_num = a->super.line_num;
    res->super.annotations = a->super.annotations;

    res->opt_type = NULL;
    if(a->opt_type != NULL){
        res->opt_type = copy_type(a->opt_type);
    }
    strcpy(res->assign_op, a->assign_op);
    res->expr = copy_expr(a->expr);
    res->var = copy_variable(a->var);

    return res;
}

struct RetStmt* copy_ret_stmt(struct RetStmt* r){
    struct RetStmt* res = make(RetStmt);

    res->super.line_num = r->super.line_num;
    res->super.annotations = r->super.annotations;

    res->return_value = copy_expr(r->return_value);
    return res;
}

struct WhileStmt* copy_while_stmt(struct WhileStmt* w){
    struct WhileStmt* res = make(WhileStmt);

    res->super.line_num = w->super.line_num;
    res->super.annotations = w->super.annotations;

    res->condition = copy_expr(w->condition);
    res->block = copy_stmt_block(w->block);

    return res;
}

struct ForStmt* copy_for_stmt(struct ForStmt* f){
    struct ForStmt* res = make(ForStmt);

    res->super.line_num = f->super.line_num;
    res->super.annotations = f->super.annotations;

    strcpy(res->index_name, f->index_name);
    res->range = copy_range(f->range);
    res->block = copy_stmt_block(f->block);

    return res;
}

struct SwitchStmt* copy_switch_stmt(struct SwitchStmt* s){
    struct SwitchStmt* res = make(SwitchStmt);

    res->super.line_num = s->super.line_num;
    res->super.annotations = s->super.annotations;

    res->expr = copy_expr(s->expr);
    res->count_cases = s->count_cases;

    res->cases = malloc(sizeof(void*)*res->count_cases);
    for(int i=0; i < res->count_cases; i++){
        res->cases[i] = copy_case_stmt(s->cases[i]);
    }

    return res;
}

struct TryCatchStmt* copy_try_catch_stmt(struct TryCatchStmt* tcs){

    struct TryCatchStmt* res = make(TryCatchStmt);

    res->super.line_num = tcs->super.line_num;
    res->super.annotations = tcs->super.annotations;

    res->try_block = copy_stmt_block(tcs->try_block);
    res->catch_block = copy_stmt_block(tcs->catch_block);

    return res;
}

struct CaseStmt* copy_case_stmt(struct CaseStmt* c){

	struct CaseStmt* res = make(CaseStmt);

	res->super.line_num = c->super.line_num;
	res->super.annotations = c->super.annotations;

	res->const_value = copy_const_value(c->const_value);
	res->block = NULL;

	if (c->block != NULL) {
		res->block = copy_stmt_block(c->block);
	}

	return res;
}