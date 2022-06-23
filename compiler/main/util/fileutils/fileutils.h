#ifndef FILEUTILS_H
#define FILEUTILS_H

#include "../../flags.h"

char* make_token_filename(char* filename);

char* make_h_filename(char* filename);
char* make_asm_filename(char* filename);

bool check_filenames_lowercase(struct Flags* flags);
#endif
