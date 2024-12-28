#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "tac.h"

struct Ctx;
struct ST;
struct SST;
struct LVST;

//this is our 2-Address Code
//it easily maps onto avr_code_gen assembly
//which assumes 1 operation and 2 registers,
//dest and src.
//where dest = dest op src
struct TAC {

	// this is the PRIVATE struct definition, it should
	// only be included in implementation files in tac/

	//three address code
	uint32_t label_index;

	uint32_t dest;
	enum TAC_KIND kind;
	uint64_t arg1;

	enum TAC_OP op;

	int64_t const_value;
};
