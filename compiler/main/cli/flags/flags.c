#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <libgen.h>
#include <malloc.h>
#include <regex.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "flags.h"
#include "all_flags.h"
#include "flag.h"

#include "validate_flags.h"

struct Flags {
	//struct Flags should be opaque outside its
	//implementation files

	bool has_main_fn;

	uint8_t count_filenames;
	uint8_t capacity_filenames;
	char** filenames;

	//generated filenames
	char* asm_filename;
	char* token_filename;
	char* hex_filename;
	char* obj_filename;

	struct Flag* all_flags;
};

// this should fall back to the default for that flag
// @returns flag arg. may be NULL
static char* flags_arg(struct Flags* flags, char* name) {

	for (size_t i = 0; i < all_flags_count(); i++) {
		struct Flag* f = &(flags->all_flags[i]);
		if (strcmp(f->name, name) != 0) {
			continue;
		}
		if (!f->has_arg) {
			return NULL;
		}
		if (!f->is_set) {
			return f->arg_default_value;
		}
		return f->arg;
	}
	return NULL;
}

static bool flags_set(struct Flags* flags, char* name) {

	for (size_t i = 0; i < all_flags_count(); i++) {
		struct Flag* f = &(flags->all_flags[i]);
		if (strcmp(f->name, name) == 0 && f->is_set) {
			return true;
		}
	}
	return false;
}

static void sd_print_flag(struct Flag* f) {

	char* arg_str = "";
	if (f->has_arg) {
		arg_str = "${ARG}";
	}
	printf(" -%-16s %6s %s\n", f->name, arg_str, f->description);

	if (f->arg_default_value) {
		printf("%25s DEFAULT VALUE: %s\n", "", f->arg_default_value);
	}
}

static void sd_print_flags() {

	for (size_t i = 0; i < all_flags_count(); i++) {
		sd_print_flag(&(all_flags[i]));
	}
}

void sd_print_help() {

	printf("OPTIONS\n");
	sd_print_flags();

	char* name = "sd";

	printf("\nEXAMPLES\n");
	printf("%s main.dg\n", name);
}

static char* filename_no_extension(char* filename) {

	char* fname_out = malloc(strlen(filename) + 5);
	if (!fname_out) {
		return NULL;
	}

	strcpy(fname_out, filename);
	//remove the '.dg'
	fname_out[strlen(fname_out) - 3] = '\0';

	return fname_out;
}

static char* make_asm_filename(char* filename) {

	char* fname_out = filename_no_extension(filename);
	strcat(fname_out, ".asm");
	return fname_out;
}

static char* make_obj_filename(char* filename) {

	char* fname_out = filename_no_extension(filename);
	strcat(fname_out, ".o");
	return fname_out;
}

static char* make_hex_filename(char* filename) {

	char* fname_out = filename_no_extension(filename);
	strcat(fname_out, ".hex");
	return fname_out;
}

static char* make_token_filename(char* filename) {

	char* token_filename = malloc(strlen(filename) + 20);

	if (!token_filename) {
		return NULL;
	}

	char* fnamecpy = malloc(strlen(filename) + 1);
	if (!fnamecpy) {
		free(token_filename);
		return NULL;
	}

	strcpy(fnamecpy, filename);
	char* base_name = basename(fnamecpy);

	char* fnamecpy2 = malloc(strlen(filename) + 1);
	if (!fnamecpy2) {
		free(token_filename);
		free(fnamecpy);
		return NULL;
	}
	strcpy(fnamecpy2, filename);
	char* dir_name = dirname(fnamecpy2);

	sprintf(token_filename, "%s/.%s.tokens", dir_name, base_name);

	free(fnamecpy);
	free(fnamecpy2);

	return token_filename;
}

// @returns number of args consumed
// @returns -1     in error case
static int make_flags_inner(struct Flags* flags, size_t i, size_t argc, char** argv) {

	char* arg = argv[i];

	if (arg[0] != '-') {

		flags->filenames[flags->count_filenames] = strdup(arg);
		flags->count_filenames += 1;
		return 1;
	}

	for (size_t j = 0; j < all_flags_count(); j++) {
		struct Flag* f = &(flags->all_flags[j]);

		if (strcmp(arg + 1, f->name) != 0) {
			continue;
		}

		f->is_set = true;
		if (!f->has_arg) {
			return 1;
		}
		if (i + 1 >= argc) {
			fprintf(stderr, "flag '-%s' expects argument, it was not provided\n", f->name);
			return -1;
		}
		f->arg = argv[i + 1];
		return 2;
	}

	fprintf(stderr, "unrecognized flag: '%s'\n", arg);
	return -1;
}

static void make_associated_filenames(struct Flags* flags) {

	flags->asm_filename = make_asm_filename(flags_filenames(flags, 0));
	flags->token_filename = make_token_filename(flags_filenames(flags, 0));
	flags->hex_filename = make_hex_filename(flags_filenames(flags, 0));
	flags->obj_filename = make_obj_filename(flags_filenames(flags, 0));
}

static bool flags_add_filename(struct Flags* flags, const char* path) {

	flags->capacity_filenames += 1;
	flags->filenames = realloc(flags->filenames, flags->capacity_filenames * sizeof(char*));

	if (!flags->filenames) {
		return false;
	}

	char* dup = strdup(path);

	if (!dup) {
		return false;
	}

	flags->filenames[flags->count_filenames++] = dup;

	return true;
}

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

