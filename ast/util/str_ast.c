#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "str_ast.h"

static void error(char* msg){

	printf("[AST][Error] %s\n", msg);
	exit(1);
}

//---------------------------------------

char* str_bool_const(struct BoolConst* bc){
	char* res =  malloc(sizeof(char)*10);
	strcpy(res, bc->value ? "true": "false");
	return res;
}	

char* str_char_const(struct CharConst* cc){

	char* res =  malloc(sizeof(char)*10);
	sprintf(res, "'%c'", cc->value);
	return res;
}

char* str_float_const(struct FloatConst* fc){

	char* res =  malloc(sizeof(char)*10);
	sprintf(res, "%f", fc->value);
	return res;
}

char* str_identifier(struct Identifier* id){

	char* res =  malloc(sizeof(char)*(strlen(id->identifier)+1));
	sprintf(res, "%s", id->identifier);
	return res;
}

char* str_range(struct Range* r){
	
	char* s1 = str_expr(r->start);
	char* s2 = str_expr(r->end);
	
	uint16_t l = strlen(s1) + strlen(s2) + 4+1;
	
	char* res = malloc(sizeof(char)*l);
	
	sprintf(res, "%s .. %s", s1, s2);
	
	free(s1); free(s2);
	
	return res;
}

char* str_stmt_block(struct StmtBlock* block){
	
	uint16_t l = 0;

	for(uint16_t i = 0; i < block->count; i++){
		
		char* s = str_stmt(block->stmts[i]);
		
		l += strlen(s) + 1;
		
		free(s);
	}
	
	l += 1 + 2 + 2;
	
	char* res = malloc(sizeof(char)*l);
	
	strcpy(res, "");
	strcat(res, "{\n");
	
	for(uint16_t i = 0; i < block->count; i++){
		
		char* s = str_stmt(block->stmts[i]);
		
		strcat(res, s);
		strcat(res, "\n");
		
		free(s);
	}
	strcat(res, "}\n");
	return res;
}

char* str_lambda(struct Lambda* lambda){
	
	uint16_t l = 0;

	for(uint16_t i = 0; i < lambda->count_params; i++){
		
		char* s = str_identifier(lambda->params[i]);
		l += strlen(s) + 1;
		free(s);
	}
	
	char* s2 = str_expr(lambda->expr);
	l += strlen(s2);
	
	l += 2 + 4;
	
	char* res = malloc(sizeof(char)*l);
	
	strcpy(res, "");
	strcat(res, "(");
	
	for(uint16_t i = 0; i < lambda->count_params; i++){
		
		char* s = str_identifier(lambda->params[i]);
		strcat(res, s);
		
		if(i < (lambda->count_params - 1)){
			strcat(res, ",");
		}
		
		free(s);
	}
	strcat(res, ") -> ");
	strcat(res, s2);
	free(s2);
	
	return res;
}

char* str_int_const(struct IntConst* ic){

	char* res =  malloc(sizeof(char)*10);
	sprintf(res, "%d", ic->value);
	return res;
}

char* str_hex_const(struct HexConst* hc){

	char* res =  malloc(sizeof(char)*10);
	sprintf(res, "0x%x", hc->value);
	return res;
}

char* str_string_const(struct StringConst* s){
	
	char* res =  malloc(sizeof(char)*(3+strlen(s->value)));
	sprintf(res, "%s", s->value);
	return res;
}

char* str_bin_const(struct BinConst* b){
	
	char* res = malloc(sizeof(char)*129);
	
	uint32_t value = b->value;
	
	if(value == 0){
		sprintf(res, "0b0");
		return res;
	}
	
	const int size = 128;
	
	char buffer[size];
	
	int index = size - 1;
	buffer[index--] = '\0';
	
	while(value > 0){
		
		uint8_t bit = value & 0x1;
		
		buffer[index--] = (bit == 0x1) ? '1' : '0';
		
		value >>= 1;
	}

	sprintf(res, "0b%s", buffer+index+1);
	return res;
}

char* str_array_type(struct ArrayType* at){

	char* inner = str_type(at->element_type);
	
	char* res = malloc(sizeof(char)*(strlen(inner)+2+1));
	
	sprintf(res, "[%s]", inner);
	
	free(inner);
	
	return res;
}

char* str_basic_type(struct BasicType* btw){
	
	if(btw->simple_type != NULL){
		return str_simple_type(btw->simple_type);
	}
	
	if(btw->subr_type != NULL){
		return str_subr_type(btw->subr_type);
	}
	
	error("strBasicTypeWrapped");
	return NULL;
}

