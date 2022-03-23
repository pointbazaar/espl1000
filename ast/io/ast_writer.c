#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "ast_writer.h"
#include "magic_num.h"
#include "serialize.h"

#define structsize(X) sizeof(struct X)

#define write_super(X) serialize_astnode(&(X->super), file)

static void error(FILE* file, char* msg){
	
	if(file != NULL){ fclose(file); }
	
	printf("[AST][Error] %s\n", msg);
	exit(1);
}

static int opt2int(void* opt){
	return (opt == NULL)?OPT_EMPTY:OPT_PRESENT;
}

void write_ast(struct AST* ast){
	
	for(int i = 0; i < ast->count_namespaces; i++)
		{ write_namespace(ast->namespaces[i]); }
}

void write_namespace(struct Namespace* m){
	
	FILE* file = fopen(m->ast_path, "w");

	if(file == NULL){
		
		printf("%s\n", m->ast_path);
		error(NULL, "could not open file");
	}
	
	//full buffering for performance
	setvbuf(file, NULL, _IOFBF, BUFSIZ);
	
	magic_num_serialize(MAGIC_NAMESPACE, file);
	
	serialize_string(m->src_path, file);
	serialize_string(m->token_path, file);
	serialize_string(m->ast_path, file);
	serialize_string(m->name, file);

	serialize_int(m->count_includes, file);
	for (int i = 0; i < m->count_includes; i++) {
		serialize_string(m->includes[i], file);
	}

	serialize_int(m->count_externc, file);
	for(int i=0; i < m->count_externc; i++)
		{ write_externc(m->externc[i], file); }
	
	serialize_int(m->count_methods, file);
	for(int i=0; i < m->count_methods; i++)
		{ write_method(m->methods[i], file); }
	
	serialize_int(m->count_structs, file);
	for(int i=0; i < m->count_structs; i++)
		{ write_struct_decl(m->structs[i], file); }
	
	magic_num_serialize(MAGIC_END_NAMESPACE, file);
	
	fclose(file);
	
}
void write_method(struct Method* m, FILE* file){
	
	magic_num_serialize(MAGIC_METHOD, file);
	
	write_super(m);
	write_method_decl(m->decl, file);
	write_stmt_block(m->block, file);
	
	magic_num_serialize(MAGIC_END_METHOD, file);
}
void write_method_decl(struct MethodDecl* m, OUTFILE){
	magic_num_serialize(MAGIC_METHOD_DECL, file);
	write_super(m);

	serialize_int(m->is_public, file);
	serialize_int(m->has_side_effects, file);

	serialize_string(m->name, file);

	write_type(m->return_type, file);

	serialize_int(m->count_args, file);

	for(int i = 0;i < m->count_args;i++)
	{ write_decl_arg(m->args[i], file); }

	magic_num_serialize(MAGIC_END_METHOD_DECL, file);
}
void write_struct_decl(struct StructDecl* m, FILE* file){
	
	magic_num_serialize(MAGIC_STRUCTDECL, file);
	
	write_super(m);

	write_simple_type(m->type, file);
	
	serialize_int(m->count_members, file);
	
	for(int i = 0;i < m->count_members;i++)
		{ write_struct_member(m->members[i], file); }
	
	magic_num_serialize(MAGIC_END_STRUCTDECL, file);
}
void write_struct_member(struct StructMember* m, FILE* file){
	
	magic_num_serialize(MAGIC_STRUCTMEMBER, file);
	
	write_super(m);

	write_type(m->type, file);
	
	serialize_string(m->name, file);
	
	magic_num_serialize(MAGIC_END_STRUCTMEMBER, file);
}
void write_externc(struct ExternC* ec, OUTFILE){

	magic_num_serialize(MAGIC_EXTERNC, file);

	serialize_int(ec->subr_decl != NULL, file);

	if (ec->subr_decl != NULL) {
		write_method_decl(ec->subr_decl, file);
	}else{
		write_struct_decl(ec->struct_decl, file);
	}

	serialize_string(ec->name_in_c, file);

	magic_num_serialize(MAGIC_END_EXTERNC, file);
}
void write_stmt_block(struct StmtBlock* block, FILE* file){
	
	magic_num_serialize(MAGIC_STMTBLOCK, file);
	
	write_super(block);
	
	serialize_int(block->count, file);
	
	for(int i = 0;i < block->count; i++)
		{ write_stmt(block->stmts[i], file); }
	
	magic_num_serialize(MAGIC_END_STMTBLOCK, file);
}

