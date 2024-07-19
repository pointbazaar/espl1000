#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "tables/symtable/symtable.h"

#include "ast/ast.h"

#include "../../cli/flags/flags.h"

#include "util/ctx.h"

#include "ibuffer/ibuffer.h"

#include "cg_avr.h"
#include "tac/tacbuffer.h"
#include "tac/tac.h"
#include "cg_avr_single_function.h"
#include "cg_avr_single_tac.h"

static void emit_defs(FILE* fout) {
	//in /usr/share/avra
	//in this file a comment must be shortened, otherwise avra will give an error
	//.INCLUDE "/usr/share/avra/m32def.inc"
	//fprintf(fout, ".DEVICE ATmega328P\n");

	//we do not want to depend on the specific location of that file
	//or if it's even there ... just append some stuff here
	fprintf(fout,
	        ".equ	RAMEND	= 0x085f\n"
	        ".def	XH	= r27\n"
	        ".def	XL	= r26\n"
	        ".def	YH	= r29\n"
	        ".def	YL	= r28\n"
	        ".def	ZH	= r31\n"
	        ".def	ZL	= r30\n"
	        ".equ	SPH	= 0x3e\n"
	        ".equ	SPL	= 0x3d\n");
}

void emit_call_main_endloop(struct IBuffer* ibu) {
	//create the endloop such that the mcu does
	//not start over again after main

	//'endloop'
	call("main", "");
	label("endloop");
	rjmp("endloop", "");
}

bool compile_and_write_avr(struct AST* ast, struct Ctx* ctx) {

	struct IBuffer* ibu = ibu_ctor();

	struct RAT* rat = rat_ctor(ctx_tables(ctx));

	struct TAC* t = makeTACSetupSP();
	emit_asm_avr_single_tac(rat, t, ctx, ibu);
	free(t);

	rat_dtor(rat);

	emit_call_main_endloop(ibu);

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
		printf("error opening output file\n");
		exit(1);
	}

	{
		emit_defs(fout);
		ibu_write(ibu, fout);
	}

	fclose(fout);

	ibu_dtor(ibu);

	return true;
}
