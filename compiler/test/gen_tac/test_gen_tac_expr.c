#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "test_gen_tac.h"

void test_gen_tac_expr_plus() {

	status_test_codegen_tac("Expr +");

	for (int8_t value1 = 0; value1 < 3; value1++) {
		for (int8_t value2 = 0; value2 < 3; value2++) {

			char snippet[200];
			sprintf(snippet, "fn main() -> int { int x = %d + %d; return x; }", value1, value2);

			//compile snippet and init a vmcu
			vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

			//step it past the main function
			vmcu_system_step_n(system, 30);

			//assert that value is returned in r0 as it should be

			int8_t r0_value = vmcu_system_read_gpr(system, 0);

			assert(r0_value == (value1 + value2));

			vmcu_system_dtor(system);
		}
	}
}

void test_gen_tac_expr_minus() {

	status_test_codegen_tac("Expr -");

	for (int8_t value1 = 50; value1 < 53; value1++) {
		for (int8_t value2 = 0; value2 < 3; value2++) {
			const int8_t expected = value1 - value2;

			char snippet[200];
			sprintf(snippet, "fn main() -> int8 { int8 x = %d - %d; return x; }", value1, value2);

			//compile snippet and init a vmcu
			vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

			//step it past the main function
			vmcu_system_step_n(system, 30);

			//assert that value is returned in r0 as it should be
			int8_t r0 = vmcu_system_read_gpr(system, 0);

			assert(r0 == expected);

			vmcu_system_dtor(system);
		}
	}
}

void test_gen_tac_expr_mul() {

	status_test_codegen_tac("Expr * (SKIPPING, TODO: re-enable)");

	return;

	for (int8_t value1 = 0; value1 < 3; value1++) {
		for (int8_t value2 = 0; value2 < 3; value2++) {

			char snippet[200];
			sprintf(snippet, "fn main() -> int { int x = %d * %d; return x; }", value1, value2);

			//compile snippet and init a vmcu
			vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

			//step it past the main function
			vmcu_system_step_n(system, 20);

			//assert that value is returned in r0 as it should be
			int8_t r0_value = vmcu_system_read_gpr(system, 0);

			assert(r0_value == (value1 * value2));

			vmcu_system_dtor(system);
		}
	}
}

void test_gen_tac_expr_and() {

	status_test_codegen_tac("Expr &");

	for (int8_t value1 = 0; value1 < 3; value1++) {
		for (int8_t value2 = 0; value2 < 3; value2++) {

			char snippet[200];
			sprintf(snippet, "fn main() -> int { int x = %d & %d; return x; }", value1, value2);

			//compile snippet and init a vmcu
			vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

			//step it past the main function
			vmcu_system_step_n(system, 30);

			//assert that value is returned in r0 as it should be

			int8_t r0_value = vmcu_system_read_gpr(system, 0);

			assert(r0_value == (value1 & value2));

			vmcu_system_dtor(system);
		}
	}
}

void test_gen_tac_expr_or() {

	status_test_codegen_tac("Expr |");

	for (int8_t value1 = 0; value1 < 3; value1++) {
		for (int8_t value2 = 0; value2 < 3; value2++) {

			char snippet[200];
			sprintf(snippet, "fn main() -> int { int x = %d | %d; return x; }", value1, value2);

			vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

			vmcu_system_step_n(system, 28);

			int8_t r0_value = vmcu_system_read_gpr(system, 0);

			assert(r0_value == (value1 | value2));

			vmcu_system_dtor(system);
		}
	}
}

void test_gen_tac_expr_shift_left() {

	status_test_codegen_tac("Expr <<");

	for (int8_t value1 = 0; value1 < 3; value1++) {
		for (int8_t value2 = 1; value2 < 3; value2++) {

			char snippet[200];
			sprintf(snippet, "fn main() -> int { int x = %d << %d; return x; }", value1, value2);

			vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

			vmcu_system_step_n(system, 28);

			const int8_t r0_value = vmcu_system_read_gpr(system, 0);
			const int8_t expect = value1 << value2;

			assert(r0_value == expect);

			vmcu_system_dtor(system);
		}
	}
}

void test_gen_tac_expr_shift_right() {

	status_test_codegen_tac("Expr >>");

	for (int8_t value1 = 0; value1 < 3; value1++) {
		for (int8_t value2 = 1; value2 < 3; value2++) {

			char snippet[200];
			sprintf(snippet, "fn main() -> int { int x = %d >> %d; return x; }", value1, value2);

			vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

			vmcu_system_step_n(system, 28);

			const int8_t r0_value = vmcu_system_read_gpr(system, 0);
			const int8_t expect = value1 >> value2;

			assert(r0_value == expect);

			vmcu_system_dtor(system);
		}
	}
}

void test_gen_tac_expr_xor() {

	status_test_codegen_tac("Expr ^");

	for (int8_t value1 = 0; value1 < 3; value1++) {
		for (int8_t value2 = 0; value2 < 3; value2++) {

			char snippet[200];
			sprintf(snippet, "fn main() -> int { int x = %d ^ %d; return x; }", value1, value2);

			//compile snippet and init a vmcu
			vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

			//step it past the main function
			vmcu_system_step_n(system, 28);

			//assert that value is returned in r0 as it should be

			const int8_t r0_value = vmcu_system_read_gpr(system, 0);
			const int8_t expect = value1 ^ value2;

			assert(r0_value == expect);

			vmcu_system_dtor(system);
		}
	}
}

