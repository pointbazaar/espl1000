#pragma once

#include <inttypes.h>
#include <stdio.h>

#include "ast/ast.h"

struct ST;
struct LVSTLine;

struct LVSTLine {

	char* name;

	struct Type* type;

	//we must differentiate if
	//it is an argument to the function
	//or not. because arguments already have a type declared
	//(for the c code generation)
	bool is_arg;

	bool read_only;
};

// @returns NULL on error
struct LVST* lvst_ctor();

// @returns false on error
bool lvst_clear(struct LVST* lvst);

void lvst_free(struct LVST* lvst);

void lvst_add(struct LVST* lvst, struct LVSTLine* line);

struct LVSTLine* lvst_line_ctor(char* name, struct Type* type, bool is_arg);

// @returns NULL on error
struct LVSTLine* lvst_get(struct LVST* lvst, const char* name);

// @returns NULL on error
struct LVSTLine* lvst_at(struct LVST* lvst, uint32_t index);

// @returns       the function argument with index 'index'
// @param index   the 0-based index of the argument
// @returns NULL on error
struct LVSTLine* lvst_arg_at(struct LVST* lvst, uint32_t index);

// @returns < 0 on error
int32_t lvst_index_of(struct LVST* lvst, const char* name);

bool lvst_contains(struct LVST* lvst, const char* name);

void lvst_print(struct LVST* lvst, FILE* fout);

void lvst_print_filename(struct LVST* lvst, char* filename);

// @returns     the size of a type in bytes
// @returns     0 on failure
// @param x86   is the target x86?
//              The size of 'int' is arch-dependent.
uint32_t lvst_sizeof_type(struct Type* type, bool x86);

size_t lvst_stack_frame_size_avr(struct LVST* lvst);
// @returns < 0 on error
ssize_t lvst_stack_frame_offset_avr(struct LVST* lvst, char* local_var_name);

// @returns   size of the complete stackframe args + locals
size_t lvst_stack_frame_size_x86(struct LVST* lvst);

// @returns   size of the stackframe but only the local variables
size_t lvst_stack_frame_size_local_vars_x86(struct LVST* lvst);
// @returs < 0 on error
ssize_t lvst_stack_frame_offset_x86(struct LVST* lvst, char* local_var_name);

// @returns   index of 'name' in the argument list
// @precondition 'name' must be an argument in this LVST
// @precondition line->is_arg == true
// @returns < 0 on error
int32_t lvst_arg_index(struct LVST* lvst, char* name);

// @returns the size of the local variable in bytes.
// @returns 0 on failure
// @param x86   is the target x86?
//              The size of 'int' is arch-dependent.
uint32_t lvst_sizeof_var(struct LVST* lvst, const char* name, bool x86);

// @returns    number of local variables
size_t lvst_nvars(struct LVST* lvst);
// @returns    number of arguments
size_t lvst_nargs(struct LVST* lvst);
