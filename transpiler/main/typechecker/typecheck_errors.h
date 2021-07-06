#ifndef TYPECHECKING_ERRORS_H
#define TYPECHECKING_ERRORS_H

#include "tcctx.h"

#define ERR_NUM_ARGS "number of args does not match"

#define ERR_ARG_TYPES "type of arg does not match"

#define ERR_CONDITION_REQUIRES_BOOL "condition must be of type bool"

#define ERR_LOOP_REQUIRES_INT "loop stmt requires an integer number of iterations "

#define ERR_RANGE_REQUIRES_INT "range requires 2 integer values"

#define ERR_RETURN_TYPE_MISMATCH "return type does not match returned type"

#define ERR_SWITCH_REQUIRES_PRIMITIVE_TYPE "switch requires a primitive type"

#define ERR_ASSIGN_TYPES_MISMATCH "type mismatch in assignment"

#define ERR_CASE_TYPE_MISMATCH "case must have same type as in switch"

#define ERR_SUBR_NOT_FOUND "subroutine was not found"

#define ERR_NO_THROW_OUTSIDE_TRY_OR_THROWS_SUBR "cannot use 'throw' outside try-block or 'throws' subroutine."

void error(struct TCCtx* tcctx, char* msg);

struct Method* currentFn;

struct ST* myst;

#endif