void write_decl_arg(struct DeclArg* m, FILE* file){

	magic_num_serialize(MAGIC_DECLARG, file);
	
	write_super(m);

	write_type(m->type, file);
	
	serialize_int(opt2int(m->name), file);
	
	//if(m->name != NULL)
		{ serialize_string(m->name, file); }
	
	magic_num_serialize(MAGIC_END_DECLARG, file);
}
void write_variable(struct Variable* m, FILE* file){

	magic_num_serialize(MAGIC_VARIABLE, file);
	
	write_super(m);

	write_simple_var(m->simple_var, file);

	serialize_int(opt2int(m->member_access), file);
	
	if(m->member_access != NULL)
		{ write_variable(m->member_access, file); }

	magic_num_serialize(MAGIC_END_VARIABLE, file);
}
void write_simple_var(struct SimpleVar* m, FILE* file){

	magic_num_serialize(MAGIC_SIMPLEVAR, file);
	
	write_super(m);

	serialize_string(m->name, file);

	serialize_int(m->count_indices, file);
	
	for(int i=0;i < m->count_indices; i++)
		{ write_expr(m->indices[i], file); }
	
	magic_num_serialize(MAGIC_END_SIMPLEVAR, file);
}
void write_expr(struct Expr* m, FILE* file){

	magic_num_serialize(MAGIC_EXPR, file);
	
	write_super(m);

	write_un_op_term(m->term1, file);
	
	serialize_int(opt2int(m->op), file);
	
	if(m->op != NULL){

		write_op(m->op, file);
		write_un_op_term(m->term2, file);
	}
	
	magic_num_serialize(MAGIC_END_EXPR, file);
}
void write_term(struct Term* m, FILE* file){
	
	magic_num_serialize(MAGIC_TERM, file);
	
	write_super(m);

	serialize_int(m->kind, file);

	switch(m->kind){
		case  4: write_call(m->ptr.m4, file); break;
		case  5: write_expr(m->ptr.m5, file); break;
		case  6: write_variable(m->ptr.m6, file); break;
		case  8: write_string_const(m->ptr.m8, file); break;
		case 11: write_lambda(m->ptr.m11, file);	break;
		case 12: write_const_value(m->ptr.m12, file); break;

		default: error(file, "Error in write_term(...)");
	}
	
	magic_num_serialize(MAGIC_END_TERM, file);
}
void write_un_op_term(struct UnOpTerm* t, FILE* file){
	
	magic_num_serialize(MAGIC_UNOPTERM, file);
	
	write_super(t);
	
	serialize_int(opt2int(t->op), file);
	
	if(t->op != NULL){ write_op(t->op, file); }

	write_term(t->term, file);
	
	magic_num_serialize(MAGIC_END_UNOPTERM, file);
}
void write_range(struct Range* r, FILE* file){
	
	magic_num_serialize(MAGIC_RANGE, file);
	
	write_super(r);

	write_expr(r->start, file);
	write_expr(r->end, file);
	
	magic_num_serialize(MAGIC_END_RANGE, file);
}
void write_lambda(struct Lambda* l, FILE* file){
	
	magic_num_serialize(MAGIC_LAMBDA, file);
	
	write_super(l);
	
	serialize_int(l->count_params, file);
	
	for(uint8_t i = 0; i < l->count_params; i++){
		serialize_string(l->params[i]->identifier, file);
	}

	write_expr(l->expr, file);
	
	magic_num_serialize(MAGIC_END_LAMBDA, file);
}
// --------- CONST NODES ----------------
void write_bool_const(struct BoolConst* m, FILE* file){
	
	magic_num_serialize(MAGIC_BOOLCONST, file);
	write_super(m);
	fwrite(m, structsize(BoolConst), 1, file);
	magic_num_serialize(MAGIC_END_BOOLCONST, file);
}
void write_int_const(struct IntConst* m, FILE* file){
	
	magic_num_serialize(MAGIC_INTCONST, file);
	write_super(m);
	fwrite(m, structsize(IntConst), 1, file);
	magic_num_serialize(MAGIC_END_INTCONST, file);
}
void write_hex_const(struct HexConst* m, FILE* file){
	
	magic_num_serialize(MAGIC_HEXCONST, file);
	write_super(m);
	fwrite(m, structsize(HexConst), 1, file);
	magic_num_serialize(MAGIC_END_HEXCONST, file);
}
void write_bin_const(struct BinConst* m, FILE* file){
	
	magic_num_serialize(MAGIC_BINCONST, file);
	write_super(m);
	fwrite(m, structsize(BinConst), 1, file);
	magic_num_serialize(MAGIC_END_BINCONST, file);
}
void write_char_const(struct CharConst* m, FILE* file){
	
	magic_num_serialize(MAGIC_CHARCONST, file);
	write_super(m);
	fwrite(m, structsize(CharConst), 1, file);
	magic_num_serialize(MAGIC_END_CHARCONST, file);
}
void write_float_const(struct FloatConst* m, FILE* file){
	
	magic_num_serialize(MAGIC_FLOATCONST, file);
	write_super(m);
	fwrite(m, structsize(FloatConst), 1, file);
	magic_num_serialize(MAGIC_END_FLOATCONST, file);
}
void write_const_value(struct ConstValue* m, OUTFILE){

	magic_num_serialize(MAGIC_CONSTVALUE, file);

	write_super(m);

	serialize_int(m->kind, file);

	switch(m->kind){
		case 1: write_bool_const(m->ptr.m1_bool_const, file); break;
		case 2: write_int_const(m->ptr.m2_int_const, file);  break;
		case 3: write_char_const(m->ptr.m3_char_const, file); break;
		case 4: write_float_const(m->ptr.m4_float_const, file); break;
		case 5: write_hex_const(m->ptr.m5_hex_const, file); break;
		case 6: write_bin_const(m->ptr.m6_bin_const, file); break;
		default:
			error(file, "Error in write_const_value");
	}

	magic_num_serialize(MAGIC_END_CONSTVALUE, file);
}
void write_string_const(struct StringConst* m, FILE* file){
	
	magic_num_serialize(MAGIC_STRINGCONST, file);
	write_super(m);
	serialize_string(m->value, file);
	
	magic_num_serialize(MAGIC_END_STRINGCONST, file);
}
void write_op(struct Op* m, FILE* file){
	
	magic_num_serialize(MAGIC_OP, file);
	write_super(m);
	fwrite(m, structsize(Op), 1, file);
	
	magic_num_serialize(MAGIC_END_OP, file);
}
// ---------------- STATEMENTNODES ---------------------
void write_stmt(struct Stmt* m, FILE* file){

	magic_num_serialize(MAGIC_STMT, file);
	write_super(m);
	serialize_int(m->kind, file);
	
	switch(m->kind){
		case 99: 
			{
				serialize_int(m->is_break ? OPT_PRESENT : OPT_EMPTY, file);
				serialize_int(m->is_continue ? OPT_PRESENT : OPT_EMPTY, file);
			}
			break;
		case 1: {
			write_call(m->ptr.m1, file);       } break;
		case 2: {
			write_while_stmt(m->ptr.m2, file);  } break;
		case 3: {
			write_if_stmt(m->ptr.m3, file);     } break;
		case 4: {
			write_ret_stmt(m->ptr.m4, file);    } break;
		case 5: {
			write_assign_stmt(m->ptr.m5, file); } break;
		case 7: {
			write_for_stmt(m->ptr.m7, file);    } break;
		case 8: {
			write_switch_stmt(m->ptr.m8, file); } break;
		default: error(file, "Error in write_stmt");
	}
	
	magic_num_serialize(MAGIC_END_STMT, file);
}

