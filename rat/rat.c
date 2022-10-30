#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "rat.h"


struct RAT {
	//struct RAT should be opaque outside of it's 
	//implementation file
	
    struct ST* st;

    //Register Allocation Table
    //allocation of the registors used inside a function
    //mapping locals, args, and temporaries to actual registers

    char* occupant[RAT_CAPACITY]; //who occupies it
    bool is_occupied[RAT_CAPACITY]; //is it occupied?

    //X: r26/r27
    //Y: r28/r29
    //Z: r30/r31
};

struct RAT* rat_ctor(struct ST* st){

    struct RAT* rat = malloc(sizeof(struct RAT));
    *rat = (struct RAT){
        .st = st,
    };

    for(int i=0;i < RAT_CAPACITY;i++){
        rat->occupant[i] = "";
    }
    memset(rat->is_occupied, false, sizeof(bool)*RAT_CAPACITY);

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

    for(int i=26;i < RAT_CAPACITY;i++){
        rat->occupant[i] = "reserved";
    }

    return rat;
}

void rat_dtor(struct RAT* rat){
    free(rat);
}

void rat_print(struct RAT* rat){

    printf("Register Allocation Table:\n");
    for(size_t i = 0; i < RAT_CAPACITY; i++){

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

bool rat_has_register_2(struct RAT* rat, char* name){
    return rat_get_register_2(rat,name) >= 0;
}

bool rat_has_register(struct RAT* rat, uint32_t tmp_index){
	return rat_get_register(rat, tmp_index) >= 0;
}

int rat_get_register_2(struct RAT* rat, char* name){
    for(int i=0;i < RAT_CAPACITY; i++){
        if(rat->is_occupied[i] && (strcmp(rat->occupant[i], name) == 0)){
            return i;
        }
    }
    return -1;
}

int rat_get_register(struct RAT* rat, uint32_t tmp_index){
	char* str = malloc(DEFAULT_STR_SIZE);
	sprintf(str, "t%d", tmp_index);
	
	for(int i=0;i < RAT_CAPACITY; i++){
        if(rat->is_occupied[i] && (strcmp(rat->occupant[i], str) == 0)){
			free(str);
            return i;
        }
    }
    free(str);
    return -1;
}

void rat_occupy_register_2(struct RAT* rat, uint8_t reg, char* name){
	//occupy via name
	rat->is_occupied[reg] = true;
	rat->occupant[reg] = name;
}

void rat_occupy_register(struct RAT* rat, uint8_t reg, uint32_t tmp_index){
	//occupy via tmp index
	rat->is_occupied[reg] = true;
	
	char* str = malloc(DEFAULT_STR_SIZE);
	sprintf(str, "t%d", tmp_index);
	
	rat->occupant[reg] = str;
}

bool rat_occupied(struct RAT* rat, uint8_t reg){
	if(reg >= RAT_CAPACITY) return false;
	
	return rat->is_occupied[reg];
}

char* rat_occupant(struct RAT* rat, uint8_t reg){
	return rat->occupant[reg];
}

int rat_get_free_register(struct RAT* rat, bool high_regs_only){
    //high_regs_only tells us at which register to start looking,
    //as there are some instructions such as 'ldi' which can only use a high register
    // (>= r16)
    
    const int start_inclusive = (high_regs_only)?16:0;

    for(int i=start_inclusive;i < RAT_CAPACITY; i++){
        if(!rat->is_occupied[i]){
            return i;
        }
    }
    printf("RAT could not find a free register, they are all occupied.\n");
    rat_print(rat);
    exit(1);
    return -1;
}
