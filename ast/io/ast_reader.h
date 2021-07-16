#ifndef ASTREADER
#define ASTREADER

#include "../ast.h"
#include <stdbool.h>

#define INFILE FILE* file

struct AST* read_ast(char** filenames, int count_filenames);

//toplevel
struct Namespace* 		read_namespace(INFILE);
struct Method* 			read_method(INFILE);
struct StructDecl* 		read_struct_decl(INFILE);
struct StructMember* 	read_struct_member(INFILE);

struct StmtBlock* read_stmt_block(INFILE);

struct DeclArg* 	read_decl_arg(INFILE);
struct Expr* 		read_expr(INFILE);
struct Op* 			read_op(INFILE);

//const nodes
struct IntConst* 	read_int_const(INFILE);
struct HexConst* 	read_hex_const(INFILE);
struct BinConst* 	read_bin_const(INFILE);
struct BoolConst* 	read_bool_const(INFILE);
struct CharConst* 	read_char_const(INFILE);
struct FloatConst* 	read_float_const(INFILE);
struct ConstValue*  read_const_value(INFILE);
struct StringConst* read_string_const(INFILE);

//other
struct Variable* 	read_variable(INFILE);
struct SimpleVar* 	read_simple_var(INFILE);
struct Term* 		read_term(INFILE);
struct UnOpTerm* 	read_un_op_term(INFILE);
struct Range* 		read_range(INFILE);
struct Lambda* 		read_lambda(INFILE);

//statementnodes
struct Stmt* 		read_stmt(INFILE);
struct IfStmt* 		read_if_stmt(INFILE);
struct WhileStmt* 	read_while_stmt(INFILE);
struct RetStmt* 	read_ret_stmt(INFILE);
struct AssignStmt* 	read_assign_stmt(INFILE);
struct Call* 		read_call(INFILE);
struct LoopStmt* 	read_loop_stmt(INFILE);
struct ForStmt* 	read_for_stmt(INFILE);
struct SwitchStmt* 	read_switch_stmt(INFILE);
struct CaseStmt* 	read_case_stmt(INFILE);
struct TryCatchStmt* read_try_catch_stmt(INFILE);

//typenodes
struct Type* 			read_type(INFILE);
struct SubrType* 		read_subr_type(INFILE);
struct SimpleType*		read_simple_type(INFILE);
struct ArrayType* 		read_array_type(INFILE);
struct TypeParam*		read_type_param(INFILE);
struct BasicType* 		read_basic_type(INFILE);
struct StructType* 		read_struct_type(INFILE);
struct PrimitiveType* 	read_primitive_type(INFILE);

#endif
