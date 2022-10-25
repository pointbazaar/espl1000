
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

#include "../../cli/flags/flags.h"

//compile a struct TACBuffer* to .asm
//call avra to create .hex
//create vmcu_model_t
//create vmcu_report_t
//create a vmcu_system_t
//return vmcu_system_t*

vmcu_system_t* prepare_vmcu_system_from_tacbuffer(struct TACBuffer* buffer){

	//create the file 
	FILE* falibi = fopen(".file.dg", "w");
	
	if(falibi == NULL){
        printf("error opening output file\n");
        exit(1);
    }
    
    fclose(falibi);

	struct Flags* flags = makeFlagsSingleFile(".file.dg");

    struct Ctx* ctx = ctx_ctor(flags, makeST(false));

    FILE* fout = fopen(flags_asm_filename(flags), "w");
    
    if(fout == NULL){
        printf("error opening output file\n");
        exit(1);
    }

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
		".equ	SPL	= 0x3d\n"
    );

    emit_setup_stack_pointer_avr(fout);
    emit_call_main_endloop(fout);

    //create basic blocks from this TAC

	int nblocks;
    struct BasicBlock** graph = basicblock_create_graph(buffer, "main", &nblocks);
	
	struct BasicBlock* root = graph[0];

    if(root == NULL){
        printf("[Error] could not create BasicBlock.Exiting.\n");
        exit(1);
    }

    //populate ctx->st->lvst
    //lvst_clear(ctx->tables->lvst);
    //lvst_fill(m, ctx->tables);

    //emit label for the function
    fprintf(fout, "%s:\n", "main");

    emit_asm_avr_basic_block(root, ctx, fout);
    
    for(int i=0;i < nblocks; i++){
		basicblock_dtor(graph[i]);
	}
	free(graph);

    tacbuffer_dtor(buffer);

    fclose(fout);

    char cmd[200];
    //we pipe stdout and stderr away 
    sprintf(cmd, "avra %s > /tmp/avra-stdout 2> /tmp/avra-stderr", flags_asm_filename(flags));
    
    int status = system(cmd);

    int status2 = WEXITSTATUS(status);

    if(status2 != 0){
        printf("error with avra, see /tmp/avra-stdout, /tmp/avra-stderr \n");
        exit(1);
    }

    vmcu_model_t* model = vmcu_model_ctor(VMCU_DEVICE_M328P);

    if(model == NULL){
        printf("[Error] could not prepare vmcu_model_t. Exiting.\n");
        exit(1);
    }

    vmcu_report_t* report = vmcu_analyze_file(flags_hex_filename(flags), model);

    if(report == NULL){
        printf("[Error] could not prepare vmcu_report_t. Exiting.\n");
        exit(1);
    }

    vmcu_system_t* system = vmcu_system_ctor(report);

    if(system == NULL){
        printf("[Error] could not prepare vmcu_system_t. Exiting.\n");
        exit(1);
    }
    
    vmcu_report_dtor(report);
    vmcu_model_dtor(model);
    
    ctx_dtor(ctx);

    return system;
}
