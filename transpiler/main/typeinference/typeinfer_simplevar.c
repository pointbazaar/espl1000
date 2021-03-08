#include <stdbool.h>

#include "ast/ast.h"
#include "ast/util/free_ast.h"
#include "ast/util/str_ast.h"

#include "code_gen/gen_c_types.h"
#include "typeinference/typeinfer.h"

#include "tables/symtable.h"
#include "tables/lvst.h"

struct Type* infer_type_simplevar(struct ST* st, struct SimpleVar* v){
	
	struct LVSTLine* line = lvst_get(st->lvst, v->name);
	
	return unwrap_indices(line->type, v->count_indices);
}
