#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "ast/ast.h"

#include "typeinference/typeinfer.h"
#include "typeinference/util/type_str.h"

#include "tables/symtable/symtable.h"

struct Type* infer_type_constvalue_int(struct ST* st, int64_t value) {
	bool is_signed = value < 0;
	uint64_t abs = (is_signed) ? -value : value;
	char* str = "int8";
	if (abs > 0xff)
		str = "int16";
	if (abs > 0xffff)
		str = "int32";

	char res[10];
	sprintf(res, "%s%s", (is_signed) ? "" : "u", str);

	struct Type* res1 = typeFromStrPrimitive(st, res);
	return res1;
}

struct Type* infer_type_constvalue(struct ST* st, struct ConstValue* cv) {

	switch (cv->kind) {

		case 1: return typeFromStrPrimitive(st, "bool");
		case 2: return infer_type_constvalue_int(st, cv->ptr.m2_int_const);
		case 3: return typeFromStrPrimitive(st, "char");
		case 5: return infer_type_constvalue_int(st, cv->ptr.m5_hex_const);
		default: return NULL;
	}
}
