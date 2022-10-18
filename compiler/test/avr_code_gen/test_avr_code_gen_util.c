
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

static struct Ctx* prep_ctx(){
    struct Ctx* ctx = malloc(sizeof(struct Ctx));
    struct Flags* flags = malloc(sizeof(struct Flags));
    flags->debug = false;

    ctx->tables = makeST(false);

    ctx->error = false;
    ctx->flags = flags;

    ctx->indent_level = 0;
    ctx->file         = NULL;

    //fill_tables(ast, ctx);

    return ctx;
}

//compile a struct TACBuffer* to .asm
//call avra to create .hex
//create vmcu_model_t
//create vmcu_report_t
//create a vmcu_system_t
//return vmcu_system_t*

vmcu_system_t* prepare_vmcu_system_from_tacbuffer(struct TACBuffer* buffer){

    struct Ctx* ctx = prep_ctx();

    //TODO
    FILE* fout = fopen(".file.asm", "w");
    if(fout == NULL){
        printf("error opening output file\n");
        exit(1);
    }

    //in /usr/share/avra

    //in this file a comment must be shortened, otherwise avra will give an error
    fprintf(fout, ".INCLUDE \"/usr/share/avra/m32def.inc\"\n");
    //fprintf(fout, ".DEVICE ATmega328P\n");

    emit_setup_stack_pointer_avr(fout);
    emit_call_main_endloop(fout);

    //create basic blocks from this TAC

    struct BasicBlock* root = basicblock_create_graph(buffer, "main");

    if(root == NULL){
        printf("[Error] could not create BasicBlock.Exiting.\n");
        exit(1);
    }

    //populate ctx->st->lvst
    //lvst_clear(ctx->tables->lvst);
    //lvst_fill(m, ctx->tables);

    //emit label for the function
    //fprintf(fout, "%s:\n",m->decl->name);
    fprintf(fout, "%s:\n", "main");


    const size_t stack_frame_size = 0; // = lvst_stack_frame_size_avr(ctx->tables->lvst)

    emit_create_stack_frame(stack_frame_size, fout);

    //now load X as our base pointer for the stack frame
    fprintf(fout, "in r28, SPL  ;Y is base ptr\n");
    fprintf(fout, "in r29, SPH  ;Y is base ptr\n\n");

    emit_asm_avr_basic_block(root, ctx, fout);

    freeST(ctx->tables);
    free(ctx->flags);
    free(ctx);

    tacbuffer_dtor(buffer);

    fclose(fout);

    char cmd[200];
    //we pipe stdout and stderr away
    sprintf(cmd, "avra -o out %s > /tmp/avra-stdout 2> /tmp/avra-stderr", ".file.asm");
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

    return system;
}