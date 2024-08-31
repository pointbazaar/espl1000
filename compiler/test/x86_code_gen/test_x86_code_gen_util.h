#pragma once

#include "unicorn/unicorn.h"

#define TEST_FAKE_STACKFRAME_SIZE 4

struct TACBuffer;
struct sd_uc_engine;

struct sd_uc_engine* sd_uc_engine_from_tacbuffer_v2(struct TACBuffer* buffer, bool debug);
struct sd_uc_engine* sd_uc_engine_from_tacbuffer_v3(struct TACBuffer* buffer, bool debug, bool fake_lvst);

uc_err sd_uc_emu_start(struct sd_uc_engine* sd_uc, size_t nsteps, bool debug);

void sd_uc_close(struct sd_uc_engine* sduc);

void sd_uc_print_regs(struct sd_uc_engine* sduc);
// prints a section of the stack
void sd_uc_print_stack(struct sd_uc_engine* sduc);

uint64_t sd_uc_default_start_addr();
uint64_t sd_uc_default_stack_addr();

// writes 8 bytes (64 bits) by default
uc_err sd_uc_mem_write64(struct sd_uc_engine* sduc, uint64_t address, const void* bytes);

// reads 8 bytes (64 bits) by default
uc_err sd_uc_mem_read64(struct sd_uc_engine* sduc, uint64_t address, void* bytes);

uc_err sd_uc_reg_read(struct sd_uc_engine* sduc, int regid, void* value);

bool sd_uc_some_reg_has_value(struct sd_uc_engine* sduc, uint64_t value);
