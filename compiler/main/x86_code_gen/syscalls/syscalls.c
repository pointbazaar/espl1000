#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "syscalls.h"

extern struct Syscall syscalls[];

uint64_t syscall_number(char* name) {

	for (int i = 0; true; i++) {
		char* syscall_name = syscalls[i].name;
		if (strcmp(name, syscall_name) == 0) {
			return syscalls[i].number;
		}
		if (syscall_name == NULL) {
			break;
		}
	}
	fprintf(stderr, "syscall with name '%s' not found.\n", name);
	assert(false);
	return 1;
}
