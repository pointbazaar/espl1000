#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "libvmcu/libvmcu_system.h"
#include "libvmcu/libvmcu_analyzer.h"

#include "compiler.h"
#include "cli/flags/flags.h"

#include "../test_gen_tac.h"

vmcu_system_t* prepare_vmcu_system_from_code_snippet(char* code_snippet) {

	//write snippet to .file.dg
	char* filename = ".file.dg";

	FILE* fout = fopen(filename, "w");

	if (fout == NULL) {
		fprintf(stderr, "error with opening file, exiting.");
		fflush(stdout);
		return NULL;
	}

	fprintf(fout, "%s", code_snippet);

	fclose(fout);

	char* argv[] = {"program", filename};
	int argc = 2;
	struct Flags* flags = makeFlags(argc, argv, false);

	assert(flags);

	bool success = compile(flags);

	if (!success) {
		return NULL;
	}

	vmcu_system_t* system = NULL;

	vmcu_model_t* model = vmcu_model_ctor(VMCU_DEVICE_M328P);

	if (model == NULL) {
		fprintf(stderr, "[Error] could not prepare vmcu_model_t. Exiting.\n");
		return NULL;
	}

	vmcu_report_t* report = vmcu_analyze_file(".file.hex", model);

	if (report == NULL) {
		fprintf(stderr, "[Error] could not prepare vmcu_report_t. Exiting.\n");
		goto exit_vmcu_model;
	}

	system = vmcu_system_ctor(report);

	if (system == NULL) {
		fprintf(stderr, "[Error] could not prepare vmcu_system_t. Exiting.\n");
	}

exit_vmcu_report:
	vmcu_report_dtor(report);
exit_vmcu_model:
	vmcu_model_dtor(model);

	return system;
}
