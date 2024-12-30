#ifndef LOCALVARSYMTABLE
#define LOCALVARSYMTABLE

#include <inttypes.h>
#include <stdio.h>

#include "ast/ast.h"

struct ST;
struct LVSTLine;

struct LVSTLine {

	char* name;

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
};

struct LVST* lvst_ctor();

void lvst_clear(struct LVST* lvst);

void lvst_free(struct LVST* lvst);

void lvst_add(struct LVST* lvst, struct LVSTLine* line);

struct LVSTLine* lvst_line_ctor(char* name, struct Type* type, bool is_arg);

struct LVSTLine* lvst_get(struct LVST* lvst, char* name);

struct LVSTLine* lvst_at(struct LVST* lvst, uint32_t index);

uint32_t lvst_index_of(struct LVST* lvst, char* name);

bool lvst_contains(struct LVST* lvst, char* name);

void lvst_print(struct LVST* lvst);

// @returns     the size of a type in bytes
// @returns     0 on failure
// @param x86   is the target x86?
//              The size of 'int' is arch-dependent.
uint32_t lvst_sizeof_type(struct Type* type, bool x86);

size_t lvst_stack_frame_size_avr(struct LVST* lvst);
size_t lvst_stack_frame_offset_avr(struct LVST* lvst, char* local_var_name);

size_t lvst_stack_frame_size_x86(struct LVST* lvst);
ssize_t lvst_stack_frame_offset_x86(struct LVST* lvst, char* local_var_name);

// @returns   index of 'name' in the argument list
// @precondition 'name' must be an argument in this LVST
// @precondition line->is_arg == true
uint32_t lvst_arg_index(struct LVST* lvst, char* name);

// @returns the size of the local variable in bytes.
// @returns 0 on failure
// @param x86   is the target x86?
//              The size of 'int' is arch-dependent.
uint32_t lvst_sizeof_var(struct LVST* lvst, char* name, bool x86);

#endif
