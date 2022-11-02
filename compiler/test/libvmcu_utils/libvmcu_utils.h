#ifndef LIBVMCU_UTILS_H
#define LIBVMCU_UTILS_H

//utilities for testing with libvmcu.

#include "libvmcu/libvmcu_system.h"
#include "libvmcu/libvmcu_analyzer.h"

void vmcu_system_step_n(vmcu_system_t* system, uint32_t count);

uint16_t vmcu_system_read_sp(vmcu_system_t* system);

uint16_t vmcu_system_read_y(vmcu_system_t* system);

#endif
