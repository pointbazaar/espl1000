#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "util/ctx.h"
#include "flags/flags.h"

#include "ast/util/free_ast.h"
#include "ast/util/str_ast.h"

#include "typeinference/typeinfer.h"
#include "typeinference/util/type_str.h"
#include "tables/sst/sst.h"

#include "transpiler/main/analyzer/halts/halts.h"

#include "code_gen/types/gen_c_types.h"
#include "structs_code_gen.h"

// --- subroutines private to this compile unit ---

void gen_struct_subr(struct StructDecl* sd, struct Ctx* ctx);
void gen_struct_subr_signature(struct StructDecl* sd, struct Ctx* ctx);
// ----
void gen_struct_subr_new(struct StructDecl* sd, struct Ctx* ctx);
void gen_struct_subr_del(struct StructDecl* sd, struct Ctx* ctx);
void gen_struct_subr_copy(struct StructDecl* sd, struct Ctx* ctx);
void gen_struct_subr_make(struct StructDecl* sd, struct Ctx* ctx);
void gen_struct_subr_print(struct StructDecl* sd, struct Ctx* ctx);
// ------------------------------------------------
//                   TOP LEVEL

	//TODO: Also update the subroutines symbol table
	//for type inference
	//gen_struct_subr_signatures(ns, ctx);
	
	//gen_struct_subrs(ns, ctx);

void gen_struct_subrs(struct Namespace* ns, struct Ctx* ctx){
	
	if(ctx->flags->debug){ 
		printf("gen_struct_subroutines(...)\n"); 
	}
	
	for(int i = 0; i < ns->count_structs; i++){
		struct StructDecl* sd = ns->structs[i];
		gen_struct_subr(sd, ctx);
	}
}

void gen_struct_subr_signatures(struct Namespace* ns, struct Ctx* ctx){
	
	if(ctx->flags->debug){ 
		printf("gen_struct_subroutines_signatures(...)\n"); 
	}
	
	for(int i = 0; i < ns->count_structs; i++){
		struct StructDecl* sd = ns->structs[i];
		gen_struct_subr_signature(sd, ctx);
	}
}
// -------------------------------
void gen_struct_subr(struct StructDecl* sd, struct Ctx* ctx){
	
	if(ctx->flags->debug){ 
		printf("gen_struct_subr(...)\n"); 
	}
	
	//generates the various subroutines 
	//for one structure, e.g. newX, delX, copyX
	
	//these struct-specific subroutines
	//are shallow for now,
	//because structs can also describe
	//graphs which can contain cycles
	//which could make it complicated to 
	//allocate and free correctly
	
	gen_struct_subr_new(sd, ctx);
	gen_struct_subr_del(sd, ctx);
	gen_struct_subr_copy(sd, ctx);
	gen_struct_subr_make(sd, ctx);
	gen_struct_subr_print(sd, ctx);
}

static void add_gen_struct_subrs_sst_single(struct Ctx* ctx, struct Namespace* ns, struct StructDecl* sd){
	
	struct SST* sst = ctx->tables->sst;
	
	struct Type* retTypeStruct = typeFromStr(ctx->tables, sd->type->structType->typeName);
	
	//add subroutines to sst
	char* nsname = ns->name;
	
	struct SSTLine* line = makeSSTLine("_", nsname, retTypeStruct, false, HALTS_ALWAYS, true, false);
	sprintf(line->name, "new%s", sd->type->structType->typeName);
	sst_add(sst, line);
	
	line = makeSSTLine("_", nsname, retTypeStruct, false, HALTS_ALWAYS, true, false);
	sprintf(line->name, "copy%s", sd->type->structType->typeName);
	sst_add(sst, line);
	
	line = makeSSTLine("_", nsname, retTypeStruct, false, HALTS_ALWAYS, true, false);
	sprintf(line->name, "make%s", sd->type->structType->typeName);
	sst_add(sst, line);
	
	line = makeSSTLine("_", nsname, typeFromStrPrimitive(ctx->tables, "int"), false, HALTS_ALWAYS, true, false);
	sprintf(line->name, "print%s", sd->type->structType->typeName);
	sst_add(sst, line);
	
	line = makeSSTLine("_", nsname, typeFromStrPrimitive(ctx->tables, "int"), false, HALTS_ALWAYS, true, false);
	sprintf(line->name, "free%s", sd->type->structType->typeName);
	sst_add(sst, line);
	
	line = makeSSTLine("_", nsname, typeFromStrPrimitive(ctx->tables, "int"), false, HALTS_ALWAYS, true, false);
	sprintf(line->name, "del%s", sd->type->structType->typeName);
	sst_add(sst, line);
}

void add_gen_struct_subrs_sst(struct Ctx* ctx, struct Namespace* ns){
	
	if(ctx->flags->debug)
		{ printf("add_gen_struct_subrs_sst\n"); }
	
	for(int i=0; i < ns->count_structs; i++)
		{ add_gen_struct_subrs_sst_single(ctx, ns, ns->structs[i]); }
}

