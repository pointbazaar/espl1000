#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../ast.h"
#include "ast_reader.h"
#include "magic_num.h"
#include "serialize.h"

#define structsize(X) sizeof(struct X)

#define read_super(X) deserialize_astnode(&(X->super), file)

static void error(FILE* file, char* msg){
	
	if(file != NULL){ fclose(file); }
	
	printf("[AST][Error] %s\n", msg);
	exit(1);
}

struct AST* read_ast(char** filenames, int count_filenames) {
	
	struct AST* ast = make(AST);
	
	ast->count_namespaces = count_filenames;
	
	uint32_t nbytes = sizeof(void*) * ast->count_namespaces;
	
	ast->namespaces = malloc(nbytes);

	for(int i = 0;i < count_filenames; i++){

		FILE* file = fopen(filenames[i], "r");
		
		if(file == NULL){
			printf("%s\n", filenames[i]);
			error(NULL, "Could not open file");
		}
		
		//full buffering for performance
		setvbuf(file, NULL, _IOFBF, BUFSIZ);

		ast->namespaces[i] = read_namespace(file);

		fclose(file);
	}
	
	return ast;
}

struct Namespace* read_namespace(FILE* file) {
	
	magic_num_require(MAGIC_NAMESPACE, file);
	
	struct Namespace* ns = make(Namespace);

	char* tmp1 = deserialize_string(file);
	char* tmp2 = deserialize_string(file);
	char* tmp3 = deserialize_string(file);
	char* tmp4 = deserialize_string(file);
	
	ns->src_path   = malloc(sizeof(char)*(strlen(tmp1)+1));
	ns->token_path = malloc(sizeof(char)*(strlen(tmp2)+1));
	ns->ast_path   = malloc(sizeof(char)*(strlen(tmp3)+1));
	
	strcpy (ns->src_path, tmp1);
	strcpy (ns->token_path, tmp2);
	strcpy (ns->ast_path, tmp3);
	
	strncpy(ns->name,     tmp4, DEFAULT_STR_SIZE);
	
	free(tmp1);
	free(tmp2);
	free(tmp3);

	ns->count_includes = deserialize_int(file);
	ns->includes = malloc(sizeof(void*) * (ns->count_includes));
	for (int i = 0; i < ns->count_includes; i++) {
		ns->includes[i] = deserialize_string(file);
	}

	ns->count_externc = deserialize_int(file);
	ns->externc = malloc(sizeof(void*)*(ns->count_externc));
	for(int i=0; i < ns->count_externc; i++){
		ns->externc[i] = read_externc(file);
	}

	ns->count_methods = deserialize_int(file);
	ns->methods = malloc(sizeof(void*)*(ns->count_methods));
	for(int i=0; i < ns->count_methods; i++){
		ns->methods[i] = read_method(file);
	}

	ns->count_structs = deserialize_int(file);
	ns->structs = malloc(sizeof(void*)*(ns->count_structs));
	for(int i=0;i < ns->count_structs; i++){
		ns->structs[i] = read_struct_decl(file);
	}
	
	magic_num_require(MAGIC_END_NAMESPACE, file);
	
	return ns;
}
struct Method* read_method(FILE* file) {
	
	magic_num_require(MAGIC_METHOD, file);
	
	struct Method* m  = make(Method);
	
	read_super(m);

	m->decl = read_method_decl(file);
	m->block = read_stmt_block(file);
	
	magic_num_require(MAGIC_END_METHOD, file);
	return m;
}

struct MethodDecl* read_method_decl(INFILE){

	magic_num_require(MAGIC_METHOD_DECL, file);

	struct MethodDecl* m  = make(MethodDecl);

	read_super(m);

	m->is_public        = deserialize_int(file);
	m->has_side_effects = deserialize_int(file);
	m->throws           = deserialize_int(file);

	char* tmp = deserialize_string(file);
	strcpy(m->name, tmp);

	free(tmp);

	m->return_type = read_type(file);

	m->count_args = deserialize_int(file);
	m->args = malloc(sizeof(void*)*(m->count_args));

	for(int i = 0;i < m->count_args;i++){
		m->args[i] = read_decl_arg(file);
	}

	magic_num_require(MAGIC_END_METHOD_DECL, file);
	return m;
}

