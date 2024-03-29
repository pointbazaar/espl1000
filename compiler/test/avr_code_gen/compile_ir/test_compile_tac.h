#ifndef TEST_COMPILE_TAC_H
#define TEST_COMPILE_TAC_H

#include "libvmcu_utils/libvmcu_utils.h"

void test_compile_tac_nop();
void test_compile_tac_const_value();

void test_compile_tac_store_const_addr();
void test_compile_tac_load_const_addr();

void test_compile_tac_binary_op_immediate();
void test_compile_tac_unary_op();
void test_compile_tac_binary_op();

void test_compile_tac_goto();
void test_compile_tac_if_goto();
void test_compile_tac_if_cmp_goto();
void test_compile_tac_return();

void test_compile_tac_copy();
void test_compile_tac_param();
void test_compile_tac_call();

void test_compile_tac_load();
void test_compile_tac_store();

void test_compile_tac_setup_sp();
void test_compile_tac_setup_stackframe();

#endif
