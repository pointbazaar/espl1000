#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "rat.h"

struct RAT* rat_ctor(struct ST* st){

    struct RAT* rat = malloc(sizeof(struct RAT));
    *rat = (struct RAT){
        .st = st,
        .capacity = 32,
    };

    for(int i=0;i < rat->capacity;i++){
        rat->occupant[i] = "";
    }
    memset(rat->is_occupied, false, sizeof(bool)*rat->capacity);

    //r0 is our garbage register, for when we need to pop something off the stack
    //to destroy our stackframe
    rat->is_occupied[0] = true;
    rat->occupant[0] = "reserved";

    //r16 is another reserved multi-use register,
    //as there is a constraint that
    //many instructions can only use registers >= r16
    rat->is_occupied[16] = true;
    rat->occupant[16] = "reserved";

    //r26 through r31 are X,Y,Z
    //and are used as pointer registers,
    //and should not be available for allocation
    rat->is_occupied[26] = true; //X
    rat->is_occupied[27] = true; //X

    //Y is our base pointer for the stack frame
    rat->is_occupied[28] = true; //Y
    rat->is_occupied[29] = true; //Y

    rat->is_occupied[30] = true; //Z
    rat->is_occupied[31] = true; //Z

    for(int i=26;i < rat->capacity;i++){
        rat->occupant[i] = "reserved";
    }

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

void rat_occupy_register(struct RAT* rat, uint8_t reg, char* name){
	
	rat->is_occupied[reg] = true;
	rat->occupant[reg] = name;
}

int rat_get_free_register(struct RAT* rat, bool high_regs_only){
    //high_regs_only tells us at which register to start looking,
    //as there are some instructions such as 'ldi' which can only use a high register
    // (>= r16)
    
    const int start_inclusive = (high_regs_only)?16:0;

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
