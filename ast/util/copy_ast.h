#pragma once

#include "../ast.h"

//this header file declares all subroutines
//for copying the memory allocated for AST Nodes.

//the subroutines here perform
//DEEP COPIES
//as this is sometimes required for AST Nodes

//const
struct ConstValue* copy_const_value(struct ConstValue* cv);
struct StringConst* copy_string_const(struct StringConst* s);

//stmt
struct Call* copy_call(struct Call* c);
struct IfStmt* copy_if_stmt(struct IfStmt* i);
struct AssignStmt* copy_assign_stmt(struct AssignStmt* a);
struct RetStmt* copy_ret_stmt(struct RetStmt* r);
struct Stmt* copy_stmt(struct Stmt* stmt);
struct WhileStmt* copy_while_stmt(struct WhileStmt* w);
struct ForStmt* copy_for_stmt(struct ForStmt* f);
struct LocalVarDeclStmt* copy_local_var_decl_stmt(struct LocalVarDeclStmt* l);

//c_types_util
struct Type* copy_type(struct Type* t);
struct TypeParam* copy_type_param(struct TypeParam* t);
struct SubrType* copy_subr_type(struct SubrType* s);
struct SimpleType* copy_simple_type(struct SimpleType* s);
struct StructType* copy_struct_type(struct StructType* s);
struct PrimitiveType* copy_primitive_type(struct PrimitiveType* p);
struct BasicType* copy_basic_type(struct BasicType* b);
struct ArrayType* copy_array_type(struct ArrayType* a);
struct PointerType* copy_pointer_type(struct PointerType* a);

//expr
struct Expr* copy_expr(struct Expr* expr);
// @returns NULL on error
struct Term* copy_term(struct Term* t);
struct AddressOf* copy_address_of(struct AddressOf* ao);
struct Deref* copy_deref(struct Deref* d);
struct UnOpTerm* copy_un_op_term(struct UnOpTerm* t);
//struct Op*       copy_op(struct Op* op);
struct LValue* copy_lvalue(struct LValue* lv);

//var
struct SimpleVar* copy_simple_var(struct SimpleVar* sv);
struct Variable* copy_variable(struct Variable* var);

//other
struct Id* copy_identifier(struct Id* id);
struct Range* copy_range(struct Range* r);
struct StmtBlock* copy_stmt_block(struct StmtBlock* s);