struct StructDecl* read_struct_decl(FILE* file) {
	
	magic_num_require(MAGIC_STRUCTDECL, file);
	
	struct StructDecl* res = make(StructDecl);
	
	read_super(res);
	
	res->type = read_simple_type(file);
	
	res->count_members = deserialize_int(file);
	
	res->members = malloc(sizeof(void*)*res->count_members);
	
	for(int i=0;i < res->count_members;i++){
		res->members[i] = read_struct_member(file);
	}
	
	magic_num_require(MAGIC_END_STRUCTDECL, file);
	
	return res;
}
struct StructMember* read_struct_member(FILE* file) {
	
	magic_num_require(MAGIC_STRUCTMEMBER, file);
	
	struct StructMember* res = make(StructMember);
	
	read_super(res);
	
	res->type = read_type(file);
	
	char* tmp = deserialize_string(file);
	
	strcpy(res->name, tmp);
	free(tmp);
	
	magic_num_require(MAGIC_END_STRUCTMEMBER, file);
	
	return res;
}
struct ExternC*	read_externc(INFILE){

	magic_num_require(MAGIC_EXTERNC, file);

	struct ExternC* res = make(ExternC);
	*res = (struct ExternC) {
			.subr_decl   = NULL,
			.struct_decl = NULL
	};

	const bool is_subr_decl = deserialize_int(file);

	if (is_subr_decl) {
		res->subr_decl = read_method_decl(file);
	}else{
		res->struct_decl = read_struct_decl(file);
	}

	char* tmp = deserialize_string(file);
	strncpy(res->name_in_c, tmp, DEFAULT_STR_SIZE-1);
	free(tmp);

	magic_num_require(MAGIC_END_EXTERNC, file);

	return res;
}
struct StmtBlock* read_stmt_block(FILE* file) {
	
	magic_num_require(MAGIC_STMTBLOCK, file);
	
	struct StmtBlock* block = make(StmtBlock);
	
	read_super(block);
	
	block->count = deserialize_int(file);
	
	block->stmts = malloc(sizeof(void*)* block->count);
	
	for(int i = 0;i < block->count; i++){
		block->stmts[i] = read_stmt(file);
	}
	
	magic_num_require(MAGIC_END_STMTBLOCK, file);
	
	return block;
}

struct DeclArg* read_decl_arg(FILE* file) {
	
	magic_num_require(MAGIC_DECLARG, file);
	
	struct DeclArg* da = make(DeclArg);
	
	read_super(da);

	da->type = read_type(file);

	const uint32_t option = deserialize_int(file);

	da->has_name = option == OPT_PRESENT;

	if(option != 0 && option != 1){

		error(file, "Error in read_decl_arg");
	}

	if(da->has_name){
		char* tmp = deserialize_string(file);
		strcpy(da->name, tmp);
		free(tmp);
	}
	
	magic_num_require(MAGIC_END_DECLARG, file);

	return da;
}
struct Expr* read_expr(FILE* file) {
	
	magic_num_require(MAGIC_EXPR, file);
	
	struct Expr* expr = make(Expr);
	
	read_super(expr);

	expr->term1 = read_un_op_term(file);
	expr->op    = NULL;
	expr->term2 = NULL;
	
	const uint32_t option = deserialize_int(file);
	
	if (option == OPT_PRESENT){
		
		expr->op    = read_op(file);
		expr->term2 = read_un_op_term(file);
	}
	
	magic_num_require(MAGIC_END_EXPR, file);

	return expr;
}
struct Op* read_op(FILE* file) {
	
	magic_num_require(MAGIC_OP, file);
	
	struct Op* op = make(Op);
	
	read_super(op);
	
	fread(op, structsize(Op), 1, file);
	
	magic_num_require(MAGIC_END_OP, file);

	return op;
}
struct IntConst* read_int_const(FILE* file) {
	
	magic_num_require(MAGIC_INTCONST, file);
	
	struct IntConst* ic = make(IntConst);
	
	read_super(ic);
		
	fread(ic, structsize(IntConst), 1, file);
	
	magic_num_require(MAGIC_END_INTCONST, file);

	return ic;
}

struct HexConst* read_hex_const(FILE* file) {
	
	magic_num_require(MAGIC_HEXCONST, file);
	
	struct HexConst* hc = make(HexConst);
	
