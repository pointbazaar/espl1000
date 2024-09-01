#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "ast/ast.h"

#include "tables/symtable/symtable.h"
#include "tables/lvst/lvst.h"

#include "typeinference/util/type_str.h"
#include "typeinference/typeinfer.h"

#include "lv_analyzer.h"

#include "ast/visitor/visitor.h"

static void lv_for_stmt(struct ST* st, struct ForStmt* l);
static void lv_assign_stmt(struct ST* st, struct AssignStmt* a);
static void lv_declarg(struct ST* st, struct DeclArg* da);

static void lv_visitor(void* node, enum NODE_TYPE type, void* arg);

void lvst_fill(struct Method* subr, struct ST* st) {

	visit_method(subr, lv_visitor, st);
}

static void lv_visitor(void* node, enum NODE_TYPE type, void* arg) {

	struct ST* myst = (struct ST*)arg;

	if (type == NODE_DECLARG) { lv_declarg(myst, (struct DeclArg*)node); }

	if (type == NODE_ASSIGNSTMT) { lv_assign_stmt(myst, (struct AssignStmt*)node); }

	if (type == NODE_FORSTMT) { lv_for_stmt(myst, (struct ForStmt*)node); }
}

static void lv_declarg(struct ST* st, struct DeclArg* da) {

	char* name = da->name;
	struct Type* type = da->type;

	struct LVSTLine* line = make(LVSTLine);

	asprintf(&(line->name), "%s", name);
	line->type = type;
	line->is_arg = true;
	line->first_occur = NULL;
	line->read_only = false;

	lvst_add(st->lvst, line);
}

static void lv_for_stmt(struct ST* st, struct ForStmt* l) {

	struct LVSTLine* line = make(LVSTLine);

	asprintf(&(line->name), "%s", l->index_name);

	line->type = typeFromStrPrimitive(st, "int");

	line->first_occur = NULL;
	line->is_arg = false;
	line->read_only = true;

	lvst_add(st->lvst, line);
}

static void lv_assign_stmt(struct ST* st, struct AssignStmt* a) {

	char* varName = a->var->simple_var->name;

	if (lvst_contains(st->lvst, varName)) { return; }

	struct LVSTLine* line = make(LVSTLine);

	line->first_occur = a;
	line->is_arg = false;
	line->read_only = false;

	asprintf(&(line->name), "%s", varName);

	if (a->opt_type != NULL) {

		line->type = a->opt_type;

	} else {

		line->type = infer_type_expr(st, a->expr);
	}

	lvst_add(st->lvst, line);
}
