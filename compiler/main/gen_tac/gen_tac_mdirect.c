
#include <stdio.h>
#include <stdlib.h>
#include "gen_tac.h"
#include "tac/tacbuffer.h"

static void case_const_addr(struct TACBuffer* buffer, struct Expr* expr);
static void case_variable_addr(struct TACBuffer* buffer, struct Expr* expr, struct Ctx* ctx);

void tac_mdirect(struct TACBuffer* buffer, struct MDirect* m, struct Ctx* ctx){

    struct Expr* expr = m->expr;
    
    if(expr->term1->term->kind == 12){
		case_const_addr(buffer, expr);
	}else{
		case_variable_addr(buffer, expr, ctx);
	}
}

static void case_const_addr(struct TACBuffer* buffer, struct Expr* expr){
	
	const uint32_t addr = expr->term1->term->ptr.m12->ptr.m2_int_const;

    struct TAC* t = makeTACLoadConstAddr(make_temp(), addr);

    tacbuffer_append(buffer, t);
}

static void case_variable_addr(struct TACBuffer* buffer, struct Expr* expr, struct Ctx* ctx){
	
	//t1 = [expr]
	
	tac_expr(buffer, expr, ctx);
	
	struct TAC* t = makeTACLoad(make_temp(), tacbuffer_last_dest(buffer));
	
	tacbuffer_append(buffer, t);
}
