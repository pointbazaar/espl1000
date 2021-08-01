#ifndef COPYAST
#define COPYAST

#include "../ast.h"

//this header file declares all subroutines
//for copying the memory allocated for AST Nodes.

//the subroutines here perform 
//DEEP COPIES
//as this is sometimes required for AST Nodes

//const
struct BoolConst*   copy_bool_const(struct BoolConst* bc);
struct CharConst*   copy_char_const(struct CharConst* cc);
struct FloatConst*  copy_float_const(struct FloatConst* fc);
struct IntConst*    copy_int_const(struct IntConst* ic);
struct HexConst*    copy_hex_const(struct HexConst* hc);
struct BinConst*    copy_bin_const(struct BinConst* hc);
struct ConstValue*  copy_const_value(struct ConstValue* cv);
struct StringConst* copy_string_const(struct StringConst* s);

//stmt
struct Call*  		copy_call(struct Call* c);
struct IfStmt*      copy_if_stmt(struct IfStmt* i);
struct AssignStmt*  copy_assign_stmt(struct AssignStmt* a);
struct RetStmt*     copy_ret_stmt(struct RetStmt* r);
struct Stmt*        copy_stmt(struct Stmt* stmt);
struct WhileStmt*   copy_while_stmt(struct WhileStmt* w);
struct LoopStmt*    copy_loop_stmt(struct LoopStmt* l);
struct ForStmt*     copy_for_stmt(struct ForStmt* f);
struct SwitchStmt*  copy_switch_stmt(struct SwitchStmt* s);
struct CaseStmt*    copy_case_stmt(struct CaseStmt* c);
struct TryCatchStmt* copy_try_catch_stmt(struct TryCatchStmt* tcs);

//types
struct Type*             copy_type(struct Type* t);
struct TypeParam*        copy_type_param(struct TypeParam* t);
struct SubrType*         copy_subr_type(struct SubrType* s);
struct SimpleType*       copy_simple_type(struct SimpleType* s);
struct StructType*       copy_struct_type(struct StructType* s);
struct PrimitiveType*    copy_primitive_type(struct PrimitiveType* p);
struct BasicType*        copy_basic_type(struct BasicType* b);
struct ArrayType*        copy_array_type(struct ArrayType* a);

//expr
struct Expr*     copy_expr(struct Expr* expr);
struct Term*     copy_term(struct Term* t);
struct UnOpTerm* copy_un_op_term(struct UnOpTerm* t);
struct Op*       copy_op(struct Op* op);

//var
struct SimpleVar* copy_simple_var(struct SimpleVar* sv);
struct Variable*  copy_variable(struct Variable* var);

//other
struct Identifier* copy_identifier(struct Identifier* id);
struct Range*      copy_range(struct Range* r);
struct StmtBlock*  copy_stmt_block(struct StmtBlock* s);

struct Lambda*     copy_lambda(struct Lambda* l);

#endif
