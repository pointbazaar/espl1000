#ifndef AST_DECLARE_H
#define AST_DECLARE_H

#define DEFAULT_STR_SIZE 32 

//--- TOPLEVEL ---
struct AST;
struct Namespace;
struct Method;
struct StructDecl;
//---------------
struct StmtBlock;
struct Op;
struct Term;
struct Expr;
struct UnOpTerm;
struct Identifier;
struct Variable;
struct SimpleVar;
struct Range;
struct DeclArg;
//const
struct BoolConst;
struct CharConst;
struct IntConst;
struct HexConst;
struct BinConst;
struct FloatConst;
struct StringConst;
//statements
struct AssignStmt;
struct IfStmt;
struct Call;
struct RetStmt;
struct Stmt;
struct WhileStmt;
struct LoopStmt;
struct BreakStmt;
struct ForStmt;
struct SwitchStmt;
struct CaseStmt;
//typenodes
struct Type;
struct ArrayType;
struct BasicTypeWrapped;
struct SimpleType;
struct SubrType;
struct TypeParam;

#endif
