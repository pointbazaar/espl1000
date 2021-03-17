#ifndef ASTREADER
#define ASTREADER

#include "../ast.h"
#include <stdbool.h>

struct AST* 		readAST(char** filenames, int count_filenames, bool debug);

//toplevel
struct Namespace* 	readNamespace(	FILE* file, bool debug);
struct Method* 		readMethod(		FILE* file, bool debug);
struct StructDecl* 	readStructDecl(	FILE* file, bool debug);
struct StructMember* readStructMember(FILE* file, bool debug);

struct StmtBlock* 	readStmtBlock(	FILE* file, bool debug);

struct DeclArg* 	readDeclArg(	FILE* file, bool debug);
struct Expr* 		readExpr(		FILE* file, bool debug);
struct Op* 			readOp(			FILE* file, bool debug);

//const nodes
struct IntConst* 	readIntConst(	FILE* file, bool debug);
struct HexConst* 	readHexConst(	FILE* file, bool debug);
struct BinConst* 	readBinConst(	FILE* file, bool debug);
struct BoolConst* 	readBoolConst(	FILE* file, bool debug);
struct CharConst* 	readCharConst(	FILE* file, bool debug);
struct FloatConst* 	readFloatConst(	FILE* file, bool debug);
struct StringConst* 	readStringConst(FILE* file, bool debug);

struct Variable* 	readVariable(	FILE* file, bool debug);
struct SimpleVar* 	readSimpleVar(	FILE* file, bool debug);
struct Term* 		readTerm(		FILE* file, bool debug);
struct UnOpTerm* 	readUnOpTerm(	FILE* file, bool debug);
struct Range* 		readRange(		FILE* file, bool debug);

//statementnodes
struct Stmt* 		readStmt(		FILE* file, bool debug);
struct IfStmt* 		readIfStmt(		FILE* file, bool debug);
struct WhileStmt* 	readWhileStmt(	FILE* file, bool debug);
struct RetStmt* 	readRetStmt(	FILE* file, bool debug);
struct AssignStmt* 	readAssignStmt(	FILE* file, bool debug);
struct Call* 	    readCall(	    FILE* file, bool debug);
struct LoopStmt* 	readLoopStmt(	FILE* file, bool debug);
struct ForStmt* 	readForStmt(	FILE* file, bool debug);
struct SwitchStmt* 	readSwitchStmt(	FILE* file, bool debug);
struct CaseStmt* 	readCaseStmt(	FILE* file, bool debug);

//typenodes
struct Type* 		readType(		FILE* file, bool debug);
struct SubrType* 	readSubrType(	FILE* file, bool debug);
struct SimpleType* 	readSimpleType(	FILE* file, bool debug);
struct ArrayType* 	readArrayType(	FILE* file, bool debug);
struct TypeParam* 	readTypeParam(	FILE* file, bool debug);
struct BasicTypeWrapped* readBasicTypeWrapped(FILE* file, bool debug);
struct StructType*  readStructType( FILE* file, bool debug);
struct PrimitiveType* readPrimitiveType(FILE* file, bool debug);

#endif
