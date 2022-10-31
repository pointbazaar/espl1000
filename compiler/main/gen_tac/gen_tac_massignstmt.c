#include <stdio.h>
#include <stdlib.h>

#include "tac/tac.h"
#include "tac/tacbuffer.h"
#include "gen_tac.h"

static void case_const_addr(struct TACBuffer* buffer, struct MAssignStmt* m, struct Ctx* ctx);
static void case_variable_addr(struct TACBuffer* buffer, struct MAssignStmt* m, struct Ctx* ctx);

void tac_massignstmt(struct TACBuffer* buffer, struct MAssignStmt* m, struct Ctx* ctx){

    struct Expr* lhs = m->lhs->expr;
    
    if(lhs->term1->term->kind == 12){
		
		//const address on left hand side
        case_const_addr(buffer, m, ctx);
        
    }else{
		
		//non-const-addr on left hand side
        case_variable_addr(buffer, m, ctx);
	}
}

static void case_const_addr(struct TACBuffer* buffer, struct MAssignStmt* m, struct Ctx* ctx){
	
	struct Expr* lhs = m->lhs->expr;
	
	const uint32_t addr = lhs->term1->term->ptr.m12->ptr.m2_int_const;

    tac_expr(buffer, m->expr, ctx);
    
    struct TAC* tstore = makeTACStoreConstAddr(addr, tacbuffer_last_dest(buffer));

    tacbuffer_append(buffer, tstore);
}

static void case_variable_addr(struct TACBuffer* buffer, struct MAssignStmt* m, struct Ctx* ctx){
	
	//emit code to calculate the expression
	tac_expr(buffer, m->expr, ctx);
	uint32_t texpr = tacbuffer_last_dest(buffer);
	
	//emit code to calculate the address
	struct Expr* lhs = m->lhs->expr;
	tac_expr(buffer, lhs, ctx);
	uint32_t taddr = tacbuffer_last_dest(buffer);
	
	
	struct TAC* t = makeTACStore(taddr, texpr);
	tacbuffer_append(buffer, t);
}
