#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <stdbool.h>
#include <malloc.h>
#include <regex.h>

#include "fileutils.h"

bool check_filenames_lowercase(struct Flags* flags) {

	char* filename_pattern = "^[a-z\\/._0-9]+$";
	regex_t regex;
	int status = regcomp(&regex, filename_pattern, REG_EXTENDED);

	if (status != 0) {
		perror("error:");
		return false;
	}

	for (int i = 0; i < flags_count_filenames(flags); ++i) {
		char* filename = flags_filenames(flags, i);
		//abc
		//find only the filename without the path
		char* filename_only = filename + strlen(filename) - 1;

		while (filename_only[0] != '/' && filename < filename_only) {
			filename_only--;
		}
		filename_only++;

		int result = regexec(&regex, filename_only, 0, NULL, 0);

		if (result != 0) {
			printf("[Error] filename %s did not match the required pattern for filenames\n", filename);
			return false;
		}
	}

	regfree(&regex);

	return true;
}
