#include <string.h>
#include <inttypes.h>
#include <stdbool.h>

#include "flag.h"

struct Flag all_flags[] = {
    {
	"help",
	"display help text",
	false,
	false,
    },
    {
	"debug",
	"print debug statements",
	false,
	false,
    },
    {
	"version",
	"display version text",
	false,
	false,
    },
    {
	"h",
	"emit .h (header) files",
	false,
	false,
    },
    {
	"avr",
	"compile for AVR (default)",
	false,
	false,
    },
    {
	"x86",
	"compile for x86-64",
	false,
	false,
    },
    {
	"print-filenames",
	"print associated filenames for a .dg file",
	false,
	false,
    },
    {
	"lexer",
	"only run the lexer for a .dg file",
	false,
	false,
    },
    {
	"parser",
	"only run the lexer and parser for a .dg file",
	false,
	false,
    },
};

size_t all_flags_count() {
	return sizeof(all_flags) / sizeof(struct Flag);
}
