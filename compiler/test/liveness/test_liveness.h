#pragma once

void test_liveness_1_block();
void test_liveness_2_block();

void test_liveness_use_map_tac_call();
void test_liveness_use_map_tac_param();
void test_liveness_use_map_tac_return();
void test_liveness_use_map_tac_goto();
void test_liveness_use_map_tac_if_goto();
void test_liveness_use_map_tac_if_cmp_goto();
void test_liveness_use_map_tac_load_local_addr();
void test_liveness_use_map_tac_store_local();
void test_liveness_use_map_tac_load();
void test_liveness_use_map_tac_store();
void test_liveness_use_map_tac_binary_op();
void test_liveness_use_map_tac_unary_op();
void test_liveness_use_map_tac_copy();
void test_liveness_use_map_tac_const_value();
void test_liveness_use_map_tac_nop();
void test_liveness_use_map_tac_binary_op_immediate();

void test_liveness_def_map_tac_call();
void test_liveness_def_map_tac_param();
void test_liveness_def_map_tac_return();
void test_liveness_def_map_tac_goto();
void test_liveness_def_map_tac_if_goto();
void test_liveness_def_map_tac_if_cmp_goto();
void test_liveness_def_map_tac_load_local_addr();
void test_liveness_def_map_tac_store_local();
void test_liveness_def_map_tac_load();
void test_liveness_def_map_tac_store();
void test_liveness_def_map_tac_binary_op();
void test_liveness_def_map_tac_unary_op();
void test_liveness_def_map_tac_copy();
void test_liveness_def_map_tac_const_value();
void test_liveness_def_map_tac_nop();
void test_liveness_def_map_tac_binary_op_immediate();

void test_liveness_in_map();

void test_liveness_out_map_tac_call();
void test_liveness_out_map_tac_param();
void test_liveness_out_map_tac_return();
void test_liveness_out_map_tac_goto();
void test_liveness_out_map_tac_if_goto();
void test_liveness_out_map_tac_if_cmp_goto();
void test_liveness_out_map_tac_copy();
