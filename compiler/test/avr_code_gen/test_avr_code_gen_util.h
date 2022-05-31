#ifndef SMALLDRAGON_TOPLEVEL_TEST_AVR_CODE_GEN_UTIL_H
#define SMALLDRAGON_TOPLEVEL_TEST_AVR_CODE_GEN_UTIL_H

#include "../../../dependencies/libvmcu-Virtual-MCU-Library/engine/include/libvmcu/libvmcu_system.h"
struct TACBuffer;

//compile a struct TACBuffer* to .asm
//call avra to create .hex
//create vmcu_model_t
//create vmcu_report_t
//create a vmcu_system_t
//return vmcu_system_t*

vmcu_system_t* prepare_vmcu_system_from_tacbuffer(struct TACBuffer* buffer);

#endif