char* str_simple_type(struct SimpleType* st){
	
	if(st->primitive_type != NULL)
		{ return str_primitive_type(st->primitive_type); }
		
	return str_struct_type(st->struct_type);
}

char* str_subr_type(struct SubrType* st){

	//TODO: get rid of the magic number '1000'
	//and calculate how long exactly
	char* res = malloc(sizeof(char)*1000);
	strcpy(res, "");
	
	strcat(res, "(");
	
	for(int i=0;i < st->count_arg_types; i++){
	
		char* argType = str_type(st->arg_types[i]);
		strcat(res, argType);
		free(argType);
		
		if(i < (st->count_arg_types - 1)){
			
			strcat(res, ", ");
		}
	}
	
	strcat(res, ")");
	
	strcat(res, (st->has_side_effects) ? "~>" : "->");
	
	char* returntype = str_type(st->return_type);
	strcat(res, returntype);
	
	if(st->throws){ strcat(res, " throws "); }
	
	free(returntype);
	
	return res;
}

char* str_type(struct Type* t){
	
	if(t->m1 != NULL){ return str_basic_type(t->m1); }
	
	if(t->m2 != NULL){ return str_type_param(t->m2); }
	
	if(t->m3 != NULL){ return str_array_type(t->m3); }
	
	error("str_type");
	return NULL;
}

char* str_type_param(struct TypeParam* t){
	
	char* res =  malloc(sizeof(char)*10);
	sprintf(res, "?T%d", t->index);
	return res;
}


char* str_primitive_type(struct PrimitiveType* p){
	
	char* res = malloc(DEFAULT_STR_SIZE);
	
	if(p->is_int_type){
		
		char* types[] = 
		{"int8","int16","int32","int64",
		"uint8","uint16","uint32","uint64",
		"int","uint"
		};
		
		sprintf(res, "%s", types[p->int_type]);
	}
	
	if(p->is_float_type) { sprintf(res, "float"); }
	if(p->is_char_type)  { sprintf(res, "char"); }
	if(p->is_bool_type)  { sprintf(res, "bool"); }
	
	return res;
}

char* str_struct_type(struct StructType* s){
	
	//TODO: add the generic part
	if(s->count_type_params != 0){
	
		error("str_struct_type");
	}
	
	char* res = malloc(DEFAULT_STR_SIZE);
	
	sprintf(res, "%s", s->type_name);
	
	return res;
}

char* str_struct_decl(struct StructDecl* decl){
	
	char* name = str_simple_type(decl->type);

	uint16_t l = strlen("struct   { } ")+strlen(name);
	
	char* memberStrs[decl->count_members];
	
	for(uint16_t i = 0; i < decl->count_members; i++){
		
		char* s2 = str_struct_member(decl->members[i]);
		l += strlen(s2);
		
		memberStrs[i] = s2;
	}
	
	char* res = malloc(sizeof(char)*l);
	
	sprintf(res, "struct %s {", decl->type->struct_type->type_name);
	
	for(uint16_t i = 0; i < decl->count_members; i++){
		
		char* s = memberStrs[i];
		strcat(res, s);
		free(s);
	}
	
	strcat(res, "}");
	
	return res;
}

char* str_struct_member(struct StructMember* s){

	char* s1 = str_type(s->type);
	const int l = strlen(s1);
	
	char* res = malloc(sizeof(char)*(l+DEFAULT_STR_SIZE+3));
	
	sprintf(res, "%s %s;", s1, s->name);
	
	return res;
}

char* str_variable(struct Variable* v){
	
	char* s1 = str_simple_var(v->simple_var);
	
	char* s2 = (v->member_access != NULL) ? str_variable(v->member_access) : "";
	
	uint16_t l = strlen(s1) + strlen(s2);
	
	char* res = malloc(sizeof(char)*(l+1));
	
	sprintf(res, "%s%s", s1, s2);
	
	free(s1);
	if(v->member_access != NULL){ free(s2); }
	
	return res;
}

char* str_simple_var(struct SimpleVar* s){
	
	//we approximate here, and could be wrong.
	//this is definitely not bulletproof.
	//TODO: replace with a robust implementation
	
	uint16_t l = DEFAULT_STR_SIZE+1;
	
	l += s->count_indices * (20+2);
	
	char* res = malloc(sizeof(char)*l);
	
	strcpy(res, s->name);
	
	for(uint16_t i = 0; i < s->count_indices; i++){
		
		char* s1 = str_expr(s->indices[i]);
		
		strcat(res, s1);
		
		free(s1);
	}
	
	return res;
}

