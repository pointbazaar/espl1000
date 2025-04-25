#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "compiler/main/typechecker/tcctx.h"

#include "test_typechecker.h"
#include "test_typechecker_util.h"
#include "typechecker/tcctx.h"

static void status_test_typechecker(char* msg) {
	printf(" - [TEST] %s\n", msg);
}

static void free_tc_errors(struct TCError* error) {
	if (error->next != NULL) {
		free_tc_errors(error->next);
	}
	free(error);
}

void test_typecheck_wrong_assign_primitive() {

	status_test_typechecker("typecheck wrong assign primitive");
	char* filename = "compiler/test/typechecker/test-src/assign_primitive.dg";

	struct TCError* errors = typecheck_file(filename);

	assert(errors != NULL);
	assert(errors->err_kind == TC_ERR_ASSIGN_TYPE_MISMATCH);
	assert(errors->next == NULL);

	free_tc_errors(errors);
}

void test_typecheck_wrong_number_of_args() {

	status_test_typechecker("typecheck wrong number of args");
	char* filename = "compiler/test/typechecker/test-src/wrong_number_of_args.dg";

	struct TCError* errors = typecheck_file(filename);

	assert(errors != NULL);
	assert(errors->err_kind == TC_ERR_ARG_NUM_MISMATCH);
	assert(errors->next == NULL);

	free_tc_errors(errors);
}

void test_typecheck_wrong_type_of_arg() {

	status_test_typechecker("typecheck wrong type of arg");
	char* filename = "compiler/test/typechecker/test-src/wrong_type_of_arg.dg";

	struct TCError* errors = typecheck_file(filename);

	assert(errors != NULL);
	assert(errors->err_kind == TC_ERR_ARG_TYPE_MISMATCH);
	assert(errors->next == NULL);

	free_tc_errors(errors);
}

void test_typecheck_wrong_return_type() {

	status_test_typechecker("typecheck wrong return type");
	char* filename = "compiler/test/typechecker/test-src/wrong_return_type.dg";

	struct TCError* errors = typecheck_file(filename);

	assert(errors != NULL);
	assert(errors->err_kind == TC_ERR_WRONG_RETURN_TYPE);
	assert(errors->next == NULL);

	free_tc_errors(errors);
}

void test_typecheck_binop_type_mismatch() {

	status_test_typechecker("typecheck binop type mismatch");
	char* filename = "compiler/test/typechecker/test-src/binop_type_mismatch.dg";

	struct TCError* errors = typecheck_file(filename);

	assert(errors != NULL);
	assert(errors->err_kind == TC_ERR_BINOP_TYPE_MISMATCH);
	assert(errors->next == NULL);

	free_tc_errors(errors);
}

void test_typecheck_subr_not_found() {

	status_test_typechecker("typecheck subr not found");
	char* filename = "compiler/test/typechecker/test-src/subr_not_found.dg";

	struct TCError* errors = typecheck_file(filename);

	assert(errors != NULL);
	assert(errors->err_kind == TC_ERR_VAR_NOT_FOUND);
	assert(errors->next == NULL);

	free_tc_errors(errors);
}

void test_typecheck_impure_called_in_pure() {

	status_test_typechecker("typecheck impure called in pure");
	char* filename = "compiler/test/typechecker/test-src/impure_called_in_pure.dg";

	struct TCError* errors = typecheck_file(filename);

	assert(errors != NULL);
	assert(errors->err_kind == TC_ERR_SIDE_EFFECT_IN_PURE_CONTEXT);
	assert(errors->next == NULL);

	free_tc_errors(errors);
}

void test_typecheck_condition_requires_bool() {

	status_test_typechecker("typecheck condition requires bool");
	char* filename = "compiler/test/typechecker/test-src/condition_requires_bool.dg";

	struct TCError* errors = typecheck_file(filename);

	assert(errors != NULL);
	assert(errors->err_kind == TC_ERR_CONDITION_REQUIRES_BOOL);
	assert(errors->next == NULL);

	free_tc_errors(errors);
}

void test_typecheck_range_requires_int() {

	status_test_typechecker("typecheck range requires int");
	char* filename = "compiler/test/typechecker/test-src/range_requires_int.dg";

	struct TCError* errors = typecheck_file(filename);

	assert(errors != NULL);
	assert(errors->err_kind == TC_ERR_RANGE_REQUIRES_INT);
	assert(errors->next == NULL);

	free_tc_errors(errors);
}