void write_if_stmt(struct IfStmt* m, FILE* file){

	magic_num_serialize(MAGIC_IFSTMT, file);
	write_super(m);
	write_expr(m->condition, file);
	write_stmt_block(m->block, file);
	
	serialize_int(opt2int(m->else_block), file);
	
	if(m->else_block != NULL)
		{ write_stmt_block(m->else_block, file); }
	
	magic_num_serialize(MAGIC_END_IFSTMT, file);
}

void write_while_stmt(struct WhileStmt* m, FILE* file){

	magic_num_serialize(MAGIC_WHILESTMT, file);
	write_super(m);
	write_expr(m->condition, file);
	write_stmt_block(m->block, file);
	
	magic_num_serialize(MAGIC_END_WHILESTMT, file);
}

void write_ret_stmt(struct RetStmt* m, FILE* file){

	magic_num_serialize(MAGIC_RETSTMT, file);
	write_super(m);
	write_expr(m->return_value, file);
	
	magic_num_serialize(MAGIC_END_RETSTMT, file);
}

void write_assign_stmt(struct AssignStmt* m, FILE* file){

	magic_num_serialize(MAGIC_ASSIGNSTMT, file);
	write_super(m);
	serialize_int(opt2int(m->opt_type), file);

	if(m->opt_type != NULL){
		write_type(m->opt_type, file);
	}

	write_variable(m->var, file);
	
	serialize_string(m->assign_op, file);

	write_expr(m->expr, file);
	
	magic_num_serialize(MAGIC_END_ASSIGNSTMT, file);
}
void write_call(struct Call* m, FILE* file){

	magic_num_serialize(MAGIC_CALL, file);
	write_super(m);

    write_variable(m->callable, file);

	serialize_int(m->count_args, file);
	for(int i=0;i < m->count_args;i++)
		{ write_expr(m->args[i], file); }
	
	magic_num_serialize(MAGIC_END_CALL, file);
}
void write_for_stmt(struct ForStmt* m, FILE* file){
	
	magic_num_serialize(MAGIC_FORSTMT, file);
	write_super(m);
	serialize_string(m->index_name, file);

	write_range(m->range, file);
	write_stmt_block(m->block, file);
	
	magic_num_serialize(MAGIC_END_FORSTMT, file);
}
void write_switch_stmt(struct SwitchStmt* m, FILE* file){
	
	magic_num_serialize(MAGIC_SWITCHSTMT, file);
	write_super(m);
	write_expr(m->expr, file);
	
	serialize_int(m->count_cases, file);
	
	for(int i=0; i < m->count_cases; i++)
		{ write_case_stmt(m->cases[i], file); }
	
	magic_num_serialize(MAGIC_END_SWITCHSTMT, file);
}
void write_case_stmt(struct CaseStmt* m, FILE* file){
	
	magic_num_serialize(MAGIC_CASESTMT, file);
	write_super(m);

	write_const_value(m->const_value, file);

	serialize_int(opt2int(m->block), file);
	
	if(m->block != NULL)
		{ write_stmt_block(m->block, file); }
	
	magic_num_serialize(MAGIC_END_CASESTMT, file);
}
// --------- TYPENODES --------------
void write_type(struct Type* m, FILE* file){
	
	magic_num_serialize(MAGIC_TYPE, file);
	write_super(m);
	if(m->basic_type != NULL){
		serialize_int(1, file);
		write_basic_type(m->basic_type, file);

	}else if(m->type_param != NULL){
		serialize_int(2, file);
		write_type_param(m->type_param, file);

	}else if(m->array_type != NULL){
		serialize_int(3, file);
		write_array_type(m->array_type, file);
	}
	
	magic_num_serialize(MAGIC_END_TYPE, file);
}

