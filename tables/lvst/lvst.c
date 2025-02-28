#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "ast/ast.h"
#include "ast/util/str_ast.h"

#include "tables/lvst/lvst.h"

#define LVST_INITIAL_CAPACITY 10

struct LVST {
	//Local Variable Symbol Table (LVST)

	//struct LVST should be opaque,
	//except in this file, for better encapsulation

	unsigned int count;
	size_t capacity;

	struct LVSTLine** lines;
};

struct LVST* lvst_ctor() {

	struct LVST* lvst = make(LVST);

	if (!lvst) {
		return NULL;
	}

	lvst->count = 0;
	lvst->capacity = LVST_INITIAL_CAPACITY;
	lvst->lines = malloc(sizeof(struct LVSTLine*) * lvst->capacity);

	if (!lvst->lines) {
		free(lvst);
		return NULL;
	}

	return lvst;
}

bool lvst_clear(struct LVST* lvst) {

	struct LVSTLine* prev = NULL;

	for (int i = 0; i < lvst->count; i++) {
		if (lvst->lines[i] != prev) {
			prev = lvst->lines[i];
			free(lvst->lines[i]->name);
			free(lvst->lines[i]);
		}
	}
	free(lvst->lines);

	lvst->count = 0;
	lvst->capacity = LVST_INITIAL_CAPACITY;
	lvst->lines = malloc(sizeof(struct LVSTLine*) * lvst->capacity);

	if (!lvst->lines) {
		return false;
	}

	return true;
}

void lvst_free(struct LVST* lvst) {

	struct LVSTLine* prev = NULL;

	for (int i = 0; i < lvst->count; i++) {
		struct LVSTLine* line = lvst->lines[i];

		if (line != prev) {
			prev = line;

			free(line->name);
			free(line);
		}
	}
	free(lvst->lines);
	free(lvst);
}

void lvst_add(struct LVST* lvst, struct LVSTLine* line) {

	for (int i = 0; i < lvst->count; i++) {

		struct LVSTLine* current_line = lvst->lines[i];

		if (strcmp(current_line->name, line->name) == 0) {

			//this local var is already present
			//free the line
			free(line);
			return;
		}
	}

	if (lvst->count >= lvst->capacity) {

		lvst->capacity *= 2;

		const int nbytes = sizeof(struct LVSTLine*) * (lvst->capacity);

		lvst->lines = realloc(lvst->lines, nbytes);
	}

	lvst->lines[lvst->count] = line;
	lvst->count += 1;
}

struct LVSTLine* lvst_line_ctor(char* name, struct Type* type, bool is_arg) {

	struct LVSTLine* line = calloc(1, sizeof(struct LVSTLine));

	line->name = strdup(name);
	line->type = type;
	line->is_arg = is_arg;

	return line;
}

struct LVSTLine* lvst_get(struct LVST* lvst, char* name) {

	for (int i = 0; i < lvst->count; i++) {

		struct LVSTLine* line = lvst->lines[i];

		if (strcmp(line->name, name) == 0) { return line; }
	}

	fprintf(stderr, "[LVST][Error] %s not found in LVST\n", name);
	lvst_print(lvst);
	return NULL;
}

struct LVSTLine* lvst_at(struct LVST* lvst, uint32_t index) {
	if (index >= lvst->count) {
		printf("[LVST][Error] index %d not found in LVST\n", index);
		lvst_print(lvst);
		return NULL;
	}
	return lvst->lines[index];
}

struct LVSTLine* lvst_arg_at(struct LVST* lvst, uint32_t index) {

	uint32_t count = 0;
	for (int i = 0; i < lvst->count; i++) {

		struct LVSTLine* line = lvst->lines[i];
		if (!line->is_arg) {
			continue;
		}

		count++;
		if (count > index) {
			return line;
		}
	}

	const size_t nvars = lvst_nvars(lvst);
	const size_t nargs = lvst_nargs(lvst);

	fprintf(stderr, "%s: could not find arg with index %d\n", __func__, index);
	fprintf(stderr, "lvst has %ld local vars, %ld args\n", nvars, nargs);
	return NULL;
}

