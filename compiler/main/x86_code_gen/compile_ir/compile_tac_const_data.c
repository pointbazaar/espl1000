#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "rat/rat.h"

#include "tables/symtable/symtable.h"
#include "tac/tac.h"
#include "x86_code_gen/compile_ir/compile_tac.h"

void compile_tac_const_data_x86(struct RAT* rat, struct TAC* tac, struct Ctx* ctx, struct IBuffer* ibu) {

	const int reg = rat_get_register(rat, tac_dest(tac));

	const uint64_t offset = tac_const_value(tac);

	struct DataTable* data_table = ctx_tables(ctx)->data;

	char* symbol = data_symbol(data_table, offset);

	assert(symbol != NULL);

	char* c;
	asprintf(&c, "TAC_CONST_DATA %s", symbol);

	mov_const_symbol(reg, symbol, c);

	free(c);
}
