#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

#include "lexer_test_utils.h"

#include "../../token/reader/token_reader.h"
#include "../../token/token/token.h"
#include "../../token/list/TokenList.h"

#include "../src/lexer_main.h"
#include "lexer/src/lexer_flags.h"

struct Token** lex(char* source) {

	//make a file with the source
	//run the lexer
	//read out the tokens

	char* fname_src = "/tmp/temporary-smalldragon.dg";
	char* fname_tks = "/tmp/.temporary-smalldragon.dg.tokens";

	//make a file with the source
	FILE* f = fopen(fname_src, "w");
	fprintf(f, "%s", source);
	fclose(f);

	struct LexerFlags flags;
	flags.filename = fname_src;

	if (lexer_main(&flags) != 0) {
		fprintf(stderr, "error lexing %s\n", fname_src);
		return NULL;
	}

	FILE* ftks = fopen(fname_tks, "r");

	struct TokenList* list = read_tokens_from_tokens_file(ftks, fname_tks);

	uint32_t capacity = list_size(list);
	struct Token** tks = malloc(sizeof(struct Token*) * capacity);

	for (uint32_t count = 0; count < capacity; count++) {
		tks[count] = list_get(list, count);
	}

	freeTokenListShallow(list);

	fclose(ftks);

	return tks;
}

void free_tokens(struct Token** tokens, unsigned int count) {
	for (uint32_t i = 0; i < count; i++) {
		freeToken(tokens[i]);
	}
	free(tokens);
}
