#pragma once

//utilities for testing with libvmcu.

#include "libvmcu/libvmcu_system.h"
#include "libvmcu/libvmcu_analyzer.h"

void vmcu_system_step_n(vmcu_system_t* system, uint32_t count);

// Do 1 step and compare register values.
// Print any register values which have changed.
void vmcu_system_step_debug(vmcu_system_t* system);

void vmcu_system_step_n_debug(vmcu_system_t* system, uint32_t count);

uint16_t vmcu_system_read_sp(vmcu_system_t* system);

uint16_t vmcu_system_read_y(vmcu_system_t* system);

uint16_t vmcu_system_read_data16(vmcu_system_t* system, uint16_t addr);

// read 16 bit value from 2 adjacent gprs
uint16_t vmcu_system_read_2_gpr(vmcu_system_t* system, const int low_reg);
