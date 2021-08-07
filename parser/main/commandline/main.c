#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#include "parser_flags.h"
#include "parser_help.h"
#include "parser.h"

#define ERR_EXPECT_1_FILENAME   "[Parser] expected exactly 1 filename argument.\n"
#define ERR_FILE_NOT_EXIST      "[Parser] file %s does not exist.\n"
#define ERR_WRONG_EXTENSION     "[Parser] '%s' does not have .tokens extension. Actual extension: '%s' - Exiting."

int main(int argc, char** argv){

    mallopt(M_CHECK_ACTION, 3);

    struct ParserFlags* flags = parseFlags(argc, argv);

    if(flags->help) {
        printHelp();
        free(flags);
        return 0;
    }

    if(flags->filename == NULL){
        printf(ERR_EXPECT_1_FILENAME);
        free(flags);
        return 1;
    }

    char* extension = strrchr(flags->filename, '.');

    if(extension == NULL){
        printf("Fatal\n");
        free(flags);
        return -1;
    }

    if(strcmp(extension, ".tokens") != 0){
        printf(ERR_WRONG_EXTENSION, flags->filename, extension);
        free(flags);
        return -1;
    }

    FILE* f = fopen(flags->filename, "r");
    if(f == NULL){
        printf(ERR_FILE_NOT_EXIST, flags->filename);
        free(flags);
        return -1;
    }
    fclose(f);

    build_ast_file(flags->filename);

    free(flags);

    return 0;
}
