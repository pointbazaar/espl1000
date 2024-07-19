//utilities for testing with libvmcu.

#include "libvmcu/libvmcu_system.h"
#include "libvmcu/libvmcu_analyzer.h"

#include "libvmcu_utils.h"

void vmcu_system_step_n(vmcu_system_t* system, uint32_t count) {

	for (uint32_t i = 0; i < count; i++) {
		vmcu_system_step(system);
	}
}

uint16_t vmcu_system_read_sp(vmcu_system_t* system) {

	const uint32_t io_offset = 0x20;

	//the 'out' instruction receives an offset to the
	//base address off the io space within the data space

	int8_t SPH_value = vmcu_system_read_data(system, io_offset + 0x3e);
	int8_t SPL_value = vmcu_system_read_data(system, io_offset + 0x3d);

	//printf("SPH = %04x, SPL = %04x\n", SPH_value, SPL_value);

	uint16_t sp = (SPH_value << 8 | SPL_value);

	return sp;
}

uint16_t vmcu_system_read_y(vmcu_system_t* system) {

	int8_t YL = vmcu_system_read_gpr(system, 28);
	int8_t YH = vmcu_system_read_gpr(system, 29);

	return (YH << 8 | YL);
}
