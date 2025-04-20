#include <stdio.h>

#include "stst.h"

static void stst_print_for_struct(struct STSTLine* line);
static char* fmt = "%16s|%16s|\n";

void stst_print(struct STST* stst) {

	printf("[STST] Struct Symbol Table (%d elements)\n", stst_size(stst));
	printf(fmt, "struct name", "member name");

	//--------
	//----------------

	printf("----------------|----------------|\n");

	for (size_t i = 0; i < stst_size(stst); i++) {

		struct STSTLine* line = stst_at(stst, i);

		stst_print_for_struct(line);
	}
}

static void stst_print_for_struct(struct STSTLine* line) {

	for (int j = 0; j < line->decl->count_members; j++) {

		struct StructMember* member = line->decl->members[j];

		printf(fmt, line->name, member->name);
	}
}
