#ifndef JAVA_CODE_GEN
#define JAVA_CODE_GEN

#include "../ast/ast.h"

int gen_java_code(struct AST_Whole_Program* ast, char* fname_out);

void gen_java_namespace(struct Namespace* ns, FILE* file);

void gen_java_method(struct Method* m, FILE* file);

//-----------------------

void gen_java_declarg(struct DeclArg* da, FILE* file);

void gen_java_type(struct Type* type, FILE* file);

void gen_java_stmt(struct Stmt* stmt, FILE* file);
	
void gen_java_methodcall(struct MethodCall* m, FILE* file);
void gen_java_whilestmt(struct WhileStmt* m, FILE* file);
void gen_java_ifstmt(struct IfStmt* m, FILE* file);
void gen_java_retstmt(struct RetStmt* m, FILE* file);
void gen_java_assignstmt(struct AssignStmt* m, FILE* file);

void gen_java_var(struct Variable* m, FILE* file);
void gen_java_expr(struct Expr* m, FILE* file);
void gen_java_simplevar(struct SimpleVar* m, FILE* file);

void gen_java_op(struct Op* m, FILE* file);
void gen_java_term(struct Term* t, FILE* file);

// const code generators
void gen_java_boolconst(struct BoolConst* m, FILE* file);
void gen_java_intconst(struct IntConst* m, FILE* file);
void gen_java_charconst(struct CharConst* m, FILE* file);
void gen_java_floatconst(struct FloatConst* m, FILE* file);

//type stuff

void gen_java_basictypewrapped(struct BasicTypeWrapped* m, FILE* file);
void gen_java_typeparam(struct TypeParam* m, FILE* file);
void gen_java_arraytype(struct ArrayType* m, FILE* file);

void gen_java_simpleType(struct SimpleType* m, FILE* file);
void gen_java_subrType(struct SubrType* m, FILE* file);
#endif