char* str_expr(struct Expr* e){

	char* strTerm1 = str_un_op_term(e->term1);
	
	char* strO = (e->op != NULL) ? str_op(e->op) : "";
	
	char* strTerm2 = (e->term2 != NULL) ? str_un_op_term(e->term2) : "";
	
	uint16_t l1 = strlen(strTerm1);
	uint16_t l2 = strlen(strO);
	uint16_t l3 = strlen(strTerm2);
	
	uint16_t l = l1+l2+l3+1;
	
	char* res = malloc(sizeof(char)*l);
	
	sprintf(res, "%s%s%s", strTerm1, strO, strTerm2);
	
	free(strTerm1);
	if(e->op != NULL){ free(strO); }
	if(e->term2 != NULL){ free(strTerm2); }
	
	return res;
}

char* str_op(struct Op* o){
	
	char* res = malloc(sizeof(char)*6);
	
	sprintf(res, "%s", o->op);
	
	return res;
}

char* str_un_op_term(struct UnOpTerm* u){

	char* strO = (u->op != NULL) ? str_op(u->op) : "";
	
	char* strT = str_term(u->term);
	
	uint16_t l = strlen(strO) + strlen(strT)+3;
	
	char* res = malloc(sizeof(char)*l);
	
	sprintf(res, "%s%s", strO, strT);
	
	if(u->op != NULL){ free(strO); }
	free(strT);
	
	return res;
}

char* str_term(struct Term* t){
	
	switch(t->kind){
		
		case 1: return str_bool_const(t->ptr.m1);
		case 2: return str_int_const(t->ptr.m2);
		case 3: return str_char_const(t->ptr.m3);
		case 4: return str_call(t->ptr.m4);
		case 5: return str_expr(t->ptr.m5);
		case 6: return str_variable(t->ptr.m6);
		case 7: return str_float_const(t->ptr.m7);
		case 8: return str_string_const(t->ptr.m8);
		case 9: return str_hex_const(t->ptr.m9);
		
		case 10: return str_bin_const(t->ptr.m10);
	}
	
	error("str_term");
	return NULL;
}

char* str_stmt(struct Stmt* stmt){
	
	switch(stmt->kind){
		
		case 0: return str_loop_stmt(stmt->ptr.m0);
		case 1: return str_call(stmt->ptr.m1);
		case 2: return str_while_stmt(stmt->ptr.m2);
		case 3: return str_if_stmt(stmt->ptr.m3);
		case 4: return str_ret_stmt(stmt->ptr.m4);
		case 5: return str_assign_stmt(stmt->ptr.m5);
		case 6: return str_try_catch_stmt(stmt->ptr.m6);
		case 7: return str_for_stmt(stmt->ptr.m7);
		case 8: return str_switch_stmt(stmt->ptr.m8);
		
		case 99: {
			//break,continue,throw,...
			char* res = malloc(sizeof(char)*30);
			strcpy(res, "");
			
			if(stmt->is_break)    { sprintf(res, "break"); }
			if(stmt->is_continue) { sprintf(res, "continue"); }
			if(stmt->is_throw)    { sprintf(res, "throw"); }
			return res;
		}
			
		default:
			error("str_stmt");
			return NULL;
	}
}

char* str_assign_stmt(struct AssignStmt* a){
	
	char* strOptType = "";
	
	if(a->opt_type != NULL){
			
		strOptType = str_type(a->opt_type);
	}
	
	char* strVar = str_variable(a->var);
	
	char* strE = str_expr(a->expr);
	
	uint16_t l1 = strlen(strOptType);
	uint16_t l2 = strlen(strVar);
	uint16_t l3 = strlen(strE);
	
	uint16_t l = l1+l2+l3+1+4;
	
	char* res = malloc(sizeof(char)*l);
	
	sprintf(res, "%s %s = %s", strOptType, strVar, strE);
	
	if(a->opt_type != NULL){ free(strOptType); }
	free(strVar);
	free(strE);
	
	return res;
}

char* str_for_stmt(struct ForStmt* f){
	
	char* s1 = str_range(f->range);
	char* s2 = str_stmt_block(f->block);
	
	const uint32_t l = strlen(s1)+strlen(s2)
				+3+4+1+DEFAULT_STR_SIZE;
	
	char* res = malloc(sizeof(char)*l);
	
	sprintf(res, "for %s in %s %s", f->index_name, s1, s2);
	
	free(s1); free(s2);
	return res;
}

