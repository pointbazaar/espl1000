#include <tables/sst/sst.h>
#include <tables/sst/sst_fill.h>
#include <tables/sst/sst_print.h>
#include <tables/stst/stst.h>
#include <tables/stst/stst_print.h>
#include "tables/symtable/symtable.h"
#include <tables/enum/enum_table.h>

#include "compiler/main/util/ctx.h"
#include "cli/flags/flags.h"

#include "fill_tables.h"

bool fill_tables(struct AST* ast, struct Ctx* ctx) {

	const bool debug = flags_debug(ctx_flags(ctx));

	if (debug) {
		printf("[debug] filling SST, STST tables...\n");
	}

	sst_clear(ctx_tables(ctx)->sst);

	for (int i = 0; i < ast->count_namespaces; i++) {

		struct Namespace* ns = ast->namespaces[i];

		sst_fill(ctx_tables(ctx), ctx_tables(ctx)->sst, ns);
		stst_fill(ctx_tables(ctx), ns);

		if (!enum_table_fill(ctx_tables(ctx)->enum_table, ns, debug)) {
			return false;
		}
	}

	if (flags_debug(ctx_flags(ctx))) {
		sst_print(ctx_tables(ctx)->sst);
		stst_print(ctx_tables(ctx)->stst);
	}

	return true;
}
