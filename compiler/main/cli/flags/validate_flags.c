#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "flags.h"
#include "validate_flags.h"

static bool check_file_exists(char* filename) {

	//check if the file actually exists
	struct stat mystat;
	if (stat(filename, &mystat) == -1) {
		fprintf(stderr, "error in check_file_exists: filename: %s\n", filename);
		fflush(stdout);
		perror("Error: ");
		return false;
	}

	mode_t mode = mystat.st_mode;
	if (!S_ISREG(mode)) {
		//not a regular file
		fprintf(stderr, "Error: %s is not a regular file.\n", filename);
		return false;
	}

	return true;
}

static bool check_dg_extension(char* filename) {

	const int ext_index = strlen(filename) - 3;

	if (strcmp(filename + ext_index, ".dg") != 0) {
		fprintf(stderr, "filename has to have .dg extension\n");
		return false;
	}
	return true;
}

bool validate_flags(struct Flags* flags) {

	if (flags_count_filenames(flags) == 0) {

		printf("expected at least 1 filename\n");

		return false;
	}

	for (int i = 0; i < flags_count_filenames(flags); i++) {

		char* filename = flags_filenames(flags, i);

		check_file_exists(filename);

		if (!check_dg_extension(filename)) {
			return false;
		}
	}

	return true;
}
