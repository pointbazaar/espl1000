#include <string.h>
#include <stdbool.h>

#include "ast/ast.h"
#include "ast/util/free_ast.h"
#include "ast/util/str_ast.h"

#include "code_gen/gen_c_types.h"
#include "typeinference/typeinfer.h"
#include "typeinference/util/type_str.h"

#include "tables/symtable.h"

static char* ERR_FATAL = "Fatal\n";

struct Type* infer_type_unopterm(struct ST* st, struct UnOpTerm* t){
	
	return infer_type_term(st, t->term);
}

struct Type* infer_type_term(struct ST* st, struct Term* t){
	
	switch(t->kind){
		
		case  1: return typeFromStr(st, "bool", true, false); 
		case  2: return typeFromStr(st, "int", true, true); 
		case  3: return typeFromStr(st, "char", true, false); 
		case  4: return infer_type_methodcall(st, t->ptr.m4); 
		case  5: return infer_type_expr(st, t->ptr.m5); 
		case  6: return infer_type_variable(st, t->ptr.m6); 
		case  7: return typeFromStr(st, "float", true, false); 
		case  8: return typeFromStr(st, "String", false, false); 
		case  9: return typeFromStr(st, "int", true, true);
		case 10: return typeFromStr(st, "int", true, true);
		
		default: print_exit(ERR_FATAL);
	}
	
	return NULL;
}
