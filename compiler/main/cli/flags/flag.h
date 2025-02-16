#pragma once

#include <inttypes.h>
#include <stdbool.h>

struct Flag {
	// e.g. -avr
	char* name;

	char* description;

	bool has_arg;

	// can be NULL
	// if set, points into argv
	char* arg;

	// can be NULL
	// only set if 'has_arg == true'
	char* arg_default_value;

	// should be false in the flags array
	bool is_set;
};
