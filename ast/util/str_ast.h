#pragma once

#include "../ast.h"

//this header file declares all subroutines
//for providing string representations
//of AST nodes. Meaning to convert them
//to their source code form.

//all char* returned are heap-allocated

//const
// @returns NULL on error
char* str_const_value(struct ConstValue* cv);
// @returns NULL on error
char* str_string_const(struct StringConst* s);
// @returns NULL on error
char* str_bin_const(uint32_t b);

//c_types_util
// @returns NULL on error
char* str_array_type(struct ArrayType* at);
// @returns NULL on error
char* str_pointer_type(struct PointerType* pt);
// @returns NULL on error
char* str_basic_type(struct BasicType* btw);
// @returns NULL on error
char* str_simple_type(struct SimpleType* st);
// @returns NULL on error
char* str_subr_type(struct SubrType* st);
// @returns NULL on error
char* str_type(struct Type* t);
// @returns NULL on error
char* str_type_param(struct TypeParam* t);
// @returns NULL on error
char* str_primitive_type(struct PrimitiveType* p);
// @returns NULL on error
char* str_struct_type(struct StructType* s);

//struct
// @returns NULL on error
char* str_struct_decl(struct StructDecl* decl);
// @returns NULL on error
char* str_struct_member(struct StructMember* s);

//var
// @returns NULL on error
char* str_struct_decl(struct StructDecl* decl);
// @returns NULL on error
char* str_variable(struct Variable* v);
// @returns NULL on error
char* str_simple_var(struct SimpleVar* s);

//expr
// @returns NULL on error
char* str_expr(struct Expr* e);
// @returns NULL on error
char* str_op(enum OP o);
// @returns NULL on error
char* str_un_op_term(struct UnOpTerm* u);
// @returns NULL on error
char* str_address_of(struct AddressOf* ao);
// @returns NULL on error
char* str_deref(struct Deref* d);
// @returns NULL on error
char* str_term(struct Term* t);
// @returns NULL on error
char* str_mdirect(struct MDirect* m);

//stmts
// @returns NULL on error
char* str_stmt(struct Stmt* stmt);

// @returns NULL on error
char* str_assign_stmt(struct AssignStmt* a);
// @returns NULL on error
char* str_for_stmt(struct ForStmt* f);
// @returns NULL on error
char* str_if_stmt(struct IfStmt* i);
// @returns NULL on error
char* str_while_stmt(struct WhileStmt* w);
// @returns NULL on error
char* str_call(struct Call* m);
// @returns NULL on error
char* str_ret_stmt(struct RetStmt* r);
// @returns NULL on error
char* str_massign_stmt(struct MAssignStmt* m);

//other
// @returns NULL on error
char* str_identifier(struct Id* id);
// @returns NULL on error
char* str_range(struct Range* r);
// @returns NULL on error
char* str_stmt_block(struct StmtBlock* block);
