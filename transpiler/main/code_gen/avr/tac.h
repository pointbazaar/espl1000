#ifndef SMALLDRAGON_TOPLEVEL_TAC_H
#define SMALLDRAGON_TOPLEVEL_TAC_H

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

extern size_t cap;
extern size_t count;
extern struct TAC** tac;

void append_tac(struct TAC* node);
void print_tac(struct TAC* tac);

struct TAC* makeTAC();
uint32_t make_label();
uint32_t make_temp();
struct TAC* makeTACLabel(uint32_t label);
struct TAC* makeTACGoto(uint32_t label);

void tac_call(struct Call* call);
void tac_whilestmt(struct WhileStmt* w);
void tac_ifstmt(struct IfStmt* s);
void tac_retstmt(struct RetStmt* r);
void tac_assignstmt(struct AssignStmt* a);
void tac_forstmt(struct ForStmt* f);

void tac_namespace(struct Namespace* ns);
void tac_method(struct Method* m);
void tac_stmtblock(struct StmtBlock* block);
void tac_stmt(struct Stmt* stmt);

void tac_expr(struct Expr* expr);
void tac_unopterm(struct UnOpTerm* t);
void tac_term(struct Term* t);

void tac_variable(struct Variable* v);
void tac_simplevar(struct SimpleVar* sv);

#endif
