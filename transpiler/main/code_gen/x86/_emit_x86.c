#include <stdio.h>

#include "_emit_x86.h"

void emit_label(char* label, FILE* file){
    fprintf(file, "%s:\n", label);
}

void emit1(char* mnem, FILE* file){

    fprintf(file, "\t%s\n", mnem);
}

void emit2(char* mnem, char* arg, FILE* file){

    fprintf(file, "\t%s %s\n", mnem, arg);
}