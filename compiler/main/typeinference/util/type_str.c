#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "tables/symtable/symtable.h"
#include "tables/sst/sst.h"

#include "ast/ast.h"

#include "typeinference/util/type_str.h"

static struct Type* typeFromStrPrimitive_inner(char* typeName);

enum INTTYPE get_inttype(char* typeName) {

	if (strcmp(typeName, "int") == 0) return INT;
	if (strcmp(typeName, "uint") == 0) return UINT;

	if (strcmp(typeName, "int8") == 0) return INT8;
	if (strcmp(typeName, "uint8") == 0) return UINT8;

	if (strcmp(typeName, "int16") == 0) return INT16;
	if (strcmp(typeName, "uint16") == 0) return UINT16;

	if (strcmp(typeName, "int32") == 0) return INT32;
	if (strcmp(typeName, "uint32") == 0) return UINT32;

	if (strcmp(typeName, "int64") == 0) return INT64;
	if (strcmp(typeName, "uint64") == 0) return UINT64;

	return NONE;
}

static struct Type* typeFromStrPrimitive_inner(char* typeName) {

	struct Type* res = make(Type);
	struct BasicType* btw = make(BasicType);
	struct SimpleType* s = make(SimpleType);
	struct PrimitiveType* p = make(PrimitiveType);

	res->basic_type = btw;
	res->type_param = NULL;
	res->array_type = NULL;

	p->is_int_type = false;

	p->is_int_type |= strcmp(typeName, "int") == 0;

	p->is_int_type |= strcmp(typeName, "int8") == 0;
	p->is_int_type |= strcmp(typeName, "uint8") == 0;
	p->is_int_type |= strcmp(typeName, "int16") == 0;
	p->is_int_type |= strcmp(typeName, "uint16") == 0;
	p->is_int_type |= strcmp(typeName, "int32") == 0;
	p->is_int_type |= strcmp(typeName, "uint32") == 0;
	p->is_int_type |= strcmp(typeName, "int64") == 0;
	p->is_int_type |= strcmp(typeName, "uint64") == 0;

	p->is_char_type = strcmp(typeName, "char") == 0;
	p->is_bool_type = strcmp(typeName, "bool") == 0;

	p->int_type = get_inttype(typeName);

	if (p->is_int_type && p->int_type == NONE)
		return NULL;

	s->primitive_type = p;
	s->struct_type = NULL;

	btw->subr_type = NULL;
	btw->simple_type = s;

	return res;
}

struct Type* typeFromStrPrimitive(struct ST* st, char* typeName) {

	struct Type* res = typeFromStrPrimitive_inner(typeName);
	if (res == NULL) return res;

	st_register_inferred_type(st, res);

	return res;
}

struct Type* typeFromStrArray(struct ST* st, char* typeName) {

	struct ArrayType* at = make(ArrayType);
	at->element_type = typeFromStrPrimitive_inner(typeName);

	struct Type* t = make(Type);

	t->basic_type = NULL;
	t->type_param = NULL;
	t->array_type = at;

	st_register_inferred_type(st, t);

	return t;
}
