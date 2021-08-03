

#include <malloc.h>
#include <flags/flags.h>
#include <util/help.h>
#include <test.h>
#include "util/fileutils/fileutils.h"
#include "transpiler.h"

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

int main(int argc, char* argv[]){

    mallopt(M_CHECK_ACTION, 3);

    struct Flags* flags = makeFlags(argc, argv);

    if(!check_filenames_lowercase(flags)){
        return EXIT_FAILURE;
    }

    if(flags->help){
        sd_print_help();
        freeFlags(flags);
        return EXIT_SUCCESS;
    }

    if(flags->version){
        printf("smalldragon v0.2.0\n");
        freeFlags(flags);
        return EXIT_SUCCESS;
    }

    if(flags->test){
        int status = transpiler_test_all(flags->debug);
        freeFlags(flags);
        return status;
    }

    bool success = transpileAndCompile(flags);

    freeFlags(flags);

    return (success)?EXIT_SUCCESS:EXIT_FAILURE;
}