int32_t lvst_index_of(struct LVST* lvst, char* name) {

	for (int i = 0; i < lvst->count; i++) {

		struct LVSTLine* line = lvst->lines[i];
		if (strcmp(line->name, name) == 0) { return i; }
	}

	printf("[LVST][Error] %s not found in LVST\n", name);
	lvst_print(lvst);
	return -1;
}

bool lvst_contains(struct LVST* lvst, char* name) {

	for (int i = 0; i < lvst->count; i++) {

		char* lv_name = lvst->lines[i]->name;

		if (strcmp(lv_name, name) == 0) { return true; }
	}
	return false;
}

void lvst_print(struct LVST* lvst) {

	char* fmt = " |% -24s|%-6s| %-24s |\n";
	char* linebig = "------------------------";
	char* line5 = "-----";

	printf("Local Variable Symbol Table [LVST]\n");
	printf(fmt, "name", "is_arg", "Type");
	printf(fmt, linebig, line5, linebig);
	for (int i = 0; i < lvst->count; i++) {
		struct LVSTLine* line = lvst->lines[i];

		printf(fmt,
		       line->name,
		       (line->is_arg) ? "yes" : "no",
		       str_type(line->type));
	}
}

// @returns < 0 on error
static int32_t lvst_sizeof_primitivetype(struct PrimitiveType* pt, bool x86) {

	if (pt->is_char_type || pt->is_bool_type) return 1;

	switch (pt->int_type) {

		// the size of this type is arch-dependent :(
		case INT:
		case UINT:
			if (x86) {
				return 4;
			}
			return 1;

		case INT8:
		case UINT8: return 1;

		case INT16:
		case UINT16: return 2;
		case INT32:
		case UINT32: return 4;
		case INT64:
		case UINT64: return 8;

		default:
			fprintf(stderr, "%s: unhandled case %d\n", __func__, pt->int_type);
			return -1;
	}
}

uint32_t lvst_sizeof_structtype(struct StructType* st, bool x86) {
	if (x86) {
		return 8;
	}
	return 2;
}

uint32_t lvst_sizeof_simpletype(struct SimpleType* st, bool x86) {

	if (st->struct_type != NULL)
		return lvst_sizeof_structtype(st->struct_type, x86);

	if (st->primitive_type != NULL)
		return lvst_sizeof_primitivetype(st->primitive_type, x86);

	assert(false);
	return 0;
}

uint32_t lvst_sizeof_subrtype(struct SubrType* st, bool x86) {
	if (x86) {
		return 8;
	}
	return 2; //function pointer type
}

uint32_t lvst_sizeof_basictype(struct BasicType* bt, bool x86) {

	if (bt->subr_type != NULL)
		return lvst_sizeof_subrtype(bt->subr_type, x86);

	if (bt->simple_type != NULL)
		return lvst_sizeof_simpletype(bt->simple_type, x86);

	assert(false);
	return 0;
}

uint32_t lvst_sizeof_typeparam(struct TypeParam* tp) {
	return 2; // could be wide
}

uint32_t lvst_sizeof_arraytype(struct ArrayType* at, bool x86) {
	if (x86) {
		return 8;
	}
	return 2;
}

static uint32_t lvst_sizeof_pointer_type(bool x86) {
	if (x86) {
		return 8;
	}
	return 2;
}

// returns the size in number of bytes
uint32_t lvst_sizeof_type(struct Type* type, bool x86) {

	assert(type);

	uint32_t res = 0;
	//sizeof(type) in bytes
	if (type->basic_type != NULL)
		res = lvst_sizeof_basictype(type->basic_type, x86);

	if (type->type_param != NULL)
		res = lvst_sizeof_typeparam(type->type_param);

	if (type->array_type != NULL)
		res = lvst_sizeof_arraytype(type->array_type, x86);

	if (type->pointer_type != NULL)
		res = lvst_sizeof_pointer_type(x86);

	return res;
}

