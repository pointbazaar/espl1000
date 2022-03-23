#ifndef SMALLDRAGON_TOPLEVEL_RAT_H
#define SMALLDRAGON_TOPLEVEL_RAT_H

#include "tables/symtable/symtable.h"

#include <stdbool.h>
#include <inttypes.h>


struct RAT {
    struct ST* st;

    //Register Allocation Table
    //allocation of the registors used inside a function
    //mapping locals, args, and temporaries to actual registers

    uint8_t capacity; //should be 32

    char* occupant[32]; //who occupies it
    bool is_occupied[32]; //is it occupied?
};

struct RAT* rat_ctor(struct ST* st);
void rat_dtor(struct RAT* rat);

void rat_print(struct RAT* rat);

bool rat_has_register(struct RAT* rat, char* name);
int rat_get_register(struct RAT* rat, char* name);

int rat_get_free_register(struct RAT* rat, int start_inclusive);

#endif
