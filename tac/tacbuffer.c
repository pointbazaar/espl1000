#include <stdlib.h>
#include <stdio.h>

#include "ast/ast.h"

#include "tacbuffer.h"
#include "tac.h"

struct Ctx;
struct ST;

//TACBuffer should be opaque outside of this file
struct TACBuffer {
	size_t cap;
	size_t count;
	struct TAC** buffer;
};

void tacbuffer_append(struct TACBuffer* buffer, struct TAC* node) {

	if (buffer->count >= buffer->cap)
		buffer->buffer = realloc(buffer->buffer, sizeof(struct TAC*) * (buffer->cap *= 2));

	buffer->buffer[buffer->count] = node;

	buffer->count += 1;
}

bool tacbuffer_set(struct TACBuffer* buffer, int index, struct TAC* node) {

	if (index >= buffer->cap || index >= buffer->count) {
		fprintf(stderr, "trying to write to TACBuffer outside its bounds...");
		return false;
	}

	buffer->buffer[index] = node;
	return true;
}

struct TAC* tacbuffer_get(struct TACBuffer* buffer, int index) {

	if (index >= buffer->count) {
		fprintf(stderr, "TACBuffer: no value at index %d ...", index);
		return NULL;
	}

	return buffer->buffer[index];
}

struct TAC* tacbuffer_get_last(struct TACBuffer* buffer) {
	return tacbuffer_get(buffer, tacbuffer_count(buffer) - 1);
}

struct TACBuffer* tacbuffer_ctor() {

	struct TACBuffer* buffer = make(TACBuffer);

	const int initial_size = 32;

	*buffer = (struct TACBuffer){
	    .buffer = malloc(sizeof(struct TAC*) * initial_size),
	    .count = 0,
	    .cap = initial_size};
	return buffer;
}

void tacbuffer_shallow_dtor(struct TACBuffer* buffer) {
	free(buffer->buffer);
	free(buffer);
}

void tacbuffer_dtor(struct TACBuffer* buffer) {
	struct TAC* prev = NULL;
	for (size_t i = 0; i < buffer->count; i++) {
		if (buffer->buffer[i] != prev) {
			prev = buffer->buffer[i];
			free(buffer->buffer[i]);
		}
	}
	free(buffer->buffer);
	free(buffer);
}

void tacbuffer_print(struct TACBuffer* buffer, struct SST* sst, struct LVST* lvst) {
	char* s = tacbuffer_tostring(buffer, false, sst, lvst);
	printf("%s", s);
	free(s);
}
char* tacbuffer_tostring(struct TACBuffer* buffer, bool graphviz, struct SST* sst, struct LVST* lvst) {
	char* res = malloc(sizeof(char) * (buffer->count * 100));

	if (!res) {
		return NULL;
	}

	strcpy(res, "");

	for (size_t k = 0; k < buffer->count; k++) {
		char* s = tac_tostring(buffer->buffer[k], sst, lvst);
		strcat(res, s);

		strcat(res, (graphviz) ? "\\l" : "\n");

		free(s);
	}

	return res;
}

uint32_t tacbuffer_last_dest(struct TACBuffer* buffer) {

	struct TAC* tac = buffer->buffer[buffer->count - 1];
	return tac_opt_dest(tac);
}

size_t tacbuffer_count(struct TACBuffer* buffer) {
	return buffer->count;
}

ssize_t tacbuffer_indexof(struct TACBuffer* buffer, struct TAC* tac) {

	for (size_t k = 0; k < buffer->count; k++) {
		if (buffer->buffer[k] == tac) return k;
	}

	//FATAL ERROR
	return -1;
}
