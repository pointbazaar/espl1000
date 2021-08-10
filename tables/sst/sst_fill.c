#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include "sst_fill.h"
#include "../symtable/symtable.h"
#include "ast/util/copy_ast.h"

static void sst_fill_externc(struct SST* sst, struct Namespace* ns);

void sst_fill(struct ST* st, struct SST* sst, struct Namespace* ns){

	sst_fill_externc(sst, ns);
	
	for(int i = 0; i < ns->count_methods; i++){
		
		struct Method* m = ns->methods[i];
		
		struct Type* t = method_to_type(m);
		
		st_register_inferred_type(st, t);
		
		struct SSTLine* line = makeSSTLine2(m, t, ns->name);
		
		sst_add(sst, line);
	}
}

struct Type* method_to_type(struct Method* m){
	
	struct Type* t = method_decl_to_type(m->decl);

	t->super.line_num = m->super.line_num;
	t->super.annotations = m->super.annotations;
	
	return t;
}

struct SubrType* method_decl_to_subrtype(struct MethodDecl* mdecl){

	struct SubrType* stype = make(SubrType);

	stype->super.annotations = mdecl->super.annotations;
	stype->super.line_num    = mdecl->super.line_num;

	stype->return_type     = copy_type(mdecl->return_type);
	stype->has_side_effects = mdecl->has_side_effects;
	stype->count_arg_types = mdecl->count_args;
	stype->throws         = mdecl->throws;

	stype->arg_types = malloc(sizeof(void*) * stype->count_arg_types);

	for(uint32_t i = 0; i < stype->count_arg_types; i++){
		stype->arg_types[i] = copy_type(mdecl->args[i]->type);
	}

	return stype;
}
struct Type* method_decl_to_type(struct MethodDecl* mdecl){

	struct Type* t         = make(Type);
	struct BasicType* bt   = make(BasicType);

	t->super.line_num = mdecl->super.line_num;
	t->super.annotations = mdecl->super.annotations;

	bt->simple_type = NULL;
	bt->subr_type   = method_decl_to_subrtype(mdecl);
	t->basic_type = bt;
	t->type_param = NULL;
	t->array_type = NULL;

	return t;
}
static void sst_fill_externc(struct SST* sst, struct Namespace* ns){

	for(int i = 0; i < ns->count_externc; i++) {

		struct ExternC* ec = ns->externc[i];

		if (ec->subr_decl == NULL){ continue; }

		struct SSTLine* line = makeSSTLine(
				ec->subr_decl->name,
				"_EXTERN_C_",
				ec->subr_decl->return_type,
				true,
				HALTS_UNKNOWN,
				ec->subr_decl->has_side_effects,
				ec->subr_decl->throws
		);

		line->name_in_c = ec->name_in_c;
		line->is_extern_c = true;

		line->type = method_decl_to_type(ec->subr_decl); //correct?

		sst_add(sst, line);
	}
}
