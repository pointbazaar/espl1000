#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "ast/ast.h"
#include "cc.h"

struct CCNode {

	char name[DEFAULT_STR_SIZE];
	
	struct CCNode* next; //may be NULL
};


static void free_cc_node(struct CCNode* node);

static void add_cc_node(struct CCNode* node, char* name);

static struct CCNode* make_cc_node(char* name, struct CCNode* next);

//--------------------------

struct CC* make_cc(){
	
	struct CC* res = make(CC);
	
	res->callers = NULL;
	res->callees = NULL;
	
	res->calls_fn_ptrs = true; //we don't know
	
	return res;
}

void free_cc(struct CC* cc){

	if(cc->callers != NULL){ free_cc_node(cc->callers); }
	if(cc->callees != NULL){ free_cc_node(cc->callees); }
	
	free(cc);
}

static void free_cc_node(struct CCNode* node){

	if(node->next != NULL)
		{ free_cc_node(node->next); }
		
	free(node);
}

//--------------------------------------------

void cc_add_callee(struct CC* cc, char* name){
	
	if(cc->callees != NULL){
			
		add_cc_node(cc->callees, name);
	}

	if(cc->callees == NULL){
		cc->callees = make_cc_node(name, NULL);
	}
}

void cc_add_caller(struct CC* cc, char* name){
	
	if(cc->callers != NULL){
			
		add_cc_node(cc->callers, name);
	}

	if(cc->callers == NULL){
		cc->callers = make_cc_node(name, NULL);
	}
}

//---------------------------------------------

uint32_t cc_size(struct CCNode* ccnode){
	
	struct CCNode* current = ccnode;
	uint32_t res = 0;
	
	while(current != NULL){
		res++;
		current = current->next;
	}
	return res;
}

static void add_cc_node(struct CCNode* node, char* name){

	char* current = node->name;
	
	if(strcmp(current, name) == 0)
		{ return; }
		
	if(node->next == NULL){
	
		node->next = make_cc_node(name, NULL);
	}else{
		
		add_cc_node(node->next, name);
	}
}

static struct CCNode* make_cc_node(char* name, struct CCNode* next){

	struct CCNode* res = make(CCNode);
	
	strncpy(res->name, name, DEFAULT_STR_SIZE);
	res->next = next;
	
	return res;
}

//--------------------------------------------

struct CCNode* cc_next(struct CCNode* node){
	
	return node->next;
}

char* cc_name(struct CCNode* node){

	return node->name;
}

void cc_set_calls_fn_ptrs(struct CC* cc, bool value){

	cc->calls_fn_ptrs = value;
}
