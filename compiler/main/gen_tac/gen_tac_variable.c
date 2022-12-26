#include <stdio.h>
#include <stdlib.h>

#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "tables/sst/sst.h"
#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

#include "helper_gen_tac_derefll.h"
#include "derefll/derefll.h"

#include "gen_tac.h"

//static void case_member(struct TACBuffer* buffer, struct Variable* v, struct Ctx* ctx);

void tac_variable(struct TACBuffer* buffer, struct Variable* v, struct Ctx* ctx){

	//TODO: remove temp fix
	//this is a temp fix because most of the codebase is just not ready
	//to handle calculations involving register pairs. Here we avoid the more
	//general code-path to get some of the tests running again.
	if(v->member_access == NULL){

		tac_simplevar(buffer, v->simple_var, ctx);
		return;
	}

	tac_variable_addr(buffer, v, ctx);

	uint32_t tlast = tacbuffer_last_dest(buffer);

	tacbuffer_append(buffer, makeTACLoad(make_temp(), tlast));
}

void tac_variable_addr(struct TACBuffer* buffer, struct Variable* v, struct Ctx* ctx){

	struct DerefLL* dll = derefll_ctor_variable(v);

	derefll_annotate_types(dll, ctx, NULL);

	//now iterate over the linked list and emit the correct TACs along the way

	struct DerefLL* current = dll;
	struct Type* prev_type = NULL;


	while(current == dll || current->next != NULL){

		tac_derefll_single(buffer, current, prev_type, ctx);

		prev_type = current->type;
		current = current->next;

		if(current == NULL) break;
	}

	derefll_dtor(dll);
}

/*
void tac_variable_addr(struct TACBuffer* buffer, struct Variable* v, struct Ctx* ctx){

	//produce the address of that variable as a temporary

	if(v->member_access == NULL){
		tac_simplevar_addr(buffer, v->simple_var, ctx);
		return;
	}

	//&(x1[...].x2[...]....)

	//t1 = &x1
	//tcurrent = t1

	//we have atleast 1 member

	//for member in members:
	//	for index in indices
	//		tindex = ...
	//		tcurrent += tindex
	//		t2 = [tcurrent]
	//		tcurrent = t2

	//	tcurrent += offset_in_struct(member)
	//
	//	if(member is last member){
	//		if(member has indices){}
	//	}
	//	t2 = [tcurrent]
	//	tcurrent = t2

	printf("tac_variable_addr currently unsupported");
	fflush(stdout);
	exit(1);
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
*/
