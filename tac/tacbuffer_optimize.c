#include "tacbuffer.h"
#include "tac.h"

#include <stdio.h>

// @precondition  t1 immediately precedes t2
// @param i  index of t1
// @returns true if two IR statements may be swapped
static bool tacbuffer_optimize_swap_criterion(struct TAC* t1, struct TAC* t2, bool debug) {

	(void)debug;

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
		const int32_t dest2 = tac_dest(t2);

		if (k1 == TAC_CONST_VALUE || k1 == TAC_CONST_DATA) {
			const int32_t dest1 = tac_opt_dest(t1);

			if (dest1 != dest2) {
				// TAC_CONST_VALUE instance does not define the needed
				// temporary for TAC_PARAM instance, safe to swap.
				return true;
			}
		}

		if (k1 == TAC_UNARY_OP || k1 == TAC_COPY || k1 == TAC_LOAD) {
			const int32_t dest1 = tac_opt_dest(t1);

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
	int status = 0;

	for (size_t i = 0; i < count - 1; i++) {

		struct TAC* t1 = tacbuffer_get(buffer, i);
		struct TAC* t2 = tacbuffer_get(buffer, i + 1);

		if (!t1 || !t2) {
			return -1;
		}

		if (tacbuffer_optimize_swap_criterion(t1, t2, debug)) {

			if (debug) {
				printf("   TACBuffer optimization: swap %ld, %ld\n", i, i + 1);
			}

			if (!tacbuffer_swap(buffer, i, i + 1)) {
				return -1;
			}

			status = 1;
		}
	}
	return status;
}

bool tacbuffer_optimize_reorder(struct TACBuffer* buffer, struct Flags* flags) {

	const bool debug = flags_debug(flags) || flags_debug_optimize_tacbuffer_reorder(flags);

	if (debug) {
		printf("\nApplying TACBuffer reordering optimization\n");
	}

	bool changed = true;
	size_t iteration = 1;

	while (changed) {

		if (debug) {
			printf(" TACBuffer reorder iteration: %ld\n", iteration);
		}

		changed = false;

		int32_t status = tacbuffer_optimize_iteration(buffer, debug);

		if (status == 1) {
			changed = true;
		}

		if (status == -1) {
			return false;
		}

		iteration++;
	}

	return true;
}
