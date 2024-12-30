#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "tables/lvst/lvst.h"

#include "test.h"
#include "util/status/status.h"

static void test_status_lvst(char* msg) {

	char m[100];
	sprintf(m, "LVST %s", msg);
	status(m);
}

void test_tables_lvst_ctor() {

	test_status_lvst("ctor");

	struct LVST* lvst = lvst_ctor();

	lvst_free(lvst);
}

void test_tables_lvst_sizeof_type_primitive() {

	test_status_lvst("lvst_sizeof_type (primitive)");

	struct Type type;
	struct BasicType bt;
	struct SimpleType st;
	struct PrimitiveType pt;
	struct StructType stt;
	struct SubrType subrt;

	pt.is_int_type = true;
	pt.is_char_type = false;
	pt.is_bool_type = false;

	st.struct_type = NULL;
	st.primitive_type = &pt;

	bt.subr_type = NULL;
	bt.simple_type = &st;

	type.basic_type = &bt;
	type.type_param = NULL;
	type.array_type = NULL;

	pt.int_type = INT;
	assert(lvst_sizeof_type(&type, false) == 1);
	assert(lvst_sizeof_type(&type, true) == 4);

	pt.int_type = UINT8;
	assert(lvst_sizeof_type(&type, false) == 1);
	assert(lvst_sizeof_type(&type, true) == 1);

	pt.int_type = INT16;
	assert(lvst_sizeof_type(&type, false) == 2);
	assert(lvst_sizeof_type(&type, true) == 2);

	pt.int_type = INT32;
	assert(lvst_sizeof_type(&type, false) == 4);
	assert(lvst_sizeof_type(&type, true) == 4);

	pt.int_type = UINT64;
	assert(lvst_sizeof_type(&type, false) == 8);
	assert(lvst_sizeof_type(&type, true) == 8);
}

void test_tables_lvst_sizeof_type_other() {

	test_status_lvst("lvst_sizeof_type (other)");

	struct Type type;
	struct BasicType bt;
	struct SimpleType st;
	struct PrimitiveType pt;
	struct StructType stt;
	struct SubrType subrt;
	struct ArrayType at;

	pt.is_int_type = true;
	pt.is_char_type = false;
	pt.is_bool_type = false;

	st.struct_type = NULL;
	st.primitive_type = &pt;

	bt.subr_type = NULL;
	bt.simple_type = &st;

	type.basic_type = &bt;
	type.type_param = NULL;
	type.array_type = NULL;

	st.struct_type = &stt;
	st.primitive_type = NULL;

	assert(lvst_sizeof_type(&type, false) == 2);
	assert(lvst_sizeof_type(&type, true) == 8);

	bt.simple_type = NULL;
	bt.subr_type = &subrt;
	assert(lvst_sizeof_type(&type, false) == 2);
	assert(lvst_sizeof_type(&type, true) == 8);

	type.basic_type = NULL;
	type.type_param = NULL;
	type.array_type = &at;
	assert(lvst_sizeof_type(&type, false) == 2);
	assert(lvst_sizeof_type(&type, true) == 8);
}

void test_tables_lvst_stack_frame_size_avr() {

	test_status_lvst("lvst_stack_frame_size (avr)");

	struct Type type;
	struct BasicType bt;
	struct SimpleType st;
	struct PrimitiveType pt;

	pt.is_int_type = true;
	pt.is_char_type = false;
	pt.is_bool_type = false;

	st.struct_type = NULL;
	st.primitive_type = &pt;

	bt.subr_type = NULL;
	bt.simple_type = &st;

	type.basic_type = &bt;
	type.type_param = NULL;
	type.array_type = NULL;

	pt.int_type = INT;

	struct LVST* lvst = lvst_ctor();

	assert(lvst_stack_frame_size_x86(lvst) == 0);
	assert(lvst_stack_frame_size_avr(lvst) == 0);

	struct LVSTLine* line1 = lvst_line_ctor("x1", &type, false);
	struct LVSTLine* line2 = lvst_line_ctor("x2", &type, false);

	lvst_add(lvst, line1);

	assert(lvst_stack_frame_size_avr(lvst) == 1);

	lvst_add(lvst, line2);

	assert(lvst_stack_frame_size_avr(lvst) == 2);

	line1->is_arg = true;

	assert(lvst_stack_frame_size_avr(lvst) == 1);

	line2->is_arg = true;

	assert(lvst_stack_frame_size_avr(lvst) == 0);

	line1->is_arg = false;
	line2->is_arg = false;
	pt.int_type = INT64;

	assert(lvst_stack_frame_size_avr(lvst) == 16);

	lvst_free(lvst);
}

void test_tables_lvst_stack_frame_size_x86() {

	test_status_lvst("lvst_stack_frame_size (x86)");

	struct Type type;
	struct BasicType bt;
	struct SimpleType st;
	struct PrimitiveType pt;

	pt.is_int_type = true;
	pt.is_char_type = false;
	pt.is_bool_type = false;

	st.struct_type = NULL;
	st.primitive_type = &pt;

	bt.subr_type = NULL;
	bt.simple_type = &st;

	type.basic_type = &bt;
	type.type_param = NULL;
	type.array_type = NULL;

	pt.int_type = INT;

	struct LVST* lvst = lvst_ctor();

	assert(lvst_stack_frame_size_x86(lvst) == 0);
	assert(lvst_stack_frame_size_avr(lvst) == 0);

	struct LVSTLine* line = lvst_line_ctor("x", &type, false);

	lvst_add(lvst, line);

	assert(lvst_stack_frame_size_x86(lvst) == 4);

	line->is_arg = true;

	assert(lvst_stack_frame_size_x86(lvst) == 0);

	lvst_free(lvst);
}

