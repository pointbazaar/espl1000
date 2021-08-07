#ifndef FILEUTILS_H
#define FILEUTILS_H

#include "../../flags.h"

char* make_ast_filename(char* filename);

char* make_c_filename(char* filename);
char* make_h_filename(char* filename);

char* make_gcc_cmd(struct Flags* flags, char* fname_out);

bool check_filenames_lowercase(struct Flags* flags);
#endif
