#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <libgen.h>
#include <unistd.h>

#include "driver.h"
#include "lexer_flags.h"

#include "../../token/TokenKeys.h"

void out_nostr(int outFd, int id) {
	dprintf(outFd, "%d\n", id);
}

void out(int outFd, int id, char* str) {
	char* s = str;
	if (str == NULL) {
		s = "";
	}
	dprintf(outFd, "%d %s\n", id, s);
}
void out_length(int outFd, int id, char* str, int length) {
	char* s = str;
	if (str == NULL) {
		s = "";
	}
	dprintf(outFd, "%d %.*s\n", id, length, s);
}

void out2(int outFd, int id, int id2) {
	dprintf(outFd, "%d %d\n", id, id2);
}

void out_plus_plus(int outFd) {
	out(outFd, ASSIGNOP_PLUS, "+=");
	out(outFd, INTEGER, "1");
}

void out_minus_minus(int outFd) {
	out(outFd, ASSIGNOP_MINUS, "-=");
	out(outFd, INTEGER, "1");
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
