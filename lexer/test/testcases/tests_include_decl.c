#include <assert.h>
#include <string.h>

#include "../test.h"
#include "../token/token/token.h"
#include "../token/TokenKeys.h"
#include "../lexer_test_utils.h"

void test_include_decl() {

	printt("test #include <file.h>  \n");

	char* str = "#include <file.h>";
	struct Token** tokens = lex(str);

	assert(tokens != NULL);

	assert(tokens[0]->kind == INCLUDE_DECL);
	assert(strcmp(tokens[0]->value_ptr, "file.h") == 0);

	free_tokens(tokens, 1);
}
