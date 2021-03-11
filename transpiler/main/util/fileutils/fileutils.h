#ifndef FILEUTILS_H
#define FILEUTILS_H

#include "../../flags/flags.h"

char* make_ast_filename(char* filename);

char* make_c_filename(char* filename);

char* make_gcc_cmd(struct Flags* flags, char* fname_out);


#endif
