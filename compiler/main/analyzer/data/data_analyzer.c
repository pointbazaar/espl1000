#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "ast/ast.h"

#include "tables/symtable/symtable.h"
#include "tables/data/data.h"

#include "data_analyzer.h"

#include "ast/visitor/visitor.h"

static void data_visitor(void* node, enum NODE_TYPE type, void* arg);

void analyze_data(struct ST* st, struct AST* ast) {

	visit_ast(ast, data_visitor, st->data);
}

static void data_visitor(void* node, enum NODE_TYPE type, void* arg) {

	struct DataTable* data = (struct DataTable*)arg;

	if (type != NODE_STRINGCONST) { return; }

	struct StringConst* sc = (struct StringConst*)node;

	data_insert(data, sc->value);
}
