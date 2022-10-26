#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "gen_tac.h"

void tac_stmt(struct TACBuffer* buffer, struct Stmt* stmt, struct Ctx* ctx){

    if(stmt->is_break){
        tacbuffer_append(buffer, makeTACGoto(ctx_get_label_loop_end(ctx)));
        return;
    }
    if(stmt->is_continue){
        tacbuffer_append(buffer, makeTACGoto(ctx_get_label_loop_start(ctx)));
        return;
    }

    switch(stmt->kind){
        case 1: tac_call(buffer, stmt->ptr.m1); break;
        case 2: tac_whilestmt(buffer, stmt->ptr.m2, ctx); break;
        case 3: tac_ifstmt(buffer, stmt->ptr.m3, ctx); break;
        case 4: tac_retstmt(buffer, stmt->ptr.m4); break;
        case 5: tac_assignstmt(buffer, stmt->ptr.m5); break;
        case 7: tac_forstmt(buffer, stmt->ptr.m7, ctx); break;
        case 8: tac_switchstmt(buffer, stmt->ptr.m8, ctx); break;
        case 9: tac_massignstmt(buffer, stmt->ptr.m9); break;
    }
}
