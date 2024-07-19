#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "flags.h"
#include "validate_flags.h"

static bool check_dg_extension(char* filename);

static void check_file_exists(char* filename);

void validate_flags(struct Flags* flags) {

	if (flags_count_filenames(flags) == 0) {

		printf("expected at least 1 filename\n");

		freeFlags(flags);
		exit(1);
	}

	for (int i = 0; i < flags_count_filenames(flags); i++) {

		char* filename = flags_filenames(flags, i);

		check_file_exists(filename);

		if (!check_dg_extension(filename)) {
			freeFlags(flags);
			exit(1);
		}
	}
}

static bool check_dg_extension(char* filename) {

	const int ext_index = strlen(filename) - 3;

	if (strcmp(filename + ext_index, ".dg") != 0) {
		printf("filename has to have .dg extension\n");
		return false;
	}
	return true;
}

static void check_file_exists(char* filename) {

	//check if the file actually exists
	struct stat mystat;
	if (stat(filename, &mystat) == -1) {
		printf("error in check_file_exists");
		fflush(stdout);
		perror("Error: ");
		//freeFlags(flags);
		exit(1);
	}

	mode_t mode = mystat.st_mode;
	if (!S_ISREG(mode)) {
		//not a regular file
		printf("Error: %s is not a regular file.\n", filename);
		//freeFlags(flags);
		exit(1);
	}
}
