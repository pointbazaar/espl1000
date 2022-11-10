#include <stdio.h>
#include <stdlib.h>

#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "gen_tac.h"

static void case_no_member(struct TACBuffer* buffer, struct Variable* v, struct Ctx* ctx);
static void case_member(struct TACBuffer* buffer, struct Variable* v, struct Ctx* ctx);
//static void case_member_addr(struct TACBuffer* buffer, struct Variable* v, struct Ctx* ctx);

void tac_variable(struct TACBuffer* buffer, struct Variable* v, struct Ctx* ctx){

    if(v->member_access != NULL){
		
		if(v->simple_var->count_indices != 0){
			printf("member access with indices currently not implemented for avr_code_gen\n");
			exit(1);
		}
		
		case_member(buffer, v, ctx);
        
    }else{
		case_no_member(buffer, v, ctx);
	}
}

void tac_variable_addr(struct TACBuffer* buffer, struct Variable* v, struct Ctx* ctx){
	
	//produce the address of that variable as a temporary
	if(v->member_access == NULL){
		tac_simplevar_addr(buffer, v->simple_var, ctx);
	}else{
		printf("tac_variable_addr currently unsupported");
		fflush(stdout);
		exit(1);
		//case_member_addr(buffer, v, ctx);
	}
}

static void case_no_member(struct TACBuffer* buffer, struct Variable* v, struct Ctx* ctx){
	
	tac_simplevar(buffer, v->simple_var, ctx);
}

static void case_member(struct TACBuffer* buffer, struct Variable* v, struct Ctx* ctx){
	
	//t1 = simplevar
	// //tcurrent = t1
	
	//for member in members		
		//tcurrent += offset in struct (member)
		
		//t2 = [tcurrent]
		// //tcurrent = t2
		
	struct STST* stst = ctx_tables(ctx)->stst;
	
	struct LVSTLine* line = lvst_get(ctx_tables(ctx)->lvst, v->simple_var->name);
	
	struct Type* structType = line->type;
	struct Type* memberType = NULL;
	
	struct Variable* current = v;
	
	tac_simplevar(buffer, v->simple_var, ctx);
	
	uint32_t tcurrent = tacbuffer_last_dest(buffer);
	
	while(current != NULL && current->member_access != NULL){
		
		struct Variable* member = current->member_access;
	
		char* struct_name = structType->basic_type->simple_type->struct_type->type_name;
		
		char* member_name = member->simple_var->name;
		
		struct StructMember* sm = stst_get_member(stst, struct_name, member_name);
		memberType = sm->type;
		
		uint32_t offset = stst_member_offset(stst, struct_name, member_name);
		
		// --- codegen ---
		
		if(offset != 0){
			tacbuffer_append(buffer, makeTACBinOpImmediate(tcurrent, TAC_OP_ADD, offset));
		}
		
		tacbuffer_append(buffer, makeTACLoad(make_temp(), tcurrent));
		
		
		
		structType = memberType;
		tcurrent = tacbuffer_last_dest(buffer);
		current = current->member_access;
	}
}

