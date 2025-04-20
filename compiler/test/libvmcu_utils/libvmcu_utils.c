//utilities for testing with libvmcu.

#include "libvmcu/libvmcu_system.h"
#include "libvmcu/libvmcu_analyzer.h"
#include <stdio.h>

#include "libvmcu_utils.h"

void vmcu_system_step_n(vmcu_system_t* system, uint32_t count) {

	for (uint32_t i = 0; i < count; i++) {
		vmcu_system_step(system);
	}
}

void vmcu_system_step_n_debug(vmcu_system_t* system, uint32_t count) {

	for (uint32_t i = 0; i < count; i++) {
		vmcu_system_step_debug(system);
	}
}

void vmcu_system_step_debug(vmcu_system_t* system) {

	static uint8_t old_regs[32] = {0};
	static uint8_t data[2048] = {0};

	vmcu_system_step(system);
	printf("[debug] ");

	for (int i = 0; i < 32; i++) {
		const uint8_t value = vmcu_system_read_gpr(system, i);

		if (value != old_regs[i]) {
			printf("r%d=0x%x ", i, value);
		}
		old_regs[i] = value;
	}

	// start at 32 since the registers are memory mapped
	for (int i = 32; i < 2048; i++) {
		const uint8_t value = vmcu_system_read_data(system, i);
		if (value != data[i]) {
			printf("[0x%x]=0x%x ", i, value);
		}
		data[i] = value;
	}
	printf("\n");
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

uint16_t vmcu_system_read_data16(vmcu_system_t* system, uint16_t addr) {

	const uint8_t low = vmcu_system_read_data(system, addr);
	const uint8_t high = vmcu_system_read_data(system, addr + 1);

	return (high << 8) | low;
}

uint32_t vmcu_system_read_data32(vmcu_system_t* system, uint16_t addr) {

	const uint32_t low = vmcu_system_read_data16(system, addr);
	const uint32_t high = vmcu_system_read_data16(system, addr + 16);
	return (high << 16) | low;
}

uint64_t vmcu_system_read_data64(vmcu_system_t* system, uint16_t addr) {

	const uint64_t low = vmcu_system_read_data16(system, addr);
	const uint64_t high = vmcu_system_read_data16(system, addr + 32);
	return (high << 32) | low;
}

void vmcu_system_write_data16(vmcu_system_t* system, uint16_t addr, uint16_t value) {

	vmcu_system_write_data(system, addr, value & 0xff);
	vmcu_system_write_data(system, addr + 1, (value >> 8) & 0xff);
}

uint16_t vmcu_system_read_2_gpr(vmcu_system_t* system, const int low_reg) {

	const uint8_t low = vmcu_system_read_gpr(system, low_reg);
	const uint8_t high = vmcu_system_read_gpr(system, low_reg + 1);
	return (high << 8) | low;
}
