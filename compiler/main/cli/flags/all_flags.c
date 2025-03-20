#include <string.h>
#include <inttypes.h>
#include <stdbool.h>

#include "flag.h"

struct Flag all_flags[] = {
    {
	.name = "help",
	.description = "display help text",
	.has_arg = false,
	.arg_default_value = NULL,
	.is_set = false,
    },
    {
	.name = "debug",
	.description = "print debug statements",
	.has_arg = false,
	.arg_default_value = NULL,
	.is_set = false,
    },
    {
	.name = "version",
	.description = "display version text",
	.has_arg = false,
	.arg_default_value = NULL,
	.is_set = false,
    },
    {
	.name = "h",
	.description = "emit .h (header) files",
	.has_arg = false,
	.arg_default_value = NULL,
	.is_set = false,
    },
    {
	.name = "avr",
	.description = "compile for AVR (default)",
	.has_arg = false,
	.arg_default_value = NULL,
	.is_set = false,
    },
    {
	.name = "x86",
	.description = "compile for x86-64",
	.has_arg = false,
	.arg_default_value = NULL,
	.is_set = false,
    },
    {
	.name = "print-filenames",
	.description = "print associated filenames for a .dg file",
	.has_arg = false,
	.arg_default_value = NULL,
	.is_set = false,
    },
    {
	.name = "lexer",
	.description = "only run the lexer for a .dg file",
	.has_arg = false,
	.arg_default_value = NULL,
	.is_set = false,
    },
    {
	.name = "parser",
	.description = "only run the lexer and parser for a .dg file",
	.has_arg = false,
	.arg_default_value = NULL,
	.is_set = false,
    },
    {
	.name = "nolink",
	.description = "do not run the linker (ld). Useful if some functions are declared as extern.",
	.has_arg = false,
	.arg_default_value = NULL,
	.is_set = false,
    },
    {
	.name = "rat",
	.description = "only print a plain initialized RAT for the architecture",
	.has_arg = false,
	.arg_default_value = NULL,
	.is_set = false,
    },
    {
	.name = "o",
	.description = "output file path",
	.has_arg = true,
	.arg_default_value = "a.out",
	.is_set = false,
    },
    {
	.name = "dump-tokens",
	.description = "dump .tokens file from lexer",
	.has_arg = false,
	.arg_default_value = NULL,
	.is_set = false,
    },
    {
	.name = "dump-data",
	.description = "dump table for .data section",
	.has_arg = false,
	.arg_default_value = NULL,
	.is_set = false,
    },
    {
	.name = "stdlib",
	.description = "compile stdlib/ aswell",
	.has_arg = false,
	.arg_default_value = NULL,
	.is_set = false,
    },
};

size_t all_flags_count() {
	return sizeof(all_flags) / sizeof(struct Flag);
}