void write_array_type(struct ArrayType* m, FILE* file){
	
	magic_num_serialize(MAGIC_ARRAYTYPE, file);
	write_super(m);
	write_type(m->element_type, file);
	
	magic_num_serialize(MAGIC_END_ARRAYTYPE, file);
}

void write_type_param(struct TypeParam* m, FILE* file){
	
	magic_num_serialize(MAGIC_TYPEPARAM, file);
	write_super(m);
	fwrite(m, structsize(TypeParam), 1, file);
	
	magic_num_serialize(MAGIC_END_TYPEPARAM, file);
}

void write_basic_type(struct BasicType* m, FILE* file){

	magic_num_serialize(MAGIC_BASICTYPE, file);
	write_super(m);
	if(m->simple_type != NULL){
		
		serialize_int(1, file);
		write_simple_type(m->simple_type, file);
		
	}else if(m->subr_type != NULL){
		
		serialize_int(2, file);
		write_subr_type(m->subr_type, file);
		
	}
	
	magic_num_serialize(MAGIC_END_BASICTYPE, file);
}

void write_struct_type(struct StructType* m, FILE* file){
	
	magic_num_serialize(MAGIC_STRUCTTYPE, file);
	write_super(m);
	serialize_string(m->type_name, file);
	serialize_int(m->count_type_params, file);
	
	for(int i = 0;i < m->count_type_params; i++){
        write_type(m->type_params[i], file);
	}
	
	magic_num_serialize(MAGIC_END_STRUCTTYPE, file);
}

void write_primitive_type(struct PrimitiveType* m, FILE* file){
	
	magic_num_serialize(MAGIC_PRIMITIVETYPE, file);
	write_super(m);
	
	serialize_int(m->is_int_type, file);
	serialize_int(m->is_float_type, file);
	serialize_int(m->is_char_type, file);
	serialize_int(m->is_bool_type, file);

	serialize_int(m->int_type, file);

	magic_num_serialize(MAGIC_END_PRIMITIVETYPE, file);
}

void write_simple_type(struct SimpleType* m, FILE* file){
	
	magic_num_serialize(MAGIC_SIMPLETYPE, file);
	write_super(m);
	
	serialize_int(opt2int(m->struct_type), file);
	
	if(m->primitive_type != NULL){
		write_primitive_type(m->primitive_type, file);
	}
	
	if(m->struct_type != NULL){
		write_struct_type(m->struct_type, file);
	}
	
	magic_num_serialize(MAGIC_END_SIMPLETYPE, file);
}

void write_subr_type(struct SubrType* m, FILE* file){

	magic_num_serialize(MAGIC_SUBRTYPE, file);
	write_super(m);

	write_type(m->return_type, file);
	serialize_int(m->has_side_effects, file);

	serialize_int(m->count_arg_types, file);
	
	for(int i = 0;i < m->count_arg_types; i++)
		{ write_type(m->arg_types[i], file); }
	
	magic_num_serialize(MAGIC_END_SUBRTYPE, file);
}
