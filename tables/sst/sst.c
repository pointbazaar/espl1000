#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include <ast/util/free_ast.h>

#include "tables/cc/cc.h"
#include "tables/sst/sst.h"
#include "tables/symtable/symtable.h"

#include "token/TokenKeys.h"

#define SST_INITIAL_CAPACITY 10

#define ERR_SAME_NAME "[SST][Error] 2 subroutines with same name\n"

#define ERR_NOT_FOUND "[SST][Error] subroutine not found"

struct SST {
	//Subroutine Symbol Table (SST)
	//this struct should be opaque
	//outside this file

	uint32_t count;
	size_t capacity;

	struct SSTLine** lines;
};

static void sst_resize(struct SST* sst);

struct SST* sst_ctor() {

	struct SST* sst = make(SST);

	sst->capacity = SST_INITIAL_CAPACITY;

	const int nbytes = sizeof(struct SSTLine*) * sst->capacity;

	sst->count = 0;
	sst->lines = malloc(nbytes);

	if (!sst->lines) {
		free(sst);
		return NULL;
	}

	return sst;
}

void sst_clear(struct SST* sst) {

	for (uint32_t i = 0; i < sst->count; i++) {

		sst_line_free(sst->lines[i]);
	}

	free(sst->lines);

	sst->count = 0;
	sst->capacity = SST_INITIAL_CAPACITY;

	sst->lines = malloc(sizeof(struct SSTLine*) * sst->capacity);
}

void sst_free(struct SST* sst) {

	for (uint32_t i = 0; i < sst->count; i++) {
		sst_line_free(sst->lines[i]);
	}

	free(sst->lines);
	free(sst);
}

struct SSTLine* sst_line_ctor(
    char* name,
    char* _namespace,
    struct Type* return_type,
    bool isLibC,
    enum HALTS halts,
    bool has_side_effect) {

	// TODO: why is this unused?
	(void)isLibC;

	struct SSTLine* line = make(SSTLine);

	asprintf(&(line->name), "%s", name);
	asprintf(&(line->_namespace), "%s", _namespace);

	line->method = NULL;
	line->type = NULL;

	line->return_type = return_type;
	line->cc = cc_ctor();

	line->dead = DEAD_UNKNOWN;
	line->halts = halts;

	line->is_private = false;

	line->has_side_effect = has_side_effect;

	return line;
}

struct SSTLine* sst_line_ctor2(
    struct Method* m,
    struct Type* type,
    char* _namespace) {

	struct SSTLine* line = make(SSTLine);

	asprintf(&(line->name), "%s", m->decl->name);
	asprintf(&(line->_namespace), "%s", _namespace);

	line->method = m;
	line->type = type;

	line->return_type = m->decl->return_type;
	line->cc = cc_ctor();

	line->dead = DEAD_UNKNOWN;
	line->halts = HALTS_UNKNOWN;

	line->is_private = has_annotation(m->super.annotations, ANNOT_PRIVATE);

	line->has_side_effect = m->decl->has_side_effects;

	return line;
}

void sst_line_free(struct SSTLine* l) {

	if (l->cc != NULL) { cc_free(l->cc); }

	free(l->name);
	free(l->_namespace);

	free(l);
}

bool sst_add(struct SST* sst, struct SSTLine* line) {

	if (sst_contains(sst, line->name)) {

		fprintf(stderr, ERR_SAME_NAME);
		fprintf(stderr, "\t%s\n", line->name);
		return false;
	}

	if (line->type == NULL || line->return_type == NULL) {
		fprintf(stderr, "[SST_PREFILL][Error] line->type or line->return_type not set for %s\n", line->name);
		return false;
	}

	sst_resize(sst);

	sst->lines[sst->count] = line;
	sst->count += 1;

	return true;
}

struct SSTLine* sst_get(struct SST* sst, const char* name) {

	for (uint32_t i = 0; i < sst->count; i++) {

		struct SSTLine* line = sst->lines[i];

		if (strcmp(line->name, name) == 0) { return line; }
	}

	fprintf(stderr, "%s: '%s'\n", ERR_NOT_FOUND, name);
	return NULL;
}

uint32_t sst_size(struct SST* sst) {
	return sst->count;
}

struct SSTLine* sst_at(struct SST* sst, uint32_t index) {

	if (index >= sst->count) {
		fprintf(stderr, "index %d out of bounds in sst_at", index);
		return NULL;
	}
	return sst->lines[index];
}

int32_t sst_index_of(struct SST* sst, const char* name) {

	for (size_t i = 0; i < sst->count; i++) {
		if (strcmp(sst->lines[i]->name, name) == 0) { return i; }
	}

	fprintf(stderr, ERR_NOT_FOUND);
	fprintf(stderr, "\t%s in sst_index_of\n", name);
	return -1;
}

uint32_t sst_args_size_avr(struct SST* sst, char* name) {

	struct MethodDecl* decl = sst_get(sst, name)->method->decl;
	uint32_t size = 0;

	for (int i = 0; i < decl->count_args; i++) {
		size += lvst_sizeof_type(decl->args[i]->type, false);
	}
	return size;
}

uint32_t sst_args_size_x86(struct SST* sst, char* name) {
	return sst_args_size_avr(sst, name);
}

uint32_t sst_args_count(struct SST* sst, char* name) {
	const struct MethodDecl* decl = sst_get(sst, name)->method->decl;
	return decl->count_args;
}

bool sst_contains(struct SST* sst, const char* name) {

	for (size_t i = 0; i < sst->count; i++) {
		if (strcmp(sst->lines[i]->name, name) == 0) { return true; }
	}

	return false;
}

static void sst_resize(struct SST* sst) {

	if (sst->count < sst->capacity) { return; }

	sst->capacity *= 2;

	const int nbytes = sizeof(struct SSTLine*) * (sst->capacity);

	sst->lines = realloc(sst->lines, nbytes);
}
