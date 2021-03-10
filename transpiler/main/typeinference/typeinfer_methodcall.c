#include "ast/ast.h"

#include "typeinference/typeinfer.h"

#include "tables/symtable/symtable.h"
#include "tables/sst/sst.h"

struct Type* infer_type_methodcall(struct ST* st, struct MethodCall* m){
	
	return sst_get(st->sst, m->methodName)->returnType;
}