char* str_if_stmt(struct IfStmt* i){

	char* s1 = str_expr(i->condition);
	char* s2 = str_stmt_block(i->block);
	
	char* s3 = "";
	
	if(i->else_block != NULL){
		s3 = str_stmt_block(i->else_block);
	}
	
	const uint32_t l = strlen(s1)+strlen(s2)+strlen(s3)
		+1+10;
		
	char* res = malloc(sizeof(char)*l);
	
	sprintf(res, "if %s %s", s1, s2);
	
	free(s1); free(s2);
	
	if(i->else_block != NULL){
		
		strcat(res, " else ");
		strcat(res, s3);
		free(s3);
	}
	
	return res;
}

char* str_loop_stmt(struct LoopStmt* l){
	
	char* s1 = str_expr(l->count);
	char* s2 = str_stmt_block(l->block);
	
	const uint32_t l1 = strlen(s1)+strlen(s2)+4+2+1;
	
	char* res = malloc(sizeof(char)*l1);
	
	sprintf(res, "loop %s %s", s1, s2);
	
	free(s1); free(s2);
	return res;
}

char* str_while_stmt(struct WhileStmt* w){

	char* s1 = str_expr(w->condition);
	char* s2 = str_stmt_block(w->block);
	
	const uint32_t l = strlen(s1)+strlen(s2)+5+2+1;
	
	char* res = malloc(sizeof(char)*l);
	
	sprintf(res, "while %s %s", s1, s2);
	
	free(s1); free(s2);
	return res;
}

char* str_switch_stmt(struct SwitchStmt* s){

	uint16_t l = strlen("switch { } ");
	
	char* s1 = str_expr(s->expr);
	l += strlen(s1);
	
	char* strCases[s->count_cases];
	
	for(uint16_t i = 0; i < s->count_cases; i++){
		
		strCases[i] = str_case_stmt(s->cases[i]);
		l += strlen(strCases[i]);
	}
	
	char* res = malloc(sizeof(char)*l);
	
	sprintf(res, "switch %s {", s1);
	
	free(s1);
	
	for(uint16_t i = 0; i < s->count_cases; i++){
		
		strcat(res, strCases[i]);
	}
	
	strcat(res, "}");
	
	return res;
}

char* str_call(struct Call* m){

	//we approximate here, and could be wrong.
	//this is definitely not bulletproof.
	//TODO: replace with more robust implementation
	
	uint16_t l = DEFAULT_STR_SIZE+1+2;
	
	l += m->count_args * (20+1);
	
	char* res = malloc(sizeof(char)*l);
	
	strcpy(res, m->name);
	strcat(res, "(");
	
	for(uint16_t i = 0; i < m->count_args; i++){
		
		char* s1 = str_expr(m->args[i]);
		
		strcat(res, s1);
		
		if(i != (m->count_args - 1)){
			strcat(res, ",");
		}
		
		free(s1);
	}
	
	strcat(res, ")");
	
	return res;
}

char* str_ret_stmt(struct RetStmt* r){
	
	char* s = str_expr(r->return_value);
	
	uint16_t l = 10 + strlen(s);
	
	char* res =  malloc(sizeof(char)*l);
	
	sprintf(res, "return %s;", s);
	
	free(s);
	
	return res;
}

char* str_case_stmt(struct CaseStmt* c){
	
	char* s = NULL;
	
	switch(c->kind){
	
		case 0: s = str_bool_const(c->ptr.m1); break;
		case 1: s = str_char_const(c->ptr.m2); break;
		case 2: s = str_int_const(c->ptr.m3); break;
	}
	
	char* s2 = str_stmt_block(c->block);
	
	uint16_t l = strlen(s) + strlen(s2);

	char* res = malloc(sizeof(char)*(l+1+6));
	
	sprintf(res, "case %s %s", s, s2);
	
	free(s); free(s2);
	
	return res;
}

char* str_try_catch_stmt(struct TryCatchStmt* tcs){
	
	char* s1 = str_stmt_block(tcs->try_block);
	char* s2 = str_stmt_block(tcs->catch_block);
	
	uint32_t l = strlen(s1) + strlen(s2);
	
	char* res = malloc(sizeof(char)*(l+3+5+10));
	
	sprintf(res, "try %s catch %s", s1, s2);
	
	free(s1); free(s2);
	
	return res;
}
