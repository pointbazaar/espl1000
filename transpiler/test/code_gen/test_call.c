#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "test_call.h"

#include "util/test_statuscode.h"

static void status_test_assign(char* msg){
    printf(" - [TEST] %s\n", msg);
}

void test_call_simple(){

    status_test_assign("test call simple");

    char* src = "fn main () ~> int { return sub(); } fn sub()->int { return 39; }";
    const int status = sourceToStatus(src, false);

    assert(status == 39);
}

void test_call_struct_member_access(){

    status_test_assign("test call struct member access");

    char* src = "#include <stdlib.h>\n externc fn malloc(int n)->A \"malloc\" struct A { (()->int) x; }  fn main () ~> int { A b = malloc(8); b.x = sub; return b.x(); } fn sub()->int { return 13; }";
    const int status = sourceToStatus(src, false);

    assert(status == 13);
}

void test_call_array_access(){

    status_test_assign("test call array access");

    char* src = "#include <stdlib.h>\n externc fn malloc()->[(()->int)] \"malloc\" fn main () ~> int { [(()->int)] b = malloc(8); b[0] = sub; return b[0](); } fn sub()->int { return 23; }";
    const int status = sourceToStatus(src, false);

    assert(status == 23);
}