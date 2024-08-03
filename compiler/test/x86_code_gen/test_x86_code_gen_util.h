#pragma once

#include "unicorn/unicorn.h"

struct TACBuffer;
struct sd_uc_engine;

struct sd_uc_engine {
	// wrapper struct

	// addresses of the emulated code
	uint64_t addr_start;
	uint64_t addr_end;

	uc_engine* uc;
};

struct sd_uc_engine* sd_uc_engine_from_tacbuffer(struct TACBuffer* buffer);

uc_err sd_uc_emu_start(struct sd_uc_engine* sd_uc, size_t nsteps, bool debug);

void sd_uc_print_regs(struct sd_uc_engine* sduc);

uint64_t sd_uc_default_start_addr();
