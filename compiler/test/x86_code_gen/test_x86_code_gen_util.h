#pragma once

#include "unicorn/unicorn.h"

struct TACBuffer;
struct sd_uc_engine;

struct sd_uc_engine* sd_uc_engine_from_tacbuffer(struct TACBuffer* buffer);

uc_err sd_uc_emu_start(struct sd_uc_engine* sd_uc, size_t nsteps, bool debug);

void sd_uc_close(struct sd_uc_engine* sduc);

void sd_uc_print_regs(struct sd_uc_engine* sduc);

uint64_t sd_uc_default_start_addr();

// writes 8 bytes (64 bits) by default
uc_err sd_uc_mem_write64(struct sd_uc_engine* sduc, uint64_t address, const void* bytes);

// reads 8 bytes (64 bits) by default
uc_err sd_uc_mem_read64(struct sd_uc_engine* sduc, uint64_t address, void* bytes);

uc_err sd_uc_reg_read(struct sd_uc_engine* sduc, int regid, void* value);
