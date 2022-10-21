#include "libvmcu/libvmcu_system.h"
#include "libvmcu/libvmcu_analyzer.h"

#ifndef TEST_GEN_TAC_H
#define TEST_GEN_TAC_H

void test_suite_tac_codegen();

void test_gen_tac_assignstmt();
void test_gen_tac_mdirect();

void status_test_codegen_tac(char* msg);


vmcu_system_t* prepare_vmcu_system_from_code_snippet(char* code_snippet);

#endif
