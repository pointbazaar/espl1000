#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "test.h"

#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "util/status/status.h"

void test_tacbuffer_tostring() {

	status("tacbuffer_tostring");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACNop());
	tacbuffer_append(buf, makeTACReturn(1));

	char* s = tacbuffer_tostring(buf, false, NULL, NULL);

	char* expect = "nop\nreturn t1\n";

	assert(strcmp(s, expect) == 0);

	free(s);
	tacbuffer_dtor(buf);
}

void test_tacbuffer_indexof() {

	status("tacbuffer_indexof");

	struct TACBuffer* buf = tacbuffer_ctor();

	struct TAC* t1 = makeTACNop();
	struct TAC* t2 = makeTACReturn(1);
	struct TAC* t3 = makeTACParam(2, true, 4, false);

	tacbuffer_append(buf, t1);
	tacbuffer_append(buf, t2);

	assert(tacbuffer_indexof(buf, t1) == 0);
	assert(tacbuffer_indexof(buf, t2) == 1);
	assert(tacbuffer_indexof(buf, t3) == -1);

	free(t3);
	tacbuffer_dtor(buf);
}

void test_tacbuffer_set() {

	status("tacbuffer_set");

	struct TACBuffer* buf = tacbuffer_ctor();

	struct TAC* t1 = makeTACNop();
	struct TAC* t2 = makeTACReturn(1);

	tacbuffer_append(buf, t1);

	assert(tacbuffer_get(buf, 0) == t1);

	tacbuffer_set(buf, 0, t2);

	assert(tacbuffer_get(buf, 0) == t2);

	tacbuffer_dtor(buf);
	free(t1);
}

void test_tacbuffer_get() {

	status("tacbuffer_get");

	struct TACBuffer* buf = tacbuffer_ctor();

	struct TAC* t1 = makeTACNop();
	struct TAC* t2 = makeTACReturn(1);
	struct TAC* t3 = makeTACParam(2, true, 3, false);

	tacbuffer_append(buf, t1);
	tacbuffer_append(buf, t2);

	assert(tacbuffer_get(buf, 0) == t1);
	assert(tacbuffer_get(buf, 1) == t2);

	free(t3);
	tacbuffer_dtor(buf);
}

void test_tacbuffer_get_last() {

	status("tacbuffer_get_last");

	struct TACBuffer* buf = tacbuffer_ctor();

	struct TAC* t1 = makeTACNop();
	struct TAC* t2 = makeTACReturn(1);

	tacbuffer_append(buf, t1);
	tacbuffer_append(buf, t2);

	assert(tacbuffer_get_last(buf) == t2);

	tacbuffer_dtor(buf);
}

void test_tacbuffer_last_dest() {

	status("tacbuffer_last_dest");

	struct TACBuffer* buf = tacbuffer_ctor();

	struct TAC* t1 = makeTACConst(1, 0x3);
	struct TAC* t2 = makeTACCopy(20, 1);

	tacbuffer_append(buf, t1);
	tacbuffer_append(buf, t2);

	assert(tacbuffer_last_dest(buf) == 20);

	tacbuffer_dtor(buf);
}

void test_tacbuffer_count() {

	status("tacbuffer_count");

	struct TACBuffer* buf = tacbuffer_ctor();

	struct TAC* t1 = makeTACNop();
	struct TAC* t2 = makeTACReturn(1);

	tacbuffer_append(buf, t1);
	tacbuffer_append(buf, t2);
	tacbuffer_append(buf, t2);

	assert(tacbuffer_count(buf) == 3);

	tacbuffer_dtor(buf);
}
