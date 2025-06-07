#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "tables/symtable/symtable.h"

#include "ast/ast.h"
#include "rat/rat.h"

#include "cli/flags/flags.h"

#include "util/ctx.h"

#include "ibuffer/ibuffer_avr.h"

#include "cg_avr.h"
#include "tac/tacbuffer.h"
#include "tac/tac.h"
#include "cg_avr_single_function.h"
#include "cg_avr_single_tac.h"

static void emit_defs(struct IBuffer* ibu) {
	//in /usr/share/avra
	//in this file a comment must be shortened, otherwise avra will give an error
	//.INCLUDE "/usr/share/avra/m32def.inc"
	//fprintf(fout, ".DEVICE ATmega328P\n");

	//we do not want to depend on the specific location of that file
	//or if it's even there ... just append some stuff here

	avra_equ("RAMEND = 0x085f");
	avra_def("XH = r27");
	avra_def("XL = r26");
	avra_def("YH = r29");
	avra_def("YL = r28");
	avra_def("ZH = r31");
	avra_def("ZL = r30");
	avra_equ("SPH = 0x3e");
	avra_equ("SPL = 0x3d");
}

void emit_call_main_endloop(struct IBuffer* ibu) {
	//create the endloop such that the mcu does
	//not start over again after main

	//'endloop'
	avr_call("main", "");
	avr_label("endloop");
	avr_rjmp("endloop", "");
}

static bool avr_prologue(struct Ctx* ctx, struct IBuffer* ibu) {
	struct RAT* rat = rat_ctor(RAT_ARCH_AVR, 10);

	struct TAC* t = makeTACSetupSP(0);

	int status = emit_asm_avr_single_tac(rat, t, ctx, ibu);

	if (!status) {
		free(t);
		rat_dtor(rat);
		return false;
	}

	free(t);
	rat_dtor(rat);

	emit_call_main_endloop(ibu);

	return true;
}

bool compile_and_write_avr(struct AST* ast, struct Ctx* ctx) {

	bool status = true;
	struct IBuffer* ibu = ibu_ctor();

	emit_defs(ibu);

	if (!avr_prologue(ctx, ibu)) {
		status = false;
		goto exit;
	}

	//convert AST into 3 address code with temporaries, use recursive descent to make TAC
	for (size_t i = 0; i < ast->count_namespaces; i++) {
		struct Namespace* ns = ast->namespaces[i];

		for (size_t j = 0; j < ns->count_methods; j++) {
			struct Method* m = ns->methods[j];

			compile_and_write_avr_single_function(m, ctx, ibu);
		}
	}

	FILE* fout = fopen(flags_asm_filename(ctx_flags(ctx)), "w");
	if (fout == NULL) {
		fprintf(stderr, "error opening output file\n");
		status = false;
		goto exit;
	}

	//TODO: figure out how to support something like .data on AVR.
	assert(data_count(ctx_tables(ctx)->data) == 0);

	ibu_write(ibu, fout);

	fclose(fout);

exit:

	ibu_dtor(ibu);

	return status;
}
