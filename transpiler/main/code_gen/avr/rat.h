#ifndef SMALLDRAGON_TOPLEVEL_RAT_H
#define SMALLDRAGON_TOPLEVEL_RAT_H

#include <stdbool.h>
#include <inttypes.h>

//who occupies a registers currently
enum RAT_OCCUPANT {
    RAT_OCCUPIED_LOCAL,
    RAT_OCCUPIED_ARG,
    RAT_OCCUPIED_TEMPORARY,

    RAT_OCCUPANT_NONE, //no one occupies the register
};

struct RAT {
    //Register Allocation Table
    //allocation of the registors used inside a function
    //mapping locals, args, and temporaries to actual registers

    uint8_t capacity; //should be 32

    //map locals, args and temporaries by their index
    enum RAT_OCCUPANT occupant[32]; //who occupies it
    uint8_t occupant_index[32]; //their index in their respective category
};

struct RAT* rat_ctor();
void rat_dtor(struct RAT* rat);

void rat_print(struct RAT* rat);

#endif
