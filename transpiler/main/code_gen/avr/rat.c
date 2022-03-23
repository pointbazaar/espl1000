#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "rat.h"

struct RAT* rat_ctor(struct ST* st){

    struct RAT* rat = malloc(sizeof(struct RAT));
    *rat = (struct RAT){
        .st = st,
        .capacity = 32,
    };

    memset(rat->occupant, 0, sizeof(char*)*rat->capacity);
    memset(rat->is_occupied, false, sizeof(bool)*rat->capacity);

    return rat;
}

void rat_dtor(struct RAT* rat){
    free(rat);
}

void rat_print(struct RAT* rat){

    printf("Register Allocation Table:\n");
    for(size_t i = 0; i < rat->capacity; i++){

        printf("r%02ld: ", i);

        if(rat->is_occupied[i]){
            printf("%20s", rat->occupant[i]);
        }else{
            printf("%20s", " - ");
        }

        printf("\n");
    }
    printf("------------\n");
}

bool rat_has_register(struct RAT* rat, char* name){

    return rat_get_register(rat,name) >= 0;
}

int rat_get_register(struct RAT* rat, char* name){
    for(int i=0;i < rat->capacity; i++){
        if(rat->is_occupied[i] && (strcmp(rat->occupant[i], name) == 0)){
            return i;
        }
    }
    return -1;
}

int rat_get_free_register(struct RAT* rat, int start_inclusive){
    //start_inclusive tells us at which register to start looking,
    //as there are some instructions such as 'ldi' which can only use a high register
    // (>= r16)

    for(int i=start_inclusive;i < rat->capacity; i++){
        if(!rat->is_occupied[i]){
            return i;
        }
    }
    printf("RAT could not find a free register, they are all occupied.\n");
    rat_print(rat);
    exit(1);
    return -1;
}