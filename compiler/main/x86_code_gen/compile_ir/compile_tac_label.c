#include <stdio.h>
#include <stdlib.h>

#include "tables/sst/sst.h"
#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

#include "tac/tacbuffer.h"
#include "tac/tac.h"
#include "x86_code_gen/compile_ir/compile_tac.h"

void compile_tac_label_x86(struct TAC* tac, struct IBuffer* ibu, struct Ctx* ctx) {

	char* s;

	if (tac_kind(tac) == TAC_LABEL_INDEXED) {
		char str[32];
		sprintf(str, "L%d", tac_label_index(tac));
		s = (char*)&str;
	}

	if (tac_kind(tac) == TAC_LABEL_FUNCTION) {

		char* function_name;

		if (tac_dest(tac) < sst_size(ctx_tables(ctx)->sst)) {

			function_name = sst_at(ctx_tables(ctx)->sst, tac_dest(tac))->name;
		} else {
			fprintf(stderr, "index %d out of bounds in sst\n", tac_dest(tac));
			exit(1);
		}

		s = function_name;
	}

	label(s);
}