size_t lvst_stack_frame_size_local_vars_x86(struct LVST* lvst) {

	uint32_t sum = 0;

	for (int i = 0; i < lvst->count; i++) {
		if (lvst->lines[i]->is_arg) { continue; }
		sum += lvst_sizeof_type(lvst->lines[i]->type, true);
	}
	return sum;
}

size_t lvst_stack_frame_size_x86(struct LVST* lvst) {

	uint32_t sum = 0;

	for (int i = 0; i < lvst->count; i++) {
		sum += lvst_sizeof_type(lvst->lines[i]->type, true);
	}
	return sum;
}

size_t lvst_stack_frame_size_avr(struct LVST* lvst) {
	//give the size required for the stack frame.
	//(here meaning just the local variables)
	uint32_t sum = 0;

	for (int i = 0; i < lvst->count; i++) {
		if (lvst->lines[i]->is_arg) continue;
		sum += lvst_sizeof_type(lvst->lines[i]->type, false);
	}

	return sum;
}

// offset from rbp
ssize_t lvst_stack_frame_offset_x86(struct LVST* lvst, char* local_var_name) {
	uint32_t offset = 0;

	for (int i = 0; i < lvst->count; i++) {

		struct LVSTLine* line = lvst->lines[i];

		if (line->is_arg == true) continue;

		offset += lvst_sizeof_type(line->type, true);

		if (strcmp(line->name, local_var_name) == 0) {
			return offset;
		}
	}

	for (int i = 0; i < lvst->count; i++) {

		struct LVSTLine* line = lvst->lines[i];

		if (line->is_arg == false) continue;

		offset += lvst_sizeof_type(line->type, true);

		if (strcmp(line->name, local_var_name) == 0) {
			return offset;
		}
	}

	fprintf(stderr, "fatal error in lvst_stack_frame_offset_x86.");
	fprintf(stderr, "did not find local: %s", local_var_name);
	return -1;
}

int32_t lvst_arg_index(struct LVST* lvst, char* name) {

	uint32_t index = 0;

	for (int i = 0; i < lvst->count; i++) {

		struct LVSTLine* line = lvst->lines[i];

		if (line->is_arg == false) continue;

		if (strcmp(line->name, name) == 0) {
			return index;
		}
		index++;
	}

	fprintf(stderr, "%s: did not find '%s' in arguments\n", __func__, name);
	return -1;
}

ssize_t lvst_stack_frame_offset_avr(struct LVST* lvst, char* local_var_name) {

	uint32_t offset = 1;

	//we first look at the local vars
	for (int i = 0; i < lvst->count; i++) {

		struct LVSTLine* line = lvst->lines[i];

		if (line->is_arg == true) continue;

		if (strcmp(line->name, local_var_name) == 0) return offset;

		offset += lvst_sizeof_type(line->type, false);
	}

	offset += 2; //because of the return address

	//we then look at the arguments
	for (int i = 0; i < lvst->count; i++) {

		struct LVSTLine* line = lvst->lines[i];

		if (line->is_arg == false) continue;

		if (strcmp(line->name, local_var_name) == 0) return offset;

		offset += lvst_sizeof_type(line->type, false);
	}

	fprintf(stderr, "fatal error in lvst_stack_frame_offset_avr.");
	fprintf(stderr, "did not find local: %s", local_var_name);
	return -1;
}

uint32_t lvst_sizeof_var(struct LVST* lvst, char* name, bool x86) {

	struct LVSTLine* line = lvst_get(lvst, name);
	assert(line->type);
	return lvst_sizeof_type(line->type, x86);
}

size_t lvst_nvars(struct LVST* lvst) {
	size_t count = 0;
	for (int i = 0; i < lvst->count; i++) {
		if (!lvst->lines[i]->is_arg) {
			count++;
		}
	}
	return count;
}

size_t lvst_nargs(struct LVST* lvst) {
	size_t count = 0;
	for (int i = 0; i < lvst->count; i++) {
		if (lvst->lines[i]->is_arg) {
			count++;
		}
	}
	return count;
}
