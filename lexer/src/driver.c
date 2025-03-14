#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <libgen.h>
#include <unistd.h>

#include "driver.h"
#include "lexer_flags.h"
#include "lexer.h"

#include "token/TokenKeys.h"
#include "token/list/TokenList.h"
#include "token/token/token.h"

void out_nostr(struct TokenList* list, int id) {
	list_add(list, makeTokenLineNo(id, line_no));
}

void out(struct TokenList* list, int id, char* str) {
	char* s = str;
	if (str == NULL) {
		list_add(list, makeTokenLineNo(id, line_no));
	} else {
		list_add(list, makeToken2LineNo(id, s, line_no));
	}
}

void out_length(struct TokenList* list, int id, char* str, int length) {
	char* s = str;
	if (str == NULL) {
		s = "";
	}
	char* limited = calloc(length + 1, 1);
	memcpy(limited, s, length);

	list_add(list, makeToken2LineNo(id, limited, line_no));

	free(limited);
}

void out_plus_plus(struct TokenList* list) {
	out(list, ASSIGNOP_PLUS, "+=");
	out(list, INTEGER, "1");
}

void out_minus_minus(struct TokenList* list) {
	out(list, ASSIGNOP_MINUS, "-=");
	out(list, INTEGER, "1");
}

char* lexer_make_tkn_filename(char* filename) {

	//because basename,dirname may modify their args
	char* cpy_filename_1 = malloc(strlen(filename) + 1);
	char* cpy_filename_2 = malloc(strlen(filename) + 1);
	strcpy(cpy_filename_1, filename);
	strcpy(cpy_filename_2, filename);

	char* dir = dirname(cpy_filename_1);

	char* tkn_filename = malloc(strlen(filename) + 1 + 100);

	tkn_filename[0] = '\0';
	strcat(tkn_filename, dir);
	strcat(tkn_filename, "/.");
	strcat(tkn_filename, basename(cpy_filename_2));
	strcat(tkn_filename, ".tokens");

	free(cpy_filename_1);
	free(cpy_filename_2);

	return tkn_filename;
}
