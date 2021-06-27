#include <stdbool.h>

#include "ast/ast.h"
#include "ast/util/free_ast.h"
#include "ast/util/str_ast.h"

#include "typeinference/typeinfer.h"

#include "tables/symtable/symtable.h"
#include "tables/lvst/lvst.h"

struct Type* infer_type_simplevar(char* filename, struct ST* st, struct SimpleVar* v){
	
	struct LVSTLine* line = lvst_get(st->lvst, v->name);
	
	return unwrap_indices(filename, line->type, v->count_indices);
}
