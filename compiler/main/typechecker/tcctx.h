#ifndef TC_CTX_H
#define TC_CTX_H

#include <inttypes.h>
#include <stdbool.h>

struct TCError;

enum TC_ERR_KIND {
	//the kinds of type errors that are possible in a smalldragon program

	TC_ERR_ARG_NUM_MISMATCH,
	TC_ERR_ARG_TYPE_MISMATCH,

	TC_ERR_WRONG_RETURN_TYPE,

	TC_ERR_BINOP_TYPE_MISMATCH,

	TC_ERR_SUBR_NOT_FOUND,
	TC_ERR_VAR_NOT_FOUND,

	TC_ERR_VAR_READONLY,
	TC_ERR_ASSIGN_TYPE_MISMATCH,

	TC_ERR_CALLED_THROWING_WRONG,
	TC_ERR_RANGE_REQUIRES_INT,
	TC_ERR_CONDITION_REQUIRES_BOOL,

	//unary operator
	TC_ERR_WRONG_OP_UNOP,

	//indices
	TC_ERR_INDEX_NOT_INTEGER_TYPE,
	TC_ERR_TOO_MANY_INDICES,

	TC_ERR_LOCAL_VAR_NOT_A_SUBROUTINE,
	TC_ERR_SIDE_EFFECT_IN_PURE_CONTEXT,

	TC_ERR_OTHER,

	TC_ERR_END
};

struct TCError {

	char* opt_msg; //may be NULL
	enum TC_ERR_KIND err_kind;
	char* filename;
	uint32_t line_num;

	char* opt_snippet; //may be NULL

	struct TCError* next;
};

struct TCCtx {

	struct ST* st;
	struct Method* current_fn;

	uint16_t depth_inside_loop;

	//linked list of errors
	struct TCError* tc_first_err;
	struct TCError* tc_last_err;
	uint32_t tc_err_count;
	bool print_errors;

	char* current_filename;
	uint32_t current_line_num;

	bool debug;
};

#endif
