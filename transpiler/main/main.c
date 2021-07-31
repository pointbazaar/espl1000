

#include <malloc.h>
#include <flags/flags.h>
#include <util/help.h>
#include <test.h>
#include "transpiler.h"

int main(int argc, char* argv[]){

    mallopt(M_CHECK_ACTION, 3);

    struct Flags* flags = makeFlags(argc, argv);

    if(flags->help){
        sd_print_help();
        freeFlags(flags);
        return 0;
    }

    if(flags->version){
        printf("smalldragon v0.2.0\n");
        freeFlags(flags);
        return 0;
    }

    if(flags->test){
        int status = transpiler_test_all(flags->debug);
        freeFlags(flags);
        return status;
    }

    bool success = transpileAndCompile(flags);

    freeFlags(flags);

    return (success)?0:1;
}