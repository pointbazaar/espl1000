#include <stdio.h>

#include "tables/sst/sst.h"
#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

#include "tac/tacbuffer.h"
#include "tac/tac.h"
#include "x86_code_gen/compile_ir/compile_tac.h"

void compile_tac_label_x86(struct TAC* tac, struct IBuffer* ibu, struct Ctx* ctx) {

	char* s;

	if (tac->kind == TAC_LABEL_INDEXED) {
		char str[32];
		sprintf(str, "L%d", tac->label_index);
		s = (char*)&str;
	}

	if (tac->kind == TAC_LABEL_FUNCTION) {

		//in case of tests, sst may not be filled
		char* function_name = "main";

		if (tac->dest < sst_size(ctx_tables(ctx)->sst)) {

			function_name = sst_at(ctx_tables(ctx)->sst, tac->dest)->name;
		}

		s = function_name;
	}

	label(s);
}