	read_super(hc);
	
	fread(hc, structsize(HexConst), 1, file);
	
	magic_num_require(MAGIC_END_HEXCONST, file);

	return hc;
}

struct BinConst* read_bin_const(FILE* file) {
	
	magic_num_require(MAGIC_BINCONST, file);
	
	struct BinConst* hc = make(BinConst);
	
	read_super(hc);
	
	fread(hc, structsize(BinConst), 1, file);
	
	magic_num_require(MAGIC_END_BINCONST, file);

	return hc;
}

struct BoolConst* read_bool_const(FILE* file) {
	
	magic_num_require(MAGIC_BOOLCONST, file);
	
	struct BoolConst* b = make(BoolConst);
	
	read_super(b);
	
	fread(b, structsize(BoolConst), 1, file);
	
	magic_num_require(MAGIC_END_BOOLCONST, file);
	
	return b;
}
struct CharConst* read_char_const(FILE* file) {
	
	magic_num_require(MAGIC_CHARCONST, file);
	
	struct CharConst* b = make(CharConst);
	
	read_super(b);
	
	fread(b, structsize(CharConst), 1, file);
	
	magic_num_require(MAGIC_END_CHARCONST, file);
	
	return b;
}
struct FloatConst* read_float_const(FILE* file) {
	
	magic_num_require(MAGIC_FLOATCONST, file);
	
	struct FloatConst* ic = make(FloatConst);
	
	read_super(ic);
		
	fread(ic, structsize(FloatConst), 1, file);
	
	magic_num_require(MAGIC_END_FLOATCONST, file);
	
	return ic;
}
struct ConstValue* read_const_value(INFILE){

	magic_num_require(MAGIC_CONSTVALUE, file);

	struct ConstValue* res = make(ConstValue);

	read_super(res);

	res->kind = deserialize_int(file);

	res->ptr.m1_bool_const = NULL;

	switch(res->kind){
		case 1: res->ptr.m1_bool_const = read_bool_const(file); break;
		case 2: res->ptr.m2_int_const = read_int_const(file);  break;
		case 3: res->ptr.m3_char_const = read_char_const(file); break;
		case 4: res->ptr.m4_float_const = read_float_const(file); break;
		case 5: res->ptr.m5_hex_const = read_hex_const(file); break;
		case 6: res->ptr.m6_bin_const = read_bin_const(file); break;
		default:
			error(file, "Error in read_const_value");
	}

	magic_num_require(MAGIC_END_CONSTVALUE, file);

	return res;
}
struct StringConst* read_string_const(FILE* file) {
	
	magic_num_require(MAGIC_STRINGCONST, file);
	
	struct StringConst* s = make(StringConst);
	
	read_super(s);
	
	//doing this to avoid problems
	//with whitespace or any characters at all really
	s->value = deserialize_string(file);
	
	magic_num_require(MAGIC_END_STRINGCONST, file);
	
	return s;
}
struct Variable* read_variable(FILE* file) {
	
	magic_num_require(MAGIC_VARIABLE, file);
	
	struct Variable* v = make(Variable);
	
	read_super(v);
	
	v->member_access    = NULL;
	v->simple_var       = read_simple_var(file);

	const bool hasMemberAccess = deserialize_int(file) == OPT_PRESENT;
	
	if(hasMemberAccess){
		v->member_access = read_variable(file);
	}

	magic_num_require(MAGIC_END_VARIABLE, file);
	
	return v;
}
struct SimpleVar* read_simple_var(FILE* file) {
	
	magic_num_require(MAGIC_SIMPLEVAR, file);
	
	struct SimpleVar* b = make(SimpleVar);
	
	read_super(b);
	
	char* tmp = deserialize_string(file);
	strcpy(b->name, tmp);
	free(tmp);

	b->count_indices = deserialize_int(file);
	
	b->indices = malloc(sizeof(void*)* (b->count_indices+1));
	
	for(int i=0; i < b->count_indices; i++){
		b->indices[i] = read_expr(file);
	}
	
	magic_num_require(MAGIC_END_SIMPLEVAR, file);
	
	return b;
}
struct Term* read_term(FILE* file) {
	
	magic_num_require(MAGIC_TERM, file);
	
	struct Term* b = make(Term);
	
	read_super(b);
	
