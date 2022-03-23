#ifndef STR_AST_H
#define STR_AST_H

#include "../ast.h"

//this header file declares all subroutines
//for providing string representations
//of AST nodes. Meaning to convert them
//to their source code form.

//all char* returned are heap-allocated

//const
char* str_bool_const(struct BoolConst* bc);
char* str_char_const(struct CharConst* cc);
char* str_float_const(struct FloatConst* fc);
char* str_int_const(struct IntConst* ic);
char* str_hex_const(struct HexConst* hc);
char* str_const_value(struct ConstValue* cv);
char* str_string_const(struct StringConst* s);
char* str_bin_const(struct BinConst* b);

//c_types_util
char* str_array_type(struct ArrayType* at);
char* str_basic_type(struct BasicType* btw);
char* str_simple_type(struct SimpleType* st);
char* str_subr_type(struct SubrType* st);
char* str_type(struct Type* t);
char* str_type_param(struct TypeParam* t);
char* str_primitive_type(struct PrimitiveType* p);
char* str_struct_type(struct StructType* s);

//struct
char* str_struct_decl(struct StructDecl* decl);
char* str_struct_member(struct StructMember* s);

//var
char* str_variable(struct Variable* v);
char* str_simple_var(struct SimpleVar* s);

//expr
char* str_expr(struct Expr* e);
char* str_op(struct Op* o);
char* str_un_op_term(struct UnOpTerm* u);
char* str_term(struct Term* t);

//stmts
char* str_stmt(struct Stmt* stmt);

char* str_assign_stmt(struct AssignStmt* a);
char* str_for_stmt(struct ForStmt* f);
char* str_if_stmt(struct IfStmt* i);
char* str_while_stmt(struct WhileStmt* w);
char* str_switch_stmt(struct SwitchStmt* s);
char* str_call(struct Call* m);
char* str_ret_stmt(struct RetStmt* r);
char* str_case_stmt(struct CaseStmt* c);

//other
char* str_identifier(struct Id* id);
char* str_range(struct Range* r);
char* str_stmt_block(struct StmtBlock* block);
char* str_lambda(struct Lambda* l);

#endif
