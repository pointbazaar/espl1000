#pragma once

#include <inttypes.h>
#include <stdbool.h>

struct Flag {
	// e.g. -avr
	char* name;

	char* description;

	bool has_arg;

	// should be false in the flags array
	bool is_set;
};