	b->kind = deserialize_int(file);

	switch(b->kind){

		case  4: b->ptr.m4  = read_call(file); 	    break;
		case  5: b->ptr.m5  = read_expr(file); 		break;
		case  6: b->ptr.m6  = read_variable(file); 	break;
		case  8: b->ptr.m8  = read_string_const(file); break;
		case 11: b->ptr.m11 = read_lambda(file);      break;
		case 12: b->ptr.m12 = read_const_value(file); break;
		
		default: error(file, "Error in read_term");
	}
	
	magic_num_require(MAGIC_END_TERM, file);
	
	return b;
}

struct UnOpTerm* read_un_op_term(FILE* file) {
	
	magic_num_require(MAGIC_UNOPTERM, file);
	
	struct UnOpTerm* t = make(UnOpTerm);
	
	read_super(t);
	
	const uint32_t opt = deserialize_int(file);
	
	t->op   = (opt == OPT_PRESENT) ? read_op(file) : NULL;
	t->term = read_term(file);
	
	magic_num_require(MAGIC_END_UNOPTERM, file);
	
	return t;
}

struct Range* read_range(FILE* file) {
	
	magic_num_require(MAGIC_RANGE, file);
	
	struct Range* r = make(Range);
	
	read_super(r);
	
	r->start = read_expr(file);
	r->end   = read_expr(file);
	
	magic_num_require(MAGIC_END_RANGE, file);
	
	return r;
}

struct Lambda* read_lambda(FILE* file) {
	
	magic_num_require(MAGIC_LAMBDA, file);
	
	struct Lambda* l = make(Lambda);
	
	read_super(l);
	
	l->count_params = deserialize_int(file);
	
	for(uint8_t i = 0; i < l->count_params; i++){
		
		struct Id* k = make(Id);
		strcpy(k->identifier, deserialize_string(file));
		
		l->params[i] = k;
	}
	
	l->expr = read_expr(file);
	
	magic_num_require(MAGIC_END_LAMBDA, file);
	
	return l;
}

//statementnodes
struct Stmt* read_stmt(FILE* file) {
	
	magic_num_require(MAGIC_STMT, file);
	
	struct Stmt* b = make(Stmt);
	
	read_super(b);
	
	b->kind = deserialize_int(file);

	switch(b->kind){
		case 99: 
			{
				b->is_break    = deserialize_int(file) == OPT_PRESENT;
				b->is_continue = deserialize_int(file) == OPT_PRESENT;
				b->is_throw    = deserialize_int(file) == OPT_PRESENT;
			}
			break;
		case 1: b->ptr.m1 = read_call(file);       break;
		case 2: b->ptr.m2 = read_while_stmt(file);  break;
		case 3: b->ptr.m3 = read_if_stmt(file);     break;
		case 4: b->ptr.m4 = read_ret_stmt(file);    break;
		case 5: b->ptr.m5 = read_assign_stmt(file); break;
		case 6: b->ptr.m6 = read_try_catch_stmt(file); break;
		case 7: b->ptr.m7 = read_for_stmt(file);  	 break;
		case 8: b->ptr.m8 = read_switch_stmt(file); break;
		default:
			error(file, "Error in read_stmt");
	}
	
	magic_num_require(MAGIC_END_STMT, file);
	
	return b;
}
struct IfStmt* read_if_stmt(FILE* file) {
	
	magic_num_require(MAGIC_IFSTMT, file);
	
	struct IfStmt* v = make(IfStmt);
	
	read_super(v);

	v->condition = read_expr(file);
	v->block     = read_stmt_block(file);
	v->else_block = NULL;

	const uint32_t hasElse = deserialize_int(file);
	
	if(hasElse == OPT_PRESENT){
		v->else_block = read_stmt_block(file);
	}
	
	magic_num_require(MAGIC_END_IFSTMT, file);
	
	return v;
}
struct WhileStmt* read_while_stmt(FILE* file) {
	
	magic_num_require(MAGIC_WHILESTMT, file);
	
	struct WhileStmt* v = make(WhileStmt);
	
	read_super(v);

	v->condition = read_expr(file);
	v->block     = read_stmt_block(file);
	
	magic_num_require(MAGIC_END_WHILESTMT, file);
	
	return v;
}
struct RetStmt* read_ret_stmt(FILE* file) {
	
