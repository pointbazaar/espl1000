#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

#include "lexer_test_utils.h"

#include "../../token/reader/token_reader.h"
#include "../../token/list/TokenList.h"

struct Token** lex(char* source) {

	//printf("lex(%s, ...)\n", source);

	//make a file with the source
	//run the lexer
	//read out the tokens

	char* fname_src = "/tmp/temporary-smalldragon.dg";
	char* fname_tks = "/tmp/.temporary-smalldragon.dg.tokens";

	//make a file with the source
	FILE* f = fopen(fname_src, "w");
	fprintf(f, "%s",source);
	fclose(f);


	//run the lexer
	char cmd[100];
	sprintf(cmd, "./dragon-lexer %s", fname_src);
	system(cmd);


	//read out the tokens
	FILE* ftks = fopen(fname_tks, "r");

	struct TokenList* list = read_tokens_from_tokens_file(ftks, fname_tks);

	int capacity = list_size(list);
	struct Token** tks = malloc(sizeof(struct Token*)*capacity);

	for (uint32_t count = 0; count < capacity ; count++) {
		tks[count] = list_get(list, count);
	}

	fclose(ftks);

	return tks;
}

void free_tokens(struct Token** tokens, unsigned int count) {
	for(int i = 0; i < count; i++) {
		//free(tokens[i]->value);
		free(tokens[i]);
	}
}