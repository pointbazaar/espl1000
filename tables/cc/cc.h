#ifndef CC_H
#define CC_H

struct CCNode;

struct CC {
	//CC means "Callees and Callers"
	
	struct CCNode* callees;
	struct CCNode* callers;
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
#endif
