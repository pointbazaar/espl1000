#ifndef TESTOTHER
#define TESTOTHER

#include <stdbool.h>

#define TEST_COUNT_OTHER 8

bool test_statuscode(bool debug);
bool test_simplevar(bool debug);

bool test_ifstmt(bool debug);
bool test_whilestmt(bool debug);

bool test_subrcall(bool debug);
bool test_recursive(bool debug);

bool test_charconst_cmp(bool debug);

bool test_break(bool debug);

#endif
