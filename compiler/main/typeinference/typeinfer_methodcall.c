#include <string.h>
#include "ast/ast.h"

#include "typeinference/typeinfer.h"

#include "tables/symtable/symtable.h"

struct Type* infer_type_methodcall(struct ST* st, struct Call* m) {

	struct Type* type = infer_type_variable(st, m->callable);

	//if any of the types of the arguments in the call bound a type variable,
	//then that could determine the return type

	//TODO: create a table with the type parameters
	//then recurse down the types of the arguments in the signature
	//and the types of the actual arguments to bind the type parameters

	//bool did_bind[10];
	//struct Type* bound_types[10];

	//memset(did_bind, 0, 10);

	/*
	for(uint32_t i = 0; i < type->basic_type->subr_type->count_arg_types; i++){
	    struct Type* arg_type = type->basic_type->subr_type->arg_types[i];

	    struct Type* actual_type = infer_type_expr(st, m->args[i]);

	    //TODO: recurse down the arg_type, actual_type in lockstep and bind type variables along the way
	}
	 */

	return type->basic_type->subr_type->return_type;
}
