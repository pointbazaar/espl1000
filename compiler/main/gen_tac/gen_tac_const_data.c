#include <stdio.h>

#include "tables/symtable/symtable.h"
#include "tac/tac.h"
#include "tac/tacbuffer.h"
#include "gen_tac.h"

#include "tables/data/data.h"

bool tac_const_data(struct TACBuffer* buffer, struct StringConst* str, struct Ctx* ctx) {

	const int32_t offset = data_string_offset(ctx_tables(ctx)->data, str->value);

	if (offset < 0) {
		fprintf(stderr, "%s:%s: could not find offset of '%s' in data table\n", __FILE__, __func__, str->value);
		return false;
	}

	tacbuffer_append(
	    buffer,
	    makeTACConstData(str->super.line_num, make_temp(), offset));

	return true;
}
