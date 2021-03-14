#ifndef C_CODE_GEN_STMTS_H
#define C_CODE_GEN_STMTS_H

//stmt related

void transpileStmt(struct Stmt* s, struct Ctx* ctx);


void transpileMethodCall(struct MethodCall* mc, struct Ctx* ctx);
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


#endif
