#ifndef FLAGS
#define FLAGS

#include <stdbool.h>
#include <inttypes.h>

struct Flags;

void sd_print_help();

struct Flags* makeFlags(int argc, char** argv);
struct Flags* makeFlagsSingleFile(char* filename);

void freeFlags(struct Flags* flags);

int flags_count_filenames(struct Flags* flags);
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

//generated filenames for later on
char* flags_asm_filename(struct Flags* ctx);
char* flags_token_filename(struct Flags* ctx);
char* flags_hex_filename(struct Flags* ctx);
char* flags_obj_filename(struct Flags* ctx);
#endif
