#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast/ast.h"
#include "tables/symtable/symtable.h"
#include "typeinference/typeinfer.h"
#include "typeinference/util/type_str.h"

struct Type* infer_type_constvalue(struct ST* st, struct ConstValue* cv) {

	switch(cv->kind) {

		case 1: return typeFromStrPrimitive(st, "bool");
		case 2: return typeFromStrPrimitive(st, "int");
		case 3: return typeFromStrPrimitive(st, "char");
		case 5: return typeFromStrPrimitive(st, "int"); // hex
		case 6:
			return typeFromStrPrimitive(st, "int"); // bin

		// fatal error case
		default: return typeFromStrPrimitive(st, "int");
	}
}
