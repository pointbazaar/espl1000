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

	// used for TAC_CONST_VALUE (literal value)
	// used for TAC_CONST_DATA (as data table offset)
	int64_t const_value;

	// only used for TAC_PARAM.
	// 0 based.
	// e.g. calling f(t1, t2);
	// means t1 is at param_index == 0
	uint32_t param_index;

	// only used for TAC_LOAD*
	// and TAC_STORE*.
	// width of a load/store operation, in bytes.
	// specifies how many bytes to load.
	//
	// e.g. load_store_width = 8 for TAC_LOAD
	// means that we load 8 bytes from memory
	uint8_t load_store_width;
};