void test_gen_tac_expr_lt_8bit() {

	status_test_codegen_tac("Expr < (8 bit)");

	for (uint8_t value1 = 0; value1 < 3; value1++) {
		for (uint8_t value2 = 0; value2 < 3; value2++) {

			char snippet[200];
			sprintf(snippet, "fn main() -> bool { bool x = %d < %d; return x; }", value1, value2);

			vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

			vmcu_system_step_n(system, 28);

			const int8_t r0_value = vmcu_system_read_gpr(system, 0);
			const int8_t expect = value1 < value2;

			assert(r0_value == expect);

			vmcu_system_dtor(system);
		}
	}
}

void test_gen_tac_expr_lt_16bit() {

	status_test_codegen_tac("Expr < (16 bit)");

	for (uint16_t value1 = 0x100; value1 < 0x103; value1++) {
		for (uint16_t value2 = 0x100; value2 < 0x103; value2++) {

			char snippet[200];
			sprintf(snippet, "fn main() -> bool { bool x = %d < %d; return x; }", value1, value2);

			vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

			vmcu_system_step_n(system, 28);

			const int8_t r0_value = vmcu_system_read_gpr(system, 0);
			const int8_t expect = value1 < value2;

			assert(r0_value == expect);

			vmcu_system_dtor(system);
		}
	}
}

void test_gen_tac_expr_gt_8bit() {

	status_test_codegen_tac("Expr > (8 bit)");

	for (uint8_t value1 = 0; value1 < 3; value1++) {
		for (uint8_t value2 = 0; value2 < 3; value2++) {

			char snippet[200];
			sprintf(snippet, "fn main() -> bool { bool x = %d > %d; return x; }", value1, value2);

			vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

			vmcu_system_step_n(system, 28);

			const uint8_t r0_value = vmcu_system_read_gpr(system, 0);
			const uint8_t expect = value1 > value2;

			assert(r0_value == expect);

			vmcu_system_dtor(system);
		}
	}
}

void test_gen_tac_expr_gt_16bit() {

	status_test_codegen_tac("Expr > (16 bit)");

	for (uint16_t value1 = 0x100; value1 < 0x103; value1++) {
		for (uint16_t value2 = 0x100; value2 < 0x103; value2++) {

			char snippet[200];
			sprintf(snippet, "fn main() -> bool { bool x = %d > %d; return x; }", value1, value2);

			vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

			vmcu_system_step_n(system, 28);

			const uint8_t r0_value = vmcu_system_read_gpr(system, 0);
			const uint8_t expect = value1 > value2;

			assert(r0_value == expect);

			vmcu_system_dtor(system);
		}
	}
}

void test_gen_tac_expr_eq_8bit() {

	status_test_codegen_tac("Expr == (8 bit)");

	for (uint8_t value1 = 0; value1 < 3; value1++) {
		for (uint8_t value2 = 0; value2 < 3; value2++) {

			char snippet[200];
			sprintf(snippet, "fn main() -> bool { bool x = %d == %d; return x; }", value1, value2);

			vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

			vmcu_system_step_n(system, 28);

			const uint8_t r0_value = vmcu_system_read_gpr(system, 0);
			const uint8_t expect = value1 == value2;

			assert(r0_value == expect);

			vmcu_system_dtor(system);
		}
	}
}

void test_gen_tac_expr_eq_16bit() {

	status_test_codegen_tac("Expr == (16 bit)");

	for (uint16_t value1 = 0x100; value1 < 0x103; value1++) {
		for (uint16_t value2 = 0x100; value2 < 0x103; value2++) {

			char snippet[200];
			sprintf(snippet, "fn main() -> bool { bool x = %d == %d; return x; }", value1, value2);

			vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

			vmcu_system_step_n(system, 28);

			const uint8_t r0_value = vmcu_system_read_gpr(system, 0);
			const uint8_t expect = value1 == value2;

			assert(r0_value == expect);

			vmcu_system_dtor(system);
		}
	}
}

void test_gen_tac_expr_neq_8bit() {

	status_test_codegen_tac("Expr != (8 bit)");

	for (uint8_t value1 = 0; value1 < 3; value1++) {
		for (uint8_t value2 = 0; value2 < 3; value2++) {

			char snippet[200];
			sprintf(snippet, "fn main() -> bool { bool x = %d != %d; return x; }", value1, value2);

			vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

			vmcu_system_step_n(system, 28);

			const uint8_t r0_value = vmcu_system_read_gpr(system, 0);

			if (value1 != value2)
				assert(r0_value != 0);
			else
				assert(r0_value == 0);

			vmcu_system_dtor(system);
		}
	}
}

void test_gen_tac_expr_neq_16bit() {

	status_test_codegen_tac("Expr != (16 bit)");

	for (uint16_t value1 = 0x100; value1 < 0x103; value1++) {
		for (uint16_t value2 = 0x100; value2 < 0x103; value2++) {

			char snippet[200];
			sprintf(snippet, "fn main() -> bool { bool x = %d != %d; return x; }", value1, value2);

			vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

			vmcu_system_step_n(system, 28);

			const uint8_t r0_value = vmcu_system_read_gpr(system, 0);

			if (value1 != value2)
				assert(r0_value != 0);
			else
				assert(r0_value == 0);

			vmcu_system_dtor(system);
		}
	}
}
