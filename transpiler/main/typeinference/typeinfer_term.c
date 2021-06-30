#include <string.h>
#include <stdbool.h>

#include "ast/ast.h"
#include "ast/util/free_ast.h"
#include "ast/util/str_ast.h"

#include "typeinference/typeinfer.h"
#include "typeinference/util/type_str.h"

#include "tables/symtable/symtable.h"

static char* ERR_FATAL = "Fatal\n";

struct Type* infer_type_unopterm(char* filename, struct ST* st, struct UnOpTerm* t){
	
	return infer_type_term(filename, st, t->term);
}

struct Type* infer_type_term(char* filename, struct ST* st, struct Term* t){
	
	switch(t->kind){
		
		case  1: return typeFromStrPrimitive(st, "bool"); 
		case  2: return typeFromStrPrimitive(st, "int"); 
		case  3: return typeFromStrPrimitive(st, "char"); 
		case  4: return infer_type_methodcall(st, t->ptr.m4); 
		case  5: return infer_type_expr(filename, st, t->ptr.m5); 
		case  6: return infer_type_variable(filename, st, t->ptr.m6); 
		case  7: return typeFromStrPrimitive(st, "float"); 
		case  8: return typeFromStr(st, "String"); 
		case  9: return typeFromStrPrimitive(st, "int");
		case 10: return typeFromStrPrimitive(st, "int");
		
		default: print_exit(filename, &(t->super), ERR_FATAL);
	}
	
	return NULL;
}
