#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <util/fill_tables.h>

#include "ast/ast.h"

#include "util/ctx.h"
#include "flags.h"

#include "cg.h"
#include "_cg.h"

#include "c_types_util/gen_c_types.h"

#include "typechecker/typecheck.h"

#include "lambda/transpile_lambdas.h"

//Analyzer Includes
#include "analyzer/fn/fn_analyzer.h"
#include "analyzer/dead/dead_analyzer.h"
#include "analyzer/halts/halt_analyzer.h"
#include "analyzer/annotation/annotation_analyzer.h"

//Table Includes
#include "tables/sst/sst.h"
#include "tables/sst/sst_fill.h"
#include "tables/sst/sst_print.h"
#include "tables/stst/stst.h"
#include "tables/stst/stst_print.h"
#include "tables/symtable/symtable.h"

//counter for generating labels
unsigned int label_count = 0;

static void transpile_ast(struct AST* ast, struct Ctx* ctx, char* h_filename);

static void ns_transpile_struct_fwd_decls(struct Namespace* ns, struct Ctx* ctx);
static void ns_transpile_struct_decls(struct Namespace* ns, struct Ctx* ctx);
static void ns_transpile_subr_fwd_decls(struct Namespace* ns, struct Ctx* ctx);
static void ns_transpile_subrs(struct Namespace* ns, struct Ctx* ctx);

static void ns_transpile_passthrough_includes(struct Namespace* ns, struct Ctx* ctx);

bool transpile_and_write_c_code(char* c_filename, char* h_filename, struct AST* ast, struct Flags* flags, struct Ctx* ctx){

	ctx->c_file      = fopen(c_filename, "w");
	//full buffering for performance
	setvbuf(ctx->c_file, NULL, _IOFBF, BUFSIZ);
	
	if(flags->emit_headers){
		ctx->header_file = fopen(h_filename, "w");
		//full buffering for performance
		setvbuf(ctx->header_file, NULL, _IOFBF, BUFSIZ);
	}
	
	if(ctx->c_file == NULL || (ctx->header_file == NULL && flags->emit_headers)){
		
		printf("[smalldragon][Error] could not open output file: ");
		
		printf("%s\n", (ctx->c_file == NULL)?c_filename:h_filename);
		
		freeST(ctx->tables);
		free(ctx);
		
		return false;
	}

    transpile_ast(ast, ctx, h_filename);

	fclose(ctx->c_file);
	if(flags->emit_headers){ fclose(ctx->header_file); }
	
	freeST(ctx->tables);
	
	const bool status = !(ctx->error);
	
	free(ctx);
	
	return status;
}

static void transpile_ast(struct AST* ast, struct Ctx* ctx, char* h_filename){
	
	ctx->file = ctx->c_file; //direct output to c file
	
	{
		fprintf(ctx->c_file, "#include <stdbool.h>\n"); //absolutely needed
		fprintf(ctx->c_file, "#include <math.h>\n");  //absolutely needed
		fprintf(ctx->c_file, "#include <inttypes.h>\n"); //absolutely needed

		//used for try-catch
		fprintf(ctx->c_file, "#include <setjmp.h>\n"); //absolutely needed
	}
	
	if(ctx->flags->emit_headers){
		fprintf(ctx->c_file, "#include \"%s\"\n", h_filename);
	}

	ctx->flags->has_main_fn = sst_contains(ctx->tables->sst, "main");
	
	if(ctx->flags->debug){
		sst_print(ctx->tables->sst);
		stst_print(ctx->tables->stst);
	}
	
	if(ctx->flags->emit_headers){ 
		ctx->file = ctx->header_file; 
		
		//header guards
		char* sym_name = basename(ast->namespaces[0]->name);
		while(*sym_name == '.'){ sym_name++; }
		fprintf(ctx->file, "#ifndef %s_H\n", sym_name);
		fprintf(ctx->file, "#define %s_H\n\n", sym_name);
	}

	//TODO: make sure that the .h file also receives the correct
	//includes to even make the forward declarations (e.g. <stdbool.h>, ...
	for(int i=0; i < ast->count_namespaces; i++) {
		ns_transpile_passthrough_includes(ast->namespaces[i], ctx);
	}
	
	for(int i=0; i < ast->count_namespaces; i++) { 
		ns_transpile_struct_fwd_decls(ast->namespaces[i], ctx); 
	}
	
	for(int i=0; i < ast->count_namespaces; i++) { 

		ns_transpile_subr_fwd_decls(ast->namespaces[i], ctx);
	}
	
	if(ctx->flags->emit_headers){
		//header guards
		fprintf(ctx->file, "\n#endif\n");
	}
	
	for(int i=0; i < ast->count_namespaces; i++)
	{ ns_transpile_struct_decls(ast->namespaces[i], ctx); }
	
	ctx->file = ctx->c_file;


    fprintf(ctx->file, "#pragma GCC diagnostic push\n");
    fprintf(ctx->file, "#pragma GCC diagnostic ignored \"-Wint-conversion\" \n");

	for(int i=0; i < ast->count_namespaces; i++){
		ns_transpile_subrs(ast->namespaces[i], ctx);
	}

	fprintf(ctx->file, "#pragma GCC diagnostic pop\n");
}

static void ns_transpile_passthrough_includes(struct Namespace* ns, struct Ctx* ctx) {

	for (int i = 0; i < ns->count_includes; i++) {
		fprintf(ctx->file, "%s\n", ns->includes[i]);
	}
}

static void ns_transpile_struct_fwd_decls(struct Namespace* ns, struct Ctx* ctx){
	
	for(int i=0;i < ns->count_structs; i++){
		
		struct StructDecl* decl = ns->structs[i];
		
		char* name = decl->type->struct_type->type_name;
		
		fprintf(ctx->file, "struct %s;\n", name);
	}
}

static void ns_transpile_struct_decls(struct Namespace* ns, struct Ctx* ctx){
	
	for(int i=0;i < ns->count_structs; i++){
		transpileStructDecl(ns->structs[i], ctx);
	}
}

static void ns_transpile_subr_fwd_decls(struct Namespace* ns, struct Ctx* ctx){

	for(int i=0; i < ns->count_methods; i++){
		
		struct Method* m = ns->methods[i];
		
		if(sst_get(ctx->tables->sst, m->decl->name)->dead == DEAD_ISDEAD)
			{ continue; }
		
		transpileMethodSignature(m, ctx);
		fprintf(ctx->file, ";\n");
	}
}

static void ns_transpile_subrs(struct Namespace* ns, struct Ctx* ctx){

	for(int i=0; i < ns->count_methods; i++){
		
		struct Method* m = ns->methods[i];
		
		if(sst_get(ctx->tables->sst, m->decl->name)->dead == DEAD_ISDEAD)
			{ continue; }
		
		transpileMethod(m, ctx);
	}
}

