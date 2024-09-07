#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "x86_code_gen/cg_x86.h"
#include "x86_code_gen/cg_x86_basic_block.h"

#include "tac/tacbuffer.h"

#include "test_x86_code_gen.h"
#include "test_x86_code_gen_util.h"

#include "compile_ir/test_compile_tac.h"

void status_test_x86_codegen(char* msg) {
	printf(" - [TEST] x86 codegen %s\n", msg);
}
