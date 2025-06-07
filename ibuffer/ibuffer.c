#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "ibuffer_write.h"
#include "ibuffer_shared.h"

#define INSTR_COMMENT_LEN 50

struct Instr {
	//this struct should be opaque outside this file
	enum IKEY key;

	//can be registers, constants, offsets, ...
	//and is interpreted according to 'key'
	int32_t x1;
	int32_t x2;
	int32_t x3;

	//label, etc.
	char* str;

	//comment for the assembly code
	char comment[INSTR_COMMENT_LEN];

	// source line number
	// value 0 means field is invalid / unpopulated
	uint32_t line_no;
};

struct IBuffer {
	//this struct should be opaque outside this file

	struct Instr** buffer;
	uint32_t capacity;
	uint32_t count;

	// Current line number. This is used to populate
	// 'line_no' field of 'struct Instr' when it is inserted into
	// the instruction buffer.
	// Value 0 means field is invalid / unpopulated
	uint32_t line_no;
	uint32_t current_line_no;
};

struct IBuffer* ibu_ctor() {

	struct IBuffer* ibu = malloc(sizeof(struct IBuffer));

	if (!ibu) {
		return NULL;
	}

	ibu->capacity = 100;
	ibu->buffer = malloc(sizeof(struct Instr*) * ibu->capacity);

	if (!ibu->buffer) {
		free(ibu);
		return NULL;
	}

	ibu->count = 0;
	ibu->current_line_no = 0;

	return ibu;
}

void ibu_dtor(struct IBuffer* ibu) {

	for (uint32_t k = 0; k < ibu->count; k++) {
		free(ibu->buffer[k]->str);
		free(ibu->buffer[k]);
	}
	free(ibu->buffer);
	free(ibu);
}

bool ibu_write(struct IBuffer* ibu, FILE* fout) {

	for (uint32_t k = 0; k < ibu->count; k++) {

		struct Instr* i = ibu->buffer[k];

		assert(i != NULL);

		ibu_write_instr(i->key, i->x1, i->x2, i->x3, i->str, i->comment, fout);
	}

	return true;
}

bool ibu_write_to_file(struct IBuffer* ibu, char* filename) {

	FILE* fout = fopen(filename, "w");

	if (fout == NULL) {
		fprintf(stderr, "error opening %s\n", filename);
		return false;
	}

	ibu_write(ibu, fout);
	fclose(fout);

	return true;
}

void ibu_set_line_num(struct IBuffer* ibu, uint32_t line_no) {
	if (!ibu) {
		return;
	}
	ibu->current_line_no = line_no;
}

void ibu_push(struct IBuffer* ibu, struct Instr* i) {

	if (ibu->count >= ibu->capacity) {

		//resize buffer
		ibu->capacity *= 2;
		ibu->buffer = realloc(ibu->buffer, sizeof(struct Instr*) * ibu->capacity);
	}

	i->line_no = ibu->current_line_no;

	ibu->buffer[ibu->count] = i;
	ibu->count += 1;
}

bool ibu_push4(struct IBuffer* ibu, enum IKEY key, int64_t x1, int64_t x2, int64_t x3, char* label, const char* comment) {

	struct Instr* i = malloc(sizeof(struct Instr));

	if (!i) {
		return false;
	}

	i->key = key;
	i->x1 = x1;
	i->x2 = x2;
	i->x3 = x3;

	if (label == NULL) {
		i->str = malloc(1);
		if (!i->str) {
			free(i);
			return false;
		}
		memset(i->str, 0, 1);
	} else {
		i->str = malloc(strlen(label) + 1);
		if (!i->str) {
			free(i);
			return false;
		}
		strcpy(i->str, label);
	}

	strncpy(i->comment, comment, INSTR_COMMENT_LEN - 1);
	i->comment[INSTR_COMMENT_LEN - 1] = '\0';

	ibu_push(ibu, i);

	return true;
}