	magic_num_require(MAGIC_RETSTMT, file);
	
	struct RetStmt* v = make(RetStmt);
	
	read_super(v);

	v->return_value = read_expr(file);
	
	magic_num_require(MAGIC_END_RETSTMT, file);
	
	return v;
}
struct AssignStmt* read_assign_stmt(FILE* file) {
	
	magic_num_require(MAGIC_ASSIGNSTMT, file);
	
	struct AssignStmt* v = make(AssignStmt);
	
	read_super(v);
	
	const uint32_t option = deserialize_int(file);

	if(option != OPT_EMPTY && option != OPT_PRESENT){
		
		error(file, "Error in read_assign_stmt\n");
	}

	v->opt_type = NULL;

	if(option == OPT_PRESENT){
		v->opt_type = read_type(file);
	}

	v->var = read_variable(file);
	
	char* assign_op = deserialize_string(file);
	
	strncpy(v->assign_op, assign_op, ASSIGNOP_LENGTH);
	
	free(assign_op);
	
	v->expr = read_expr(file);
	
	magic_num_require(MAGIC_END_ASSIGNSTMT, file);

	return v;
}
struct Call* read_call(FILE* file) {
	
	magic_num_require(MAGIC_CALL, file);
	
	struct Call* v = make(Call);
	
	read_super(v);

	char* tmp = deserialize_string(file);
	strcpy(v->name, tmp);
	free(tmp);
	
	v->count_args = deserialize_int(file);

	v->args = malloc(sizeof(void*)*(v->count_args));
	
	for(int i=0;i < (v->count_args);i++){
		v->args[i] = read_expr(file);
	}
	
	magic_num_require(MAGIC_END_CALL, file);
	
	return v;
}
struct ForStmt* read_for_stmt(FILE* file) {
	
	magic_num_require(MAGIC_FORSTMT, file);
	
	struct ForStmt* res = make(ForStmt);
	
	read_super(res);
	
	char* indexName = deserialize_string(file);
	
	strncpy(res->index_name, indexName, DEFAULT_STR_SIZE);
	free(indexName);
	
	res->range = read_range(file);
	res->block = read_stmt_block(file);
	
	magic_num_require(MAGIC_END_FORSTMT, file);
	
	return res;
}
struct SwitchStmt* read_switch_stmt(FILE* file) {
	
	magic_num_require(MAGIC_SWITCHSTMT, file);
	
	struct SwitchStmt* res = make(SwitchStmt);
	
	read_super(res);
	
	res->expr = read_expr(file);
	
	res->count_cases = deserialize_int(file);

	res->cases = malloc(sizeof(void*)* (res->count_cases));
	
	for(int i=0; i < res->count_cases; i++){
		
		res->cases[i] = read_case_stmt(file);
	}
	
	magic_num_require(MAGIC_END_SWITCHSTMT, file);
	
	return res;
}
struct CaseStmt* read_case_stmt(FILE* file) {
	
	magic_num_require(MAGIC_CASESTMT, file);
	
	struct CaseStmt* res = make(CaseStmt);
	
	read_super(res);

	res->const_value = read_const_value(file);

	const uint32_t hasBlock = deserialize_int(file);
	
	if(hasBlock == OPT_PRESENT){
		
		res->block = read_stmt_block(file);
	}
	
	magic_num_require(MAGIC_END_CASESTMT, file);
	
	return res;
}
struct TryCatchStmt* read_try_catch_stmt(FILE* file) {
	
	magic_num_require(MAGIC_TRYCATCHSTMT, file);
	
	struct TryCatchStmt* res = make(TryCatchStmt);
	
	res->try_block   = read_stmt_block(file);
	res->catch_block = read_stmt_block(file);
	
	magic_num_require(MAGIC_END_TRYCATCHSTMT, file);
	
	return res;
}
// --- typenodes -------------------------
struct Type* read_type(FILE* file) {
	
	magic_num_require(MAGIC_TYPE, file);
	
	struct Type* b = make(Type);
	
	read_super(b);
	
	const uint32_t kind = deserialize_int(file);
	
	b->m1 = NULL;
	b->m2 = NULL;
	b->m3 = NULL;

