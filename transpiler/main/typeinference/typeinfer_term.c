#include <string.h>

#include "ast/ast.h"

#include "typeinference/typeinfer.h"
#include "typeinference/util/type_str.h"

#include "tables/symtable/symtable.h"

static char* ERR_FATAL = "Fatal\n";

struct Type* infer_type_unopterm(char* filename, struct ST* st, struct UnOpTerm* t){
	
	return infer_type_term(filename, st, t->term);
}

struct Type* infer_type_term(char* filename, struct ST* st, struct Term* t){
	
	switch(t->kind){

		case  4: return infer_type_methodcall(st, t->ptr.m4); 
		case  5: return infer_type_expr(filename, st, t->ptr.m5); 
		case  6: return infer_type_variable(filename, st, t->ptr.m6);
		case  8: return typeFromStrArray(st, "char");
		//do not use lambda here
		case 12: return infer_type_constvalue(filename, st, t->ptr.m12);
		
		default: print_exit(filename, &(t->super), ERR_FATAL);
	}
	
	return NULL;
}
