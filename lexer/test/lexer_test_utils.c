#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <unistd.h>

#include "lexer_test_utils.h"

#include "../../token/reader/token_reader.h"
#include "../../token/token/token.h"
#include "../../token/list/TokenList.h"

#include "../src/lexer_main.h"
#include "lexer/src/lexer_flags.h"

struct Token** lex(char* source, size_t* out_count) {

	//make a file with the source
	//run the lexer
	//read out the tokens

	char* fname_src = "/tmp/temporary-smalldragon.dg";

	{
		FILE* f = fopen(fname_src, "w");

		if (f == NULL) {
			fprintf(stderr, "Lexer: could not write to %s\n", fname_src);
			return NULL;
		}

		fprintf(f, "%s", source);
		fclose(f);
	}

	struct LexerFlags flags;
	flags.filename = fname_src;
	flags.write_token_file = false;

	int fd = lexer_main(&flags);

	if (fd < 0) {
		fprintf(stderr, "error lexing %s\n", fname_src);
		return NULL;
	}

	struct TokenList* list = read_tokens_from_tokens_file(fd, fname_src);

	if (list == NULL) {
		fprintf(stderr, "Lexer: could not read from fd %d\n", fd);
		close(fd);
		return NULL;
	}

	uint32_t capacity = list_size(list);
	*out_count = list_size(list);
	struct Token** tks = malloc(sizeof(struct Token*) * capacity);

	for (uint32_t count = 0; count < capacity; count++) {
		tks[count] = list_get(list, count);
	}

	freeTokenListShallow(list);

	return tks;
}

void free_tokens(struct Token** tokens, unsigned int count) {
	for (uint32_t i = 0; i < count; i++) {
		freeToken(tokens[i]);
	}
	free(tokens);
}
