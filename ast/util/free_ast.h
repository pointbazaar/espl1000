#pragma once

#include "../ast.h"

//this header file declares all subroutines
//for freeing the memory allocated for AST Nodes.

//these subroutines are separated from the
//constructor subroutines in the parser,
//as other modules (compiler, etc.)
//must also be able to free AST Nodes,
//as they could obtain such via ast_reader.c

void free_ast(struct AST* ast);

//const
void free_const_value(struct ConstValue* cv);
void free_string_const(struct StringConst* s);

//subr
void free_decl_arg(struct DeclArg* da);
void free_method(struct Method* m);
void free_method_decl(struct MethodDecl* m);

//other
void free_identifier(struct Id* id);
void free_namespace(struct Namespace* ns);
void free_stmt_block(struct StmtBlock* block);
void free_range(struct Range* range);

//struct
void free_struct_decl(struct StructDecl* sd);
void free_struct_member(struct StructMember* sm);

//expr
void free_expr(struct Expr* expr);
// @returns false on error
bool free_term(struct Term* t);
void free_un_op_term(struct UnOpTerm* t);
void free_address_of(struct AddressOf* ao);
void free_deref(struct Deref* d);
void free_lvalue(struct LValue* lv);

//var
void free_variable(struct Variable* var);
void free_simple_var(struct SimpleVar* sv);

//stmts
void free_assign_stmt(struct AssignStmt* as);
void free_if_stmt(struct IfStmt* is);
void free_call(struct Call* mc);
void free_ret_stmt(struct RetStmt* rs);
// @returns false on error
bool free_stmt(struct Stmt* s);
void free_while_stmt(struct WhileStmt* ws);
void free_for_stmt(struct ForStmt* fstmt);
void free_local_var_decl_stmt(struct LocalVarDeclStmt* m);

//c_types_util
void free_array_type(struct ArrayType* at);
void free_pointer_type(struct PointerType* pt);
void free_basic_type(struct BasicType* btw);
void free_simple_type(struct SimpleType* st);
void free_subr_type(struct SubrType* st);
void free_type(struct Type* t);
void free_type_param(struct TypeParam* tp);
void free_primitive_type(struct PrimitiveType* p);
void free_struct_type(struct StructType* s);
