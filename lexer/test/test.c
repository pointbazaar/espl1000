#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "test.h"
#include "../../token/token/token.h"
#include "../../token/TokenKeys.h"
#include "lexer_test_utils.h"

#include "util/status/status.h"

void printt(char* test_name) {
	status(test_name);
}
