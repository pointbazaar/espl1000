#ifndef TYPECHECKING_ERRORS_H
#define TYPECHECKING_ERRORS_H


#define ERR_NUM_ARGS "number of arguments does not match"

#define ERR_ARG_TYPES "type of argument does not match"

#define ERR_CONDITION_REQUIRES_BOOL "condition must be of type bool"

#define ERR_LOOP_REQUIRES_INT "loop stmt requires an integer number of iterations "

#define ERR_RANGE_REQUIRES_INT "range requires 2 integer values"

#define ERR_RETURN_TYPE_MISMATCH "return type does not match returned type"

#define ERR_SWITCH_REQUIRES_PRIMITIVE_TYPE "switch requires a primitive type"

#define ERR_ASSIGN_TYPES_MISMATCH "Type mismatch in assignment"

#define ERR_CASE_TYPE_MISMATCH "A Case must have the same type as the value being switched on"

void error(char* msg);

struct Method* currentFn;

struct ST* myst;

#endif