static bool flags_add_stdlib_recursive(struct Flags* flags, const char* base_path) {

	DIR* dir = opendir(base_path);
	if (!dir) {
		perror("Failed to open stdlib directory");
		return false;
	}

	struct dirent* entry;
	while ((entry = readdir(dir)) != NULL) {

		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
			continue;
		}

		char* path = NULL;
		asprintf(&path, "%s/%s", base_path, entry->d_name);

		struct stat path_stat;
		if (stat(path, &path_stat) == 0) {

			char* name = entry->d_name;

			if (S_ISREG(path_stat.st_mode)) {

				char* ext = strrchr(name, '.');
				if (ext && strcmp(ext, ".dg") == 0) {
					if (flags_debug(flags)) {
						printf("Adding stdlib file: %s\n", path);
					}
					if (!flags_add_filename(flags, path)) {
						free(path);
						closedir(dir);
						return false;
					}
				}
			} else if (S_ISDIR(path_stat.st_mode)) {

				if (flags_debug(flags)) {
					printf("d_name: '%s'\n", name);
				}

				if ((strcmp(name, "avr") == 0) && flags_x86(flags)) {
					// do not include 'avr/' if we are compiling for x86
					continue;
				}

				if ((strcmp(name, "avr") != 0) && !flags_x86(flags)) {
					// only include 'avr/' if we are compiling for avr
					continue;
				}

				if (!flags_add_stdlib_recursive(flags, path)) {
					free(path);
					closedir(dir);
					return false;
				}
			}
		}

		free(path);
	}

	closedir(dir);
	return true;
}

// Public function
static bool flags_add_stdlib(struct Flags* flags) {
	char* base_path = (ESPL_STDLIB_PATH);
	return flags_add_stdlib_recursive(flags, base_path);
}

struct Flags* makeFlags(int argc, char** argv) {

	struct Flags* flags = malloc(sizeof(struct Flags));

	if (!flags) {
		return NULL;
	}

	flags->all_flags = malloc(sizeof(struct Flag) * all_flags_count());
	memcpy(flags->all_flags, all_flags, all_flags_count() * sizeof(struct Flag));

	flags->count_filenames = 0;

	flags->capacity_filenames = argc + 1;
	flags->filenames = malloc(sizeof(char*) * argc);

	if (!flags->filenames) {
		free(flags);
		return NULL;
	}

	int i = 1;
	while (i < argc) {
		int consumed = make_flags_inner(flags, i, argc, argv);
		if (consumed < 0) {
			return NULL;
		}

		i += consumed;
	}

	if (flags_stdlib(flags)) {
		if (!flags_add_stdlib(flags)) {
			return NULL;
		}
	}

	if (flags_set(flags, "help") || flags_set(flags, "version") || flags_set(flags, "rat")) {
		return flags;
	}

	bool success = validate_flags(flags);

	if (!success) {
		freeFlags(flags);
		return NULL;
	}

	make_associated_filenames(flags);

	return flags;
}

struct Flags* makeFlagsSingleFile(char* filename) {

	char* argv[] = {"program", filename};
	return makeFlags(2, argv);
}

void freeFlags(struct Flags* flags) {

	free(flags->all_flags);
	free(flags->asm_filename);
	free(flags->hex_filename);
	free(flags->token_filename);
	free(flags->obj_filename);
	for (size_t i = 0; i < flags->count_filenames; i++) {
		free(flags->filenames[i]);
	}
	free(flags->filenames);
	free(flags);
}

int flags_count_filenames(struct Flags* flags) {
	return flags->count_filenames;
}

char* flags_filenames(struct Flags* flags, int index) {

	if (index >= flags_count_filenames(flags)) {
		fprintf(stderr, "flags_filenames: tried to access out of bounds.");
		fflush(stdout);
		return NULL;
	}
	return flags->filenames[index];
}

bool flags_debug(struct Flags* flags) {
	return flags_set(flags, "debug");
}
bool flags_version(struct Flags* flags) {
	return flags_set(flags, "version");
}
bool flags_help(struct Flags* flags) {
	return flags_set(flags, "help");
}
bool flags_x86(struct Flags* flags) {
	return flags_set(flags, "x86");
}
bool flags_print_filenames(struct Flags* flags) {
	return flags_set(flags, "print-filenames");
}
bool flags_lexer(struct Flags* flags) {
	return flags_set(flags, "lexer");
}
bool flags_parser(struct Flags* flags) {
	return flags_set(flags, "parser");
}
bool flags_nolink(struct Flags* flags) {
	return flags_set(flags, "nolink");
}
bool flags_rat(struct Flags* flags) {
	return flags_set(flags, "rat");
}
char* flags_output_filename(struct Flags* flags) {

	return flags_arg(flags, "o");
}
bool flags_dump_tokens(struct Flags* flags) {
	return flags_set(flags, "dump-tokens");
}
bool flags_dump_data(struct Flags* flags) {
	return flags_set(flags, "dump-data");
}
bool flags_stdlib(struct Flags* flags) {
	return flags_set(flags, "stdlib");
}

char* flags_asm_filename(struct Flags* flags) {
	return flags->asm_filename;
}

char* flags_token_filename(struct Flags* flags) {
	return flags->token_filename;
}

char* flags_hex_filename(struct Flags* flags) {
	return flags->hex_filename;
}

char* flags_obj_filename(struct Flags* flags) {
	return flags->obj_filename;
}
