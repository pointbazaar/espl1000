#include "tacbuffer.h"
#include "tac.h"

#include <stdio.h>

// @precondition  t1 immediately precedes t2
// @param i  index of t1
// @returns true if two IR statements may be swapped
static bool tacbuffer_optimize_swap_criterion(size_t i, struct TAC* t1, struct TAC* t2) {

	// Currently there are only very simplistic examples here
	// It can be improved in the future!
	// There are lots more opportunities to optimize

	enum TAC_KIND k1 = tac_kind(t1);
	enum TAC_KIND k2 = tac_kind(t2);

	// This optimizes the placement of TAC_PARAM.
	// If a param is used for function call/syscall, we can move
	// it to the needed register earlier, which shortens the live span
	// of the temporary, freeing up registers!
	if (k2 == TAC_PARAM) {
		int32_t dest2 = tac_dest(t2);

		if (k1 == TAC_CONST_VALUE) {
			int32_t dest1 = tac_opt_dest(t1);

			if (dest1 != dest2) {
				// TAC_CONST_VALUE instance does not define the needed
				// temporary for TAC_PARAM instance, safe to swap.
				return true;
			}
		}

		if (k1 == TAC_UNARY_OP) {
			int32_t dest1 = tac_opt_dest(t1);

			if (dest1 != dest2) {
				// TAC_CONST_VALUE instance does not define the needed
				// temporary for TAC_PARAM instance, safe to swap.
				return true;
			}
		}
	}

	return false;
}

// @returns 1 if something changed
// @returns 0   if nothing changed
// @returns -1  on error
static int32_t tacbuffer_optimize_iteration(struct TACBuffer* buffer, bool debug) {

	const size_t count = tacbuffer_count(buffer);

	for (size_t i = 0; i < count - 1; i++) {

		struct TAC* t1 = tacbuffer_get(buffer, i);
		struct TAC* t2 = tacbuffer_get(buffer, i + 1);

		if (!t1 || !t2) {
			return -1;
		}

		if (tacbuffer_optimize_swap_criterion(i, t1, t2)) {

			if (debug) {
				printf("TACBuffer optimization: swap %ld, %ld", i, i + 1);
			}

			if (!tacbuffer_swap(buffer, i, i + 1)) {
				return -1;
			}

			return 1;
		}
	}
	return 0;
}

bool tacbuffer_optimize_reorder(struct TACBuffer* buffer, bool debug) {

	if (debug) {
		printf("\nApplying TACBuffer reordering optimization\n");
	}

	bool changed = true;

	while (changed) {

		changed = false;

		int32_t status = tacbuffer_optimize_iteration(buffer, debug);

		if (status == 1) {
			changed = true;
		}

		if (status == -1) {
			return false;
		}
	}

	return true;
}
