#ifndef CC_H
#define CC_H

#include <stdbool.h>

struct CCNode;

struct CC {
	//CC means "Callees and Callers"
	
	struct CCNode* callees;
	struct CCNode* callers;
	
	bool calls_fn_ptrs;
};

//--------------------------------------------

struct CC* make_cc();
void       free_cc(struct CC* cc);

//--------------------------------------------
void cc_add_callee(struct CC* cc, char* name);
void cc_add_caller(struct CC* cc, char* name);

//--------------------------------------------
struct CCNode* cc_next(struct CCNode* node); //may return NULL
char* cc_name(struct CCNode* node);

void cc_set_calls_fn_ptrs(struct CC* cc, bool value);
#endif