void gen_struct_subr_signature(struct StructDecl* sd, struct Ctx* ctx){
	
	//e.g. struct A
	//newA
	//delA
	//copyA
	/*
	 struct A* newA();
	 void delA();
	 struct A* copyA();
	 */
	char* name = sd->type->structType->typeName;
	
	fprintf(ctx->file, "struct %s* new%s();\n", name, name);
	fprintf(ctx->file, "int del%s(struct %s* ptr);\n", name, name);
	fprintf(ctx->file, "struct %s* copy%s(struct %s* ptr);\n", name, name, name);
	
	//constructor with all members of the struct
	fprintf(ctx->file, "struct %s* make%s(", name, name);
	for(int i=0;i < sd->count_members;i++){
		struct StructMember* member = sd->members[i];
		
		char* typeName = type2CType(member->type, ctx);
		fprintf(ctx->file, "%s %s", typeName, member->name);
		free(typeName);
		
		if(i < sd->count_members-1){
			fprintf(ctx->file, ", ");
		}
	}
	fprintf(ctx->file, ");\n");
	
	fprintf(ctx->file, "int print%s(struct %s* ptr);\n", name, name);
}

// ----------------------------------------------

void gen_struct_subr_new(struct StructDecl* sd, struct Ctx* ctx){
	//performs a shallow allocation
	
	if(ctx->flags->debug){ 
		printf("gen_struct_subr_new(...)\n"); 
	}
	
	char* name = sd->type->structType->typeName;
	
	fprintf(ctx->file, "struct %s* new%s(){\n", name, name);
	
	fprintf(ctx->file, "\tstruct %s* res = malloc(sizeof(struct %s));\n", name, name);
	
	fprintf(ctx->file, "\treturn res;\n");
	
	fprintf(ctx->file, "}\n");
	
}

void gen_struct_subr_del(struct StructDecl* sd, struct Ctx* ctx){
	//performs a shallow free
	
	if(ctx->flags->debug){ 
		printf("gen_struct_subr_del(...)\n"); 
	}
	
	char* name = sd->type->structType->typeName;
	
	fprintf(ctx->file, "int del%s(struct %s* instance){\n", name, name);
	
	fprintf(ctx->file, "\tfree(instance);\n");
	fprintf(ctx->file, "\treturn 0;\n");
	
	fprintf(ctx->file, "}\n");
	
}

void gen_struct_subr_copy(struct StructDecl* sd, struct Ctx* ctx){
	//performs a shallow copy
	
	if(ctx->flags->debug){ 
		printf("gen_struct_subr_copy(...)\n"); 
	}
	
	char* name = sd->type->structType->typeName;
	
	fprintf(ctx->file, "struct %s* copy%s(struct %s* instance){\n", name, name, name);
	
	fprintf(ctx->file, "\tstruct %s* res = malloc(sizeof(struct %s));\n", name, name);
	
	//copy all the members
	for(int i = 0; i < sd->count_members; i++){
		struct StructMember* sm = sd->members[i];
		
		fprintf(
			ctx->file, 
			"\tres->%s = instance->%s;\n",
			sm->name,
			sm->name
		);
	}
	
	fprintf(ctx->file, "\treturn res;\n");
	
	fprintf(ctx->file, "}\n");
}

void gen_struct_subr_make(struct StructDecl* sd, struct Ctx* ctx){
	
	if(ctx->flags->debug){ 
		printf("gen_struct_subr_make(...)\n"); 
	}
	
	char* name = sd->type->structType->typeName;
	
	//constructor with all members of the struct
	fprintf(ctx->file, "struct %s* make%s(", name, name);
	for(int i=0;i < sd->count_members;i++){
		struct StructMember* member = sd->members[i];
		
		char* typeName = type2CType(member->type, ctx);
		fprintf(ctx->file, "%s %s", typeName, member->name);
		free(typeName);
		
		if(i < sd->count_members-1){
			fprintf(ctx->file, ", ");
		}
	}
	fprintf(ctx->file, ") {\n");
	
	//malloc
	fprintf(ctx->file, "\tstruct %s* res = malloc(sizeof(struct %s));\n", name, name);

	//copy all the members
	for(int i = 0; i < sd->count_members; i++){
		struct StructMember* sm = sd->members[i];
		
		fprintf(
			ctx->file, 
			"\tres->%s = %s;\n",
			sm->name,
			sm->name
		);
	}
	
	fprintf(ctx->file, "\treturn res;\n");
	
	fprintf(ctx->file, "}\n");
}

void gen_struct_subr_print(struct StructDecl* sd, struct Ctx* ctx){
	
	if(ctx->flags->debug){ 
		printf("gen_struct_subr_print(...)\n"); 
	}
	
	char* name = sd->type->structType->typeName;
	
	//subroutine to print the struct contents
	fprintf(ctx->file, "int print%s(struct %s* ptr", name, name);
	
	fprintf(ctx->file, ") {\n");
	
	fprintf(ctx->file, "\tprintf(\"{\");\n");
	
	//copy all the members
	for(int i = 0; i < sd->count_members; i++){
		struct StructMember* sm = sd->members[i];
		
		char buf[DEFAULT_STR_SIZE+5];
		sprintf(buf, "ptr->%s", sm->name);
		
		char* format = "%s";
		
		//TODO: decide the format based on type
		char* typeName = strType(sm->type);
		
		format = typeNameToCFormatStr(typeName);
		
		fprintf(
			ctx->file, 
			"\tprintf(\"%%s=%s,\", \"%s\", %s);\n",
			format,
			sm->name,
			buf
		);
	}
	fprintf(ctx->file, "\tprintf(\"}\");\n");
	fprintf(ctx->file, "\treturn 0;\n");
	
	fprintf(ctx->file, "}\n");
}