	switch(kind){
		case 1: b->m1 = read_basic_type(file); break;
		case 2: b->m2 = read_type_param(file); break;
		case 3: b->m3 = read_array_type(file); break;
		default:
			error(file, "Error in read_type");
	}
	
	magic_num_require(MAGIC_END_TYPE, file);
	
	return b;
}
struct SubrType* read_subr_type(FILE* file) {
	
	magic_num_require(MAGIC_SUBRTYPE, file);
	
	struct SubrType* v = make(SubrType);
	
	read_super(v);
	
	v->return_type = read_type(file);
	
	v->has_side_effects = deserialize_int(file);
	v->throws         = deserialize_int(file);
	
	v->count_arg_types = deserialize_int(file);
	
	v->arg_types = malloc(sizeof(void*) * (v->count_arg_types));
	
	for(int i=0;i < (v->count_arg_types); i++){
		v->arg_types[i] = read_type(file);
	}
	
	magic_num_require(MAGIC_END_SUBRTYPE, file);
	
	return v;
}
struct SimpleType* read_simple_type(FILE* file) {
	
	magic_num_require(MAGIC_SIMPLETYPE, file);
	
	struct SimpleType* v = make(SimpleType);
	
	read_super(v);
	
	v->primitive_type = NULL;
	v->struct_type    = NULL;
	
	const uint32_t kind = deserialize_int(file);
	
	switch(kind){
		
		case 0: v->primitive_type = read_primitive_type(file);
			break;
			
		case 1: v->struct_type = read_struct_type(file);
			break;
			
		default:
			error(file, " kind not valid ");
	}
	
	magic_num_require(MAGIC_END_SIMPLETYPE, file);
	
	return v;
}
struct ArrayType* read_array_type(FILE* file) {
	
	magic_num_require(MAGIC_ARRAYTYPE, file);
	
	struct ArrayType* v = make(ArrayType);
	
	read_super(v);
	
	v->element_type = read_type(file);
	
	magic_num_require(MAGIC_END_ARRAYTYPE, file);
	
	return v;
}
struct TypeParam* read_type_param(FILE* file) {
	
	magic_num_require(MAGIC_TYPEPARAM, file);
	
	struct TypeParam* v = make(TypeParam);
	
	read_super(v);
		
	fread(v, structsize(TypeParam), 1, file);
	
	magic_num_require(MAGIC_END_TYPEPARAM, file);
	
	return v;
}
struct BasicType* read_basic_type(FILE* file) {
	
	magic_num_require(MAGIC_BASICTYPE, file);
	
	struct BasicType* v = make(BasicType);
	
	read_super(v);

	v->simple_type = NULL;
	v->subr_type   = NULL;
	
	const uint32_t kind = deserialize_int(file);
	
	switch(kind){

		case 1: v->simple_type = read_simple_type(file); break;
		case 2: v->subr_type   = read_subr_type(file);   break;

		default:
			error(file, "Error in read_basic_type\n");
	}
	
	magic_num_require(MAGIC_END_BASICTYPE, file);
	
	return v;
}

struct StructType* read_struct_type(FILE* file) {
	
	magic_num_require(MAGIC_STRUCTTYPE, file);
	
	struct StructType* res = make(StructType);
	
	read_super(res);
	
	char* tmp = deserialize_string(file);
	strcpy(res->type_name, tmp);
	free(tmp);
	
	res->count_type_params = deserialize_int(file);
	
	if(res->count_type_params > 0){
		res->type_params = malloc(sizeof(uint8_t) * (res->count_type_params));
	}
	
	for(int i = 0; i < res->count_type_params; i++){
		
		res->type_params[i] = deserialize_int(file);
	}

	magic_num_require(MAGIC_END_STRUCTTYPE, file);
	
	return res;
}

struct PrimitiveType* read_primitive_type(FILE* file) {
	
	magic_num_require(MAGIC_PRIMITIVETYPE, file);
	
	struct PrimitiveType* res = make(PrimitiveType);
	
	read_super(res);
	
	res->is_int_type   = deserialize_int(file);
	res->is_float_type = deserialize_int(file);
	res->is_char_type  = deserialize_int(file);
	res->is_bool_type  = deserialize_int(file);
	
	res->int_type     = deserialize_int(file);
	
	magic_num_require(MAGIC_END_PRIMITIVETYPE, file);
	
	return res;
}

