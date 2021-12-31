#ifndef SMALLDRAGON_TOPLEVEL_TAC_H
#define SMALLDRAGON_TOPLEVEL_TAC_H

#include "tacbuffer.h"
#include "ast/ast_declare.h"

enum TAC_KIND{
    TAC_NONE = -1,

    TAC_BINARY_OP = 0, //e.g. t1 = t2 + t3
    TAC_UNARY_OP, //e.g. t1 = -t2;

    TAC_GOTO,
    TAC_IF_GOTO,

    TAC_DEREF,

    TAC_COPY,
    TAC_CONST_VALUE,

    TAC_CALL,
    TAC_PARAM,
    TAC_RETURN,

    TAC_NOP,
};

#define TEMP_SIZE 20
#define TAC_NO_LABEL 0

struct TAC{
    uint32_t index; //for when we put it in a TACBuffer

    //three address code
    uint32_t label_index;
    char label_name[DEFAULT_STR_SIZE]; //for labels with names, like functions

    char dest[TEMP_SIZE];
    enum TAC_KIND kind;
    char arg1[TEMP_SIZE];
    char op[TEMP_SIZE];
    char arg2[TEMP_SIZE];

    uint32_t goto_index;

    int32_t const_value;
};

//TAC functions
void print_tac(struct TAC* tac);
char* tac_tostring(struct TAC* tac);
bool tac_is_unconditional_jump(struct TAC* tac);

struct TAC* makeTAC();
uint32_t make_label();
uint32_t make_temp();
struct TAC* makeTACLabel(uint32_t label);
struct TAC* makeTACGoto(uint32_t label);

void tac_call(struct TACBuffer* buffer, struct Call* call);
void tac_whilestmt(struct TACBuffer* buffer, struct WhileStmt* w);
void tac_ifstmt(struct TACBuffer* buffer, struct IfStmt* s);
void tac_retstmt(struct TACBuffer* buffer, struct RetStmt* r);
void tac_assignstmt(struct TACBuffer* buffer, struct AssignStmt* a);
void tac_forstmt(struct TACBuffer* buffer, struct ForStmt* f);

void tac_method(struct TACBuffer* buffer, struct Method* m);
void tac_stmtblock(struct TACBuffer* buffer, struct StmtBlock* block);
void tac_stmt(struct TACBuffer* buffer, struct Stmt* stmt);

void tac_expr(struct TACBuffer* buffer, struct Expr* expr);
void tac_unopterm(struct TACBuffer* buffer, struct UnOpTerm* t);
void tac_term(struct TACBuffer* buffer, struct Term* t);

void tac_variable(struct TACBuffer* buffer, struct Variable* v);
void tac_simplevar(struct TACBuffer* buffer, struct SimpleVar* sv);

#endif
