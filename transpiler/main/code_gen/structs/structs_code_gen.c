#include <stdbool.h>
#include <stdio.h>

#include "util/ctx.h"
#include "flags/flags.h"

#include "typeinference/typeinfer.h"
#include "typeinference/util/type_str.h"
#include "tables/sst/sst.h"

#include "transpiler/main/analyzer/halts/halts.h"

#include "structs_code_gen.h"

// --- subroutines private to this compile unit ---
void gen_struct_subr(struct StructDecl* sd, struct Ctx* ctx);
void gen_struct_subr_signature(struct StructDecl* sd, struct Ctx* ctx);
// ----
void gen_struct_subr_new(struct StructDecl* sd, struct Ctx* ctx);
// ------------------------------------------------

void gen_struct_subrs(struct Namespace* ns, struct Ctx* ctx){
	
	for(int i = 0; i < ns->count_structs; i++){
		struct StructDecl* sd = ns->structs[i];
		gen_struct_subr(sd, ctx);
	}
}

void gen_struct_subr_signatures(struct Namespace* ns, struct Ctx* ctx){
	
	for(int i = 0; i < ns->count_structs; i++){
		struct StructDecl* sd = ns->structs[i];
		gen_struct_subr_signature(sd, ctx);
	}
}
// -------------------------------
void gen_struct_subr(struct StructDecl* sd, struct Ctx* ctx){
	
	//generates the various subroutines for one structure
	
	//these struct-specific subroutines
	//are shallow for now,
	//because structs can also describe
	//graphs which can contain cycles
	//which could make it complicated to 
	//allocate and free correctly
	
	gen_struct_subr_new(sd, ctx);
}

static void add_gen_struct_subrs_sst_single(struct Ctx* ctx, struct Namespace* ns, struct StructDecl* sd){
	
	struct SST* sst = ctx->tables->sst;
	
	struct Type* retTypeStruct = typeFromStr(ctx->tables, sd->type->struct_type->type_name);
	
	//add subroutines to sst
	char* nsname = ns->name;
	
	struct SSTLine* line = makeSSTLine("_", nsname, retTypeStruct, false, HALTS_ALWAYS, true, false);
	sprintf(line->name, "new%s", sd->type->struct_type->type_name);
	sst_add(sst, line);
}

void add_gen_struct_subrs_sst(struct Ctx* ctx, struct Namespace* ns){
	
	for(int i=0; i < ns->count_structs; i++)
		{ add_gen_struct_subrs_sst_single(ctx, ns, ns->structs[i]); }
}

void gen_struct_subr_signature(struct StructDecl* sd, struct Ctx* ctx){
	
	//e.g. struct A
	/* struct A* newA(); */
	char* name = sd->type->struct_type->type_name;
	
	fprintf(ctx->file, "struct %s* new%s();\n", name, name);
}

// ----------------------------------------------

void gen_struct_subr_new(struct StructDecl* sd, struct Ctx* ctx){
	//performs a shallow allocation
	
	char* name = sd->type->struct_type->type_name;
	
	fprintf(ctx->file, "struct %s* new%s(){\n", name, name);
	
	fprintf(ctx->file, "\tstruct %s* res = malloc(sizeof(struct %s));\n", name, name);
	
	fprintf(ctx->file, "\treturn res;\n");
	
	fprintf(ctx->file, "}\n");
	
}
