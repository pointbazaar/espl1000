#pragma once

#include <stdbool.h>
#include <inttypes.h>

struct Flags;

void sd_print_help();

// @returns NULL in error case
struct Flags* makeFlags(int argc, char** argv);

// @returns NULL in error case
struct Flags* makeFlagsSingleFile(char* filename);

void freeFlags(struct Flags* flags);

int flags_count_filenames(struct Flags* flags);

// @returns NULL on error
char* flags_filenames(struct Flags* flags, int index);

bool flags_debug(struct Flags* flags);
bool flags_version(struct Flags* flags);
bool flags_help(struct Flags* flags);
bool flags_x86(struct Flags* flags);
bool flags_print_filenames(struct Flags* flags);
bool flags_lexer(struct Flags* flags);
bool flags_parser(struct Flags* flags);
bool flags_nolink(struct Flags* flags);
bool flags_rat(struct Flags* flags);
char* flags_output_filename(struct Flags* flags);
bool flags_dump_tokens(struct Flags* flags);
bool flags_dump_data(struct Flags* flags);
bool flags_stdlib(struct Flags* flags);

//generated filenames for later on
char* flags_asm_filename(struct Flags* ctx);
char* flags_token_filename(struct Flags* ctx);
char* flags_hex_filename(struct Flags* ctx);
char* flags_obj_filename(struct Flags* ctx);
