#pragma once

#include "libvmcu/libvmcu_system.h"
#include "libvmcu/libvmcu_analyzer.h"

#include "libvmcu_utils/libvmcu_utils.h"

void test_gen_tac_assignstmt();
void test_gen_tac_massignstmt();
void test_gen_tac_mdirect();
void test_gen_tac_expr();
void test_gen_tac_ifstmt();
void test_gen_tac_whilestmt();
void test_gen_tac_forstmt();
void test_gen_tac_simplevar();
void test_gen_tac_variable();
void test_gen_tac_call();
void test_gen_tac_structdecl();

void status_test_codegen_tac(char* msg);

vmcu_system_t* prepare_vmcu_system_from_code_snippet(char* code_snippet);