void test_typecheck_wrong_op_unop() {

	status_test_typechecker("typecheck wrong op unop");
	char* filename = "compiler/test/typechecker/test-src/wrong_op_unop.dg";

	struct TCError* errors = typecheck_file(filename);

	assert(errors != NULL);
	assert(errors->err_kind == TC_ERR_WRONG_OP_UNOP);
	assert(errors->next == NULL);

	free_tc_errors(errors);
}

void test_typecheck_too_many_indices() {

	status_test_typechecker("typecheck too many indices");
	char* filename = "compiler/test/typechecker/test-src/too_many_indices.dg";

	struct TCError* errors = typecheck_file(filename);

	assert(errors != NULL);

	assert(errors->err_kind == TC_ERR_TOO_MANY_INDICES);
	assert(errors->next == NULL);

	free_tc_errors(errors);
}

void test_typecheck_local_var_not_a_subroutine() {

	status_test_typechecker("typecheck local var not a subroutine");
	char* filename = "compiler/test/typechecker/test-src/local_var_not_a_subroutine.dg";

	struct TCError* errors = typecheck_file(filename);

	assert(errors != NULL);
	assert(errors->err_kind == TC_ERR_LOCAL_VAR_NOT_A_SUBROUTINE);
	assert(errors->next == NULL);

	free_tc_errors(errors);
}

void test_typecheck_var_not_found() {

	status_test_typechecker("typecheck var not found");
	char* filename = "compiler/test/typechecker/test-src/var_not_found.dg";

	struct TCError* errors = typecheck_file(filename);

	assert(errors != NULL);
	assert(errors->err_kind == TC_ERR_VAR_NOT_FOUND);
	assert(errors->next == NULL);

	free_tc_errors(errors);
}

void test_typecheck_index_not_found() {

	status_test_typechecker("typecheck index not found");
	char* filename = "compiler/test/typechecker/test-src/index_not_found.dg";

	struct TCError* errors = typecheck_file(filename);

	assert(errors != NULL);
	assert(errors->err_kind == TC_ERR_VAR_NOT_FOUND);
	assert(errors->next == NULL);

	free_tc_errors(errors);
}

void test_typecheck_no_return_stmt() {

	status_test_typechecker("typecheck return stmt not found");
	char* filename = "compiler/test/typechecker/test-src/no_return_stmt.dg";

	struct TCError* errors = typecheck_file(filename);

	assert(errors != NULL);
	assert(errors->err_kind == TC_ERR_MUST_RETURN);
	assert(errors->next == NULL);

	free_tc_errors(errors);
}

void test_typecheck_all_type_errors() {

	status_test_typechecker("typecheck all type errors");
	char* filename = "compiler/test/typechecker/test-src/all_errors.dg";

	struct TCError* errors_orig = typecheck_file(filename);

	assert(errors_orig != NULL);

	struct TCError* errors = errors_orig;

	enum TC_ERR_KIND err_kinds[100];
	uint32_t err_count = 0;

	do {
		err_kinds[err_count++] = errors->err_kind;
		errors = errors->next;
	} while (errors != NULL);

	assert(err_count == 15);

	enum TC_ERR_KIND err_kind_expected[] = {
	    TC_ERR_VAR_NOT_FOUND,
	    TC_ERR_VAR_NOT_FOUND,
	    TC_ERR_CONDITION_REQUIRES_BOOL,
	    TC_ERR_CONDITION_REQUIRES_BOOL,
	    TC_ERR_RANGE_REQUIRES_INT,
	    TC_ERR_BINOP_TYPE_MISMATCH,
	    TC_ERR_ASSIGN_TYPE_MISMATCH,
	    TC_ERR_VAR_NOT_FOUND,
	    TC_ERR_INDEX_NOT_INTEGER_TYPE,
	    TC_ERR_TOO_MANY_INDICES,
	    TC_ERR_LOCAL_VAR_NOT_A_SUBROUTINE,
	    TC_ERR_ARG_TYPE_MISMATCH,
	    TC_ERR_ARG_NUM_MISMATCH,
	    TC_ERR_WRONG_OP_UNOP,
	    TC_ERR_WRONG_RETURN_TYPE};

	for (uint32_t i = 0; i < err_count; i++) {
		assert(err_kinds[i] == err_kind_expected[i]);
	}

	free_tc_errors(errors_orig);
}