void test_tables_lvst_stack_frame_offset_avr_int8() {

	test_status_lvst("lvst_stack_frame_offset avr (int8)");

	struct Type type;
	struct BasicType bt;
	struct SimpleType st;
	struct PrimitiveType pt;

	pt.is_int_type = true;
	pt.is_char_type = false;
	pt.is_bool_type = false;

	st.struct_type = NULL;
	st.primitive_type = &pt;

	bt.subr_type = NULL;
	bt.simple_type = &st;

	type.basic_type = &bt;
	type.type_param = NULL;
	type.array_type = NULL;

	pt.int_type = INT8;

	struct LVST* lvst = lvst_ctor();

	struct LVSTLine* line = lvst_line_ctor("x", &type, false);

	lvst_add(lvst, line);

	assert(lvst_stack_frame_offset_avr(lvst, "x") == 1);

	line->is_arg = true;

	assert(lvst_stack_frame_offset_avr(lvst, "x") == 3);

	lvst_free(lvst);
}

void test_tables_lvst_stack_frame_offset_x86_int8() {

	test_status_lvst("lvst_stack_frame_offset x86 (int8)");

	struct Type type;
	struct BasicType bt;
	struct SimpleType st;
	struct PrimitiveType pt;

	pt.is_int_type = true;
	pt.is_char_type = false;
	pt.is_bool_type = false;

	st.struct_type = NULL;
	st.primitive_type = &pt;

	bt.subr_type = NULL;
	bt.simple_type = &st;

	type.basic_type = &bt;
	type.type_param = NULL;
	type.array_type = NULL;

	pt.int_type = INT8;

	struct LVST* lvst = lvst_ctor();

	struct LVSTLine* line = lvst_line_ctor("x", &type, false);

	lvst_add(lvst, line);

	assert(lvst_stack_frame_offset_x86(lvst, "x") == 1);

	lvst_free(lvst);
}

void test_tables_lvst_stack_frame_offset_avr_int64() {

	test_status_lvst("lvst_stack_frame_offset avr (int64)");

	struct Type type;
	struct BasicType bt;
	struct SimpleType st;
	struct PrimitiveType pt;

	pt.is_int_type = true;
	pt.is_char_type = false;
	pt.is_bool_type = false;

	st.struct_type = NULL;
	st.primitive_type = &pt;

	bt.subr_type = NULL;
	bt.simple_type = &st;

	type.basic_type = &bt;
	type.type_param = NULL;
	type.array_type = NULL;

	pt.int_type = INT64;

	struct LVST* lvst = lvst_ctor();

	struct LVSTLine* line1 = lvst_line_ctor("x1", &type, false);
	struct LVSTLine* line2 = lvst_line_ctor("x2", &type, false);

	lvst_add(lvst, line1);
	lvst_add(lvst, line2);

	assert(lvst_stack_frame_offset_avr(lvst, "x1") == 1);
	assert(lvst_stack_frame_offset_avr(lvst, "x2") == 9);

	line1->is_arg = true;

	assert(lvst_stack_frame_offset_avr(lvst, "x1") == 11);
	assert(lvst_stack_frame_offset_avr(lvst, "x2") == 1);

	lvst_free(lvst);
}

void test_tables_lvst_stack_frame_offset_x86_int64() {

	test_status_lvst("lvst_stack_frame_offset x86 (int64)");

	struct Type type;
	struct BasicType bt;
	struct SimpleType st;
	struct PrimitiveType pt;

	pt.is_int_type = true;
	pt.is_char_type = false;
	pt.is_bool_type = false;

	st.struct_type = NULL;
	st.primitive_type = &pt;

	bt.subr_type = NULL;
	bt.simple_type = &st;

	type.basic_type = &bt;
	type.type_param = NULL;
	type.array_type = NULL;

	pt.int_type = INT64;

	struct LVST* lvst = lvst_ctor();

	struct LVSTLine* line1 = lvst_line_ctor("x1", &type, false);
	struct LVSTLine* line2 = lvst_line_ctor("x2", &type, false);

	lvst_add(lvst, line1);
	lvst_add(lvst, line2);

	assert(lvst_stack_frame_offset_x86(lvst, "x1") == 8);
	assert(lvst_stack_frame_offset_x86(lvst, "x2") == 16);

	line1->is_arg = true;

	lvst_free(lvst);
}

void test_tables_lvst_arg_index() {

	test_status_lvst("lvst_arg_index");

	struct Type type;
	struct BasicType bt;
	struct SimpleType st;
	struct PrimitiveType pt;

	pt.is_int_type = true;
	pt.is_char_type = false;
	pt.is_bool_type = false;

	st.struct_type = NULL;
	st.primitive_type = &pt;

	bt.subr_type = NULL;
	bt.simple_type = &st;

	type.basic_type = &bt;
	type.type_param = NULL;
	type.array_type = NULL;

	pt.int_type = INT64;

	struct LVST* lvst = lvst_ctor();

	struct LVSTLine* line1 = lvst_line_ctor("x1", &type, true);
	struct LVSTLine* line2 = lvst_line_ctor("x2", &type, true);

	lvst_add(lvst, line1);
	lvst_add(lvst, line2);

	assert(lvst_arg_index(lvst, "x1") == 0);
	assert(lvst_arg_index(lvst, "x2") == 1);

	lvst_free(lvst);
}
