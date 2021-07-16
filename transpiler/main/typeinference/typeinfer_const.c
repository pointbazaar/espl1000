#include <string.h>

#include "ast/ast.h"

#include "typeinference/typeinfer.h"
#include "typeinference/util/type_str.h"

#include "tables/symtable/symtable.h"

static char* ERR_FATAL = "Fatal\n";

struct Type* infer_type_constvalue(char* filename, struct ST* st, struct ConstValue* cv){

	switch(cv->kind){

		case 1: return typeFromStrPrimitive(st, "bool");
		case 2: return typeFromStrPrimitive(st, "int");
		case 3: return typeFromStrPrimitive(st, "char");
		case 4: return typeFromStrPrimitive(st, "float");
		case 5: return typeFromStrPrimitive(st, "int"); //hex
		case 6: return typeFromStrPrimitive(st, "int"); //bin
	}

	print_exit(filename, &(cv->super), ERR_FATAL);
	return NULL;
}