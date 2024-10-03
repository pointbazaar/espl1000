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

// additionally creates a red zone around the given address,
// to be able to later check if any unintended writes or reads happened there
vmcu_system_t* prepare_vmcu_system_from_tacbuffer_with_redzone(struct TACBuffer* buffer, uint16_t addr_redzone, uint8_t redzone);

// assert that the values around the address still have 'redzone' value
void assert_redzone(vmcu_system_t* system, uint16_t addr, uint8_t width, uint8_t redzone);

#endif
