#include <stdio.h>
#include <stdlib.h>

#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "gen_tac.h"

static void case_no_member(struct TACBuffer* buffer, struct Variable* v, struct Ctx* ctx);
static void case_member(struct TACBuffer* buffer, struct Variable* v, struct Ctx* ctx);

void tac_variable(struct TACBuffer* buffer, struct Variable* v, struct Ctx* ctx){

    if(v->member_access != NULL){
		
		if(v->simple_var->count_indices != 0){
			printf("member access with indices currently not implemented for avr_code_gen\n");
			exit(1);
		}
		
		if(v->member_access->member_access != NULL){
			printf("multiple member access currently not implemented for avr_code_gen\n");
			exit(1);
		}
		
		case_member(buffer, v, ctx);
        
    }else{
		case_no_member(buffer, v, ctx);
	}
}

static void case_no_member(struct TACBuffer* buffer, struct Variable* v, struct Ctx* ctx){
	
	tac_simplevar(buffer, v->simple_var, ctx);
}

static void case_member(struct TACBuffer* buffer, struct Variable* v, struct Ctx* ctx){
	
	//t1 = address of simplevar
		
	//t1 += offset in struct (member)
	
	//t2 = [t1]
	
	struct LVSTLine* line = lvst_get(ctx_tables(ctx)->lvst, v->simple_var->name);
	
	struct Type* structType = line->type;
	
	struct Variable* member = v->member_access;
	
	char* struct_name = structType->basic_type->simple_type->struct_type->type_name;
	char* member_name = member->simple_var->name;
	
	uint32_t offset = stst_member_offset(ctx_tables(ctx)->stst, struct_name, member_name);

	// --- codegen ---
	//const uint32_t local_index = lvst_index_of(ctx_tables(ctx)->lvst, v->simple_var->name);
	
	tac_simplevar(buffer, v->simple_var, ctx);
	
	uint32_t t1 = tacbuffer_last_dest(buffer);
	
	tacbuffer_append(buffer, makeTACBinOpImmediate(t1, TAC_OP_ADD, offset));
	
	tacbuffer_append(buffer, makeTACLoad(make_temp(), t1));
}
