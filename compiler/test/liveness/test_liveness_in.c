#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "tac/tacbuffer.h"
#include "liveness/liveness.h"

#include "test_liveness.h"

static void status(char* msg) {
	printf(" - [TEST] Liveness %s\n", msg);
}

void test_liveness_in_map() {

	status("'in' map");

	struct TACBuffer* buf = tacbuffer_ctor();

	// t0 = 999
	tacbuffer_append(buf, makeTACConst(0, 0, 999));
	// t1 = t0
	tacbuffer_append(buf, makeTACCopy(0, 1, 0));
	// t2 = t1
	tacbuffer_append(buf, makeTACCopy(0, 2, 1));
	// return t3
	tacbuffer_append(buf, makeTACReturn(0, 2));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	//liveness_print(live);

	assert(live != NULL);

	assert(liveness_in(live, 0, 0) == false);
	assert(liveness_in(live, 0, 1) == false);
	assert(liveness_in(live, 0, 2) == false);

	assert(liveness_in(live, 1, 0) == true);
	assert(liveness_in(live, 1, 1) == false);
	assert(liveness_in(live, 1, 2) == false);

	assert(liveness_in(live, 2, 0) == false);
	assert(liveness_in(live, 2, 1) == true);
	assert(liveness_in(live, 2, 2) == false);

	assert(liveness_in(live, 3, 0) == false);
	assert(liveness_in(live, 3, 1) == false);
	assert(liveness_in(live, 3, 2) == true);

	tacbuffer_dtor(buf);
	liveness_dtor(live);
}
