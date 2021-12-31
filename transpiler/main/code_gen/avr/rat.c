#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "rat.h"

struct RAT* rat_ctor(){

    struct RAT* rat = malloc(sizeof(struct RAT));
    rat->capacity = 32;

    memset(rat->occupant, RAT_OCCUPANT_NONE, sizeof(enum RAT_OCCUPANT)*rat->capacity);
    memset(rat->occupant_index, 0, sizeof(uint8_t)*rat->capacity);

    return rat;
}

void rat_dtor(struct RAT* rat){
    free(rat);
}

void rat_print(struct RAT* rat){

    printf("Register Allocation Table:\n");
    for(size_t i = 0; i < rat->capacity; i++){

        printf("r%02ld: ", i);
        int32_t index = rat->occupant_index[i];

        switch (rat->occupant[i]) {
            case RAT_OCCUPANT_NONE:
                printf("-"); break;
            case RAT_OCCUPIED_LOCAL:
                printf("local #%d", index); break;
            case RAT_OCCUPIED_ARG:
                printf("arg #%d", index); break;
            case RAT_OCCUPIED_TEMPORARY:
                printf("t%d", index); break;
        }
        printf("\n");
    }
    printf("------------\n");
}