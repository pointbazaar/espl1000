#include <stdio.h>

#include "tac/tac.h"
#include "tac/tacbuffer.h"
#include "gen_tac.h"

void tac_const_data(struct TACBuffer* buffer, struct StringConst* str) {

	//struct DataTable* table = ctx_tables(ctx)->data;
	//const uint32_t offset = data_offset(str->value);
	//TODO: find the offset in data table
	const uint32_t offset = 0;

	tacbuffer_append(
	    buffer,
	    makeTACConstData(make_temp(), offset));
}
