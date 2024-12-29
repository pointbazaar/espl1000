#pragma once

#include <stdbool.h>
#include <inttypes.h>
#include <stddef.h>

struct RAT;

void rat_init_x86(struct RAT* rat);

enum SD_REGISTER rat_return_reg_x86();
enum SD_REGISTER rat_base_ptr_x86();
enum SD_REGISTER rat_stack_ptr_x86();
enum SD_REGISTER rat_scratch_reg_x86();

enum SD_REGISTER rat_param_reg_x86(uint32_t index);

// x86 specific
char* rat_regname_x86(size_t i);
void rat_print_regname_x86(struct RAT* rat, size_t reg);
