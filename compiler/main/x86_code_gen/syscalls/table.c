#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "syscalls.h"

// TODO: enter all syscalls here
struct Syscall syscalls[] = {
    {
	.name = "read",
	.number = 0,
    },
    {
	.name = "write",
	.number = 1,
    },
    {
	.name = "open",
	.number = 2,
    },
    {
	.name = "close",
	.number = 3,
    },
    {
	.name = "lseek",
	.number = 8,
    },
    {
	.name = "mmap",
	.number = 9,
    },
    {
	.name = "exit",
	.number = 60,
    },
    {
	.name = "kill",
	.number = 62,
    },

    {
	// marks end of array
	.name = NULL,
	.number = 0,
    },
};
