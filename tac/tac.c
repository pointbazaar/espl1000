#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "tacbuffer.h"
#include "tac.h"

int32_t tac_opt_dest(struct TAC* tac) {

	switch (tac->kind) {
		case TAC_CONST_VALUE:
		case TAC_BINARY_OP:
		case TAC_BINARY_OP_IMMEDIATE:
		case TAC_CALL:
		case TAC_COPY:
		case TAC_UNARY_OP:
		case TAC_LOAD:
		case TAC_LOAD_LOCAL:
		case TAC_LOAD_LOCAL_ADDR:
		case TAC_LOAD_CONST_ADDR:
			return tac->dest;

		case TAC_STORE:
		case TAC_STORE_LOCAL:
		case TAC_STORE_CONST_ADDR:
		case TAC_PARAM:
		case TAC_SETUP_STACKFRAME:
		case TAC_SETUP_SP:
		case TAC_RETURN:
		case TAC_NOP:
		case TAC_LABEL_INDEXED:
		case TAC_LABEL_FUNCTION:
		case TAC_GOTO:
		case TAC_IF_GOTO:
		case TAC_IF_CMP_GOTO:
			return -1;

		default:
			fprintf(stderr, "%s: unhandled case %d\n", __func__, tac->kind);
			exit(1);
	}
}

static void check_bounds(uint64_t index, size_t size) {

	if (index >= size) {
		fprintf(stderr, "%ld (>= %ld) is out of bounds\n", index, size);
		exit(1);
	}
}

int tac_mark_used(struct TAC* tac, bool* used_map, size_t map_size) {

	switch (tac->kind) {
		case TAC_PARAM:
		case TAC_RETURN:
		case TAC_BINARY_OP_IMMEDIATE:
			check_bounds(tac->dest, map_size);
			used_map[tac->dest] = true;
			return 0;
			break;
		case TAC_IF_GOTO:
		case TAC_STORE_LOCAL:
		case TAC_STORE_CONST_ADDR:
		case TAC_LOAD:
		case TAC_STORE:
		case TAC_UNARY_OP:
		case TAC_COPY:
			check_bounds(tac->arg1, map_size);
			used_map[tac->arg1] = true;
			return 0;
			break;
		case TAC_BINARY_OP:
		case TAC_IF_CMP_GOTO:
			check_bounds(tac->dest, map_size);
			check_bounds(tac->arg1, map_size);
			used_map[tac->dest] = true;
			used_map[tac->arg1] = true;
			return 0;
			break;

		case TAC_CALL:
		case TAC_SETUP_STACKFRAME:
		case TAC_SETUP_SP:
		case TAC_LABEL_FUNCTION:
		case TAC_GOTO:
		case TAC_LABEL_INDEXED:
		case TAC_LOAD_LOCAL:
		case TAC_LOAD_LOCAL_ADDR:
		case TAC_LOAD_CONST_ADDR:
		case TAC_NOP:
		case TAC_CONST_VALUE:
			return 0;

		default:
			fprintf(stderr, "%s: unexpected case %u\n", __func__, tac->kind);
			exit(1);
			return 1;
	}
}

int tac_mark_defines(struct TAC* tac, bool* defines_map, size_t map_size) {

	switch (tac->kind) {
		case TAC_PARAM:
		case TAC_RETURN:
		case TAC_IF_GOTO:
		case TAC_STORE_LOCAL:
		case TAC_STORE_CONST_ADDR:
		case TAC_STORE:
		case TAC_IF_CMP_GOTO:
		case TAC_SETUP_STACKFRAME:
		case TAC_SETUP_SP:
		case TAC_LABEL_FUNCTION:
		case TAC_GOTO:
		case TAC_LABEL_INDEXED:
		case TAC_NOP:
			return 0;
			break;
		case TAC_BINARY_OP_IMMEDIATE:
		case TAC_LOAD:
		case TAC_UNARY_OP:
		case TAC_COPY:
		case TAC_BINARY_OP:
		case TAC_CALL:
		case TAC_LOAD_LOCAL:
		case TAC_LOAD_LOCAL_ADDR:
		case TAC_LOAD_CONST_ADDR:
		case TAC_CONST_VALUE:
			check_bounds(tac->dest, map_size);
			defines_map[tac->dest] = true;
			return 0;
			break;

		default:
			fprintf(stderr, "%s: unexpected case %u\n", __func__, tac->kind);
			exit(1);
			return 1;
	}
}

bool tac_is_unconditional_jump(struct TAC* tac) {
	return tac->kind == TAC_GOTO || tac->kind == TAC_RETURN;
}
