#ifndef SMALLDRAGON__CG_H
#define SMALLDRAGON__CG_H

struct Ctx;

/*
 * this is the internal Header for the functions generating c code.
 *
 * It should only be included in files in the transpiler/main/code_gen/c_code_gen Module
 */


//const nodes related
void transpileBoolConst		(struct BoolConst* bc, 	struct Ctx* ctx);
void transpileIntConst		(struct IntConst* ic, 	struct Ctx* ctx);
void transpileHexConst		(struct HexConst* hc, 	struct Ctx* ctx);
void transpileBinConst		(struct BinConst* hc, 	struct Ctx* ctx);
void transpileCharConst		(struct CharConst* cc, 	struct Ctx* ctx);
void transpileFloatConst	(struct FloatConst* fc, struct Ctx* ctx);
void transpileStringConst	(struct StringConst* s, struct Ctx* ctx);

void transpileConstValue	(struct ConstValue* c, struct Ctx* ctx);

void transpileExpr(struct Expr* expr, struct Ctx* ctx);

void transpileOp(struct Op* op, struct Ctx* ctx);

void transpileTerm(struct Term* expr, struct Ctx* ctx);

void transpileUnOpTerm(struct UnOpTerm* t, struct Ctx* ctx);

void transpileAssignStmt(struct AssignStmt* as, struct Ctx* ctx);

void transpileCall(struct Call* mc, struct Ctx* ctx);

void transpileForStmt(struct ForStmt* f, struct Ctx* ctx);

void transpileIfStmt(struct IfStmt* is, struct Ctx* ctx);

void transpileLoopStmt(struct LoopStmt* ls, struct Ctx* ctx);


void transpileStmt(struct Stmt* s, struct Ctx* ctx);


void transpileCall(struct Call* mc, struct Ctx* ctx);
void transpileWhileStmt(struct WhileStmt* ws, struct Ctx* ctx);
void transpileIfStmt(struct IfStmt* is, struct Ctx* ctx);
void transpileRetStmt(struct RetStmt* rs, struct Ctx* ctx);
void transpileAssignStmt(struct AssignStmt* as, struct Ctx* ctx);
void transpileLoopStmt(struct LoopStmt* ls, struct Ctx* ctx);
void transpileBreakStmt(struct Ctx* ctx);
void transpileContinueStmt(struct Ctx* ctx);
void transpileForStmt(struct ForStmt* f, struct Ctx* ctx);
void transpileSwitchStmt(struct SwitchStmt* s, struct Ctx* ctx);
void transpileCaseStmt(struct CaseStmt* s, struct Ctx* ctx);
void transpileTryCatchStmt(struct TryCatchStmt* tcs, struct Ctx* ctx);
void transpileWhileStmt(struct WhileStmt* ws, struct Ctx* ctx);

void transpileSwitchStmt(struct SwitchStmt* s, struct Ctx* ctx);
void transpileCaseStmt(struct CaseStmt* s, struct Ctx* ctx);


void transpileThrowStmt(struct Ctx* ctx);

void transpileTryCatchStmt(struct TryCatchStmt* tcs, struct Ctx* ctx);



void transpileStructDecl(struct StructDecl* s, struct Ctx* ctx);
void transpileStructMember(struct StructMember* m, struct Ctx* ctx);


void transpileMethod(struct Method* m, struct Ctx* ctx);
void transpileMethodSignature(struct Method* m, struct Ctx* ctx);
void transpileDeclArg(struct DeclArg* da, struct Ctx* ctx);


//Types

void transpileType     (struct Type* t, struct Ctx* ctx);
void transpileBasicType(struct BasicType* btw, struct Ctx* ctx);
void transpileTypeParam(struct TypeParam* tp, struct Ctx* ctx);
void transpileArrayType(struct ArrayType* atype, struct Ctx* ctx);
void transpileSimpleType(struct SimpleType* simpleType, struct Ctx* ctx);
void transpileSubrType  (struct SubrType* subrType, struct Ctx* ctx);

//Var
void transpileVariable(struct Variable* var, struct Ctx* ctx);
void transpileSimpleVar(struct SimpleVar* svar, struct Ctx* ctx);


#endif
