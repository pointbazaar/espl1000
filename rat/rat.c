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

	char* note[RAT_CAPACITY];
    uint32_t occupant[RAT_CAPACITY]; //who occupies it (which temp)
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
        rat->occupant[i] = 0;
        rat->is_occupied[i] = false;
        rat->note[i] = "";
    }

    //r0 is our garbage register, for when we need to pop something off the stack
    //to destroy our stackframe
    rat->is_occupied[0] = true;
    rat->occupant[0] = 439434; //garbage value
    rat->note[0] = "reserved";

    //r16 is another reserved multi-use register,
    //as there is a constraint that
    //many instructions can only use registers >= r16
    rat->is_occupied[16] = true;
    rat->occupant[16] = 879563; //garbage value
    rat->note[16] = "reserved";

    //r26 through r31 are X,Y,Z
    //and are used as pointer registers,
    //and should not be available for allocation
    rat->is_occupied[26] = true; //X
    rat->is_occupied[27] = true; //X
    rat->note[26] = "XL";
    rat->note[27] = "XH";

    //Y is our base pointer for the stack frame
    rat->is_occupied[28] = true; //Y
    rat->is_occupied[29] = true; //Y
    rat->note[28] = "YL";
    rat->note[29] = "YH";

    rat->is_occupied[30] = true; //Z
    rat->is_occupied[31] = true; //Z
    rat->note[30] = "ZL";
    rat->note[31] = "ZH";

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
			if(strcmp(rat->note[i], "") == 0){
				printf("t%19d", rat->occupant[i]);
			}else{
				printf("%20s", rat->note[i]);
			}
        }else{
            printf("%20s", " - ");
        }

        printf("\n");
    }
    printf("------------\n");
}


bool rat_has_register(struct RAT* rat, uint32_t tmp_index){
	
	for(int i=0;i < RAT_CAPACITY; i++){
        if(rat->is_occupied[i] && rat->occupant[i] == tmp_index){
            return true;
        }
    }
    return false;
}

int rat_get_register(struct RAT* rat, uint32_t tmp_index){
	
	for(int i=0;i < RAT_CAPACITY; i++){
        if(rat->is_occupied[i] && rat->occupant[i] == tmp_index){
            return i;
        }
    }
    
    printf("[RAT] rat_get_register: t%d has no register\n", tmp_index);
    rat_print(rat);
    exit(1);
    
    return -1;
}

void rat_occupy_register(struct RAT* rat, uint8_t reg, uint32_t tmp_index){
	//occupy via tmp index
	rat->is_occupied[reg] = true;
	rat->occupant[reg] = tmp_index;
}

bool rat_occupied(struct RAT* rat, uint8_t reg){
	if(reg >= RAT_CAPACITY) return false;
	
	return rat->is_occupied[reg];
}

uint32_t rat_occupant(struct RAT* rat, uint8_t reg){
	return rat->occupant[reg];
}

uint32_t rat_ensure_register(struct RAT* rat, uint32_t tmp_index, bool high_regs_only){
	
	//printf("rat ensure t%d, higher=%d",tmp_index, high_regs_only);
	//fflush(stdout);
	
	if(!rat_has_register(rat, tmp_index)){
		
		uint32_t reg = rat_get_free_register(rat, high_regs_only);
		rat_occupy_register(rat, reg, tmp_index);
	}
	
	return rat_get_register(rat, tmp_index);
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
