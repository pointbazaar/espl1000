#ifndef ASTWRITER
#define ASTWRITER

#include "../ast.h"

#include <stdlib.h>
#include <stdio.h>

#define OUTFILE FILE* file

void write_ast(struct AST* ast);

// --- toplevel structures ---------

void write_namespace(struct Namespace* m);

void write_method(struct Method* m, OUTFILE);
void write_method_decl(struct MethodDecl* m, OUTFILE);
void write_struct_decl(struct StructDecl* m, OUTFILE);
void write_struct_member(struct StructMember* m, OUTFILE);
void write_externc(struct ExternC* ec, OUTFILE);

void write_stmt_block(struct StmtBlock* block, OUTFILE);

// -----------------------------------

void write_decl_arg(struct DeclArg* m, OUTFILE);
void write_expr(struct Expr* m, OUTFILE);
void write_op(struct Op* m, OUTFILE);

//const nodes
void write_int_const(struct IntConst* m, OUTFILE);
void write_hex_const(struct HexConst* m, OUTFILE);
void write_bin_const(struct BinConst* m, OUTFILE);
void write_bool_const(struct BoolConst* m, OUTFILE);
void write_char_const(struct CharConst* m, OUTFILE);
void write_float_const(struct FloatConst* m, OUTFILE);
void write_const_value(struct ConstValue* m, OUTFILE);
void write_string_const(struct StringConst* m, OUTFILE);

//other 
void write_variable(struct Variable* m, OUTFILE);
void write_simple_var(struct SimpleVar* m, OUTFILE);
void write_term(struct Term* m, OUTFILE);
void write_un_op_term(struct UnOpTerm* t, OUTFILE);
void write_range(struct Range* r, OUTFILE);
void write_lambda(struct Lambda* l, OUTFILE);
void write_mdirect(struct MDirect* m, OUTFILE);

//statementnodes
void write_stmt(struct Stmt* m, OUTFILE);
void write_if_stmt(struct IfStmt* m, OUTFILE);
void write_while_stmt(struct WhileStmt* m, OUTFILE);
void write_ret_stmt(struct RetStmt* m, OUTFILE);
void write_assign_stmt(struct AssignStmt* m, OUTFILE);
void write_call(struct Call* m, OUTFILE);
void write_for_stmt(struct ForStmt* m, OUTFILE);
void write_switch_stmt(struct SwitchStmt* m, OUTFILE);
void write_case_stmt(struct CaseStmt* m, OUTFILE);
void write_massign_stmt(struct MAssignStmt* m, OUTFILE);

//typenodes
void write_type(struct Type* m, OUTFILE);
void write_subr_type(struct SubrType* m, OUTFILE);
void write_simple_type(struct SimpleType* m, OUTFILE);
void write_array_type(struct ArrayType* m, OUTFILE);
void write_type_param(struct TypeParam* m, OUTFILE);
void write_basic_type(struct BasicType* m, OUTFILE);
void write_struct_type(struct StructType* m, OUTFILE);
void write_primitive_type(struct PrimitiveType* m, OUTFILE);

#endif
