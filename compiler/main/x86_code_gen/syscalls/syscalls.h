#pragma once

#include <stdint.h>
#include <stdbool.h>

struct Syscall {
	char* name;
	uint64_t number;
};

uint64_t syscall_number(char* name);
