#ifndef LOCALVARSYMTABLE
#define LOCALVARSYMTABLE

#include <inttypes.h>

#include "ast/ast.h"

struct ST;
struct LVSTLine;

struct LVSTLine {
	
	char name[DEFAULT_STR_SIZE]; 
	
	struct Type* type; 
	
	//the first occurence
	//in the subroutine
	//where this local variable was 
	//assigned.
	//can be NULL and is not relevant if is_arg == true
	struct AssignStmt* first_occur;
	
	
	//we must differentiate if
	//it is an argument to the function
	//or not. because arguments already have a type declared
	//(for the c code generation)
	bool is_arg;
	
	bool read_only;

    size_t stack_frame_offset_avr;
};

void freeLVSTLine(struct LVSTLine* l);

struct LVST* makeLVST();

void lvst_clear(struct LVST* lvst);

void freeLVST(struct LVST* lvst);

void lvst_add(struct LVST* lvst, struct LVSTLine* line);

struct LVSTLine* lvst_get(struct LVST* lvst, char* name);

struct LVSTLine* lvst_get_2(struct LVST* lvst, uint32_t index);

uint32_t lvst_index_of(struct LVST* lvst, char* name);

bool lvst_contains(struct LVST* lvst, char* name);

void lvst_print(struct LVST* lvst);


size_t lvst_stack_frame_size_avr(struct LVST* lvst);
size_t lvst_stack_frame_offset_avr(struct LVST* lvst, char* local_var_name);

#endif
