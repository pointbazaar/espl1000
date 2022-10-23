#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "libvmcu/libvmcu_system.h"
#include "libvmcu/libvmcu_analyzer.h"

#include "compiler.h"
#include "../../../cli/flags/flags.h"

#include "../test_gen_tac.h"

vmcu_system_t* prepare_vmcu_system_from_code_snippet(char* code_snippet){
	
	//write snippet to .file.dg
	FILE* fout = fopen(".file.dg", "w");
	
	if(fout == NULL){
		printf("error with opening file, exiting.");
		fflush(stdout);
		exit(1);
	}
	
	fprintf(fout, "%s", code_snippet);
	
	fclose(fout);
	
	char* argv[] = {"program", ".file.dg"};
	int argc = 2;
	struct Flags* flags = makeFlags(argc, argv);
	
    compile(flags);

    vmcu_model_t* model = vmcu_model_ctor(VMCU_DEVICE_M328P);

    if(model == NULL){
        printf("[Error] could not prepare vmcu_model_t. Exiting.\n");
        exit(1);
    }

    vmcu_report_t* report = vmcu_analyze_file(".file.hex", model);

    if(report == NULL){
        printf("[Error] could not prepare vmcu_report_t. Exiting.\n");
        exit(1);
    }

    vmcu_system_t* system = vmcu_system_ctor(report);

    if(system == NULL){
        printf("[Error] could not prepare vmcu_system_t. Exiting.\n");
        exit(1);
    }
    
    vmcu_model_dtor(model);
    vmcu_report_dtor(report);

    return system;
}
