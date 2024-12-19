#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "libvmcu/libvmcu_system.h"
#include "libvmcu/libvmcu_analyzer.h"

#include "test_avr_code_gen_util.h"
#include "avr_code_gen/cg_avr.h"
#include "avr_code_gen/cg_avr_single_function.h"
#include "tac/tacbuffer.h"
#include "basic_block/basicblock.h"
#include "analyzer/lv/lv_analyzer.h"
#include "avr_code_gen/cg_avr_basic_block.h"
#include "util/fill_tables.h"

#include "cli/flags/flags.h"

#include "ibuffer/ibuffer.h"

//compile a struct TACBuffer* to .asm
//call avra to create .hex
//create vmcu_model_t
//create vmcu_report_t
//create a vmcu_system_t
//return vmcu_system_t*

static void print_defs(FILE* fout);

void assert_redzone(vmcu_system_t* system, uint16_t addr, uint8_t width, uint8_t redzone) {

	assert(width <= 8);

	const uint16_t addr1 = addr - 1;
	const uint16_t addr2 = addr + width;

	const uint8_t before = vmcu_system_read_data(system, addr1);
	const uint8_t after = vmcu_system_read_data(system, addr2);

	if ((before != redzone) || (after != redzone)) {
		for (uint16_t a = addr1 - 1; a <= addr2 + 1; a++) {
			const uint8_t value = vmcu_system_read_data(system, a);
			printf("[0x%x] = 0x%x\n", a, value);
		}
	}
	assert(before == redzone);
	assert(after == redzone);
}

vmcu_system_t* prepare_vmcu_system_from_tacbuffer_with_redzone(struct TACBuffer* buffer, uint16_t addr_redzone, uint8_t redzone) {
	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(buffer);

	// fill the area with red zone to detect unintended writes
	// and reads around the address
	for (uint16_t a = addr_redzone - 2; a < addr_redzone + 4; a++) {
		vmcu_system_write_data(system, a, redzone);
	}

	return system;
}

vmcu_system_t* prepare_vmcu_system_from_tacbuffer(struct TACBuffer* buffer) {

	//create the file
	FILE* falibi = fopen(".file.dg", "w");

	if (falibi == NULL) {
		printf("error opening output file\n");
		exit(1);
	}

	fclose(falibi);

	struct Flags* flags = makeFlagsSingleFile(".file.dg");

	struct Ctx* ctx = ctx_ctor(flags, st_ctor(false));

	FILE* fout = fopen(flags_asm_filename(flags), "w");

	if (fout == NULL) {
		printf("error opening output file\n");
		exit(1);
	}

	print_defs(fout);

	int nblocks;
	struct BasicBlock** graph = basicblock_create_graph(buffer, "main", &nblocks, ctx);

	struct BasicBlock* root = graph[0];

	if (root == NULL) {
		printf("[Error] could not create BasicBlock.Exiting.\n");
		exit(1);
	}

	struct IBuffer* ibu = ibu_ctor();

	emit_asm_avr_basic_block(root, ctx, ibu);

	for (int i = 0; i < nblocks; i++) {
		basicblock_dtor(graph[i]);
	}

	ibu_write(ibu, fout);

	free(graph);

	tacbuffer_dtor(buffer);

	ibu_dtor(ibu);

	fclose(fout);

	char cmd[200];
	sprintf(cmd, "avra %s > /tmp/avra-stdout 2> /tmp/avra-stderr", flags_asm_filename(flags));

	int status = system(cmd);

	int status2 = WEXITSTATUS(status);

	if (status2 != 0) {
		printf("error with avra, see /tmp/avra-stdout, /tmp/avra-stderr \n");
		exit(1);
	}

	vmcu_model_t* model = vmcu_model_ctor(VMCU_DEVICE_M328P);

	if (model == NULL) {
		printf("[Error] could not prepare vmcu_model_t. Exiting.\n");
		exit(1);
	}

	vmcu_report_t* report = vmcu_analyze_file(flags_hex_filename(flags), model);

	if (report == NULL) {
		printf("[Error] could not prepare vmcu_report_t. Exiting.\n");
		exit(1);
	}

	vmcu_system_t* system = vmcu_system_ctor(report);

	if (system == NULL) {
		printf("[Error] could not prepare vmcu_system_t. Exiting.\n");
		exit(1);
	}

	vmcu_report_dtor(report);
	vmcu_model_dtor(model);

	ctx_dtor(ctx);

	return system;
}

static void print_defs(FILE* fout) {

	//in /usr/share/avra

	//in this file a comment must be shortened, otherwise avra will give an error
	//.INCLUDE "/usr/share/avra/m32def.inc"
	//fprintf(fout, ".DEVICE ATmega328P\n");

	//we do not want to depend on the specific location of that file
	//or if it's even there ... just append some stuff here
	fprintf(fout,
	        ".DEVICE ATmega328P\n"
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
