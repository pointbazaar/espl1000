#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "ast/ast.h"
#include "ast/util/copy_ast.h"

#include "token/TokenKeys.h"
#include "parser/main/astnodes/types/Type.h"

#include "stst.h"
#include "stst_print.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

#define STST_INITIAL_CAPACITY 10;

#define ERR_NOT_FOUND "[STST] could not find struct \n"

struct STST {
	//STruct Symbol Table (STST)

	//struct STST should be opaque,
	//except in this file, for better encapsulation

	struct STSTLine** lines;

	unsigned int count;
	unsigned int capacity;
};

struct STST* stst_ctor() {

	struct STST* stst = make(STST);

	if (!stst) {
		return NULL;
	}

	stst->capacity = STST_INITIAL_CAPACITY;
	stst->lines = malloc(sizeof(struct STSTLine*) * stst->capacity);

	if (!stst->lines) {
		return NULL;
	}

	stst->count = 0;

	return stst;
}

void stst_fill(struct ST* st, struct Namespace* ns) {

	struct STST* stst = st->stst;

	for (size_t i = 0; i < ns->count_structs; i++) {

		struct StructDecl* mystruct = ns->structs[i];

		struct STSTLine* line = stst_line_ctor(st, mystruct, ns->name);

		stst_add(stst, line);
	}
}

struct STSTLine* stst_get(struct STST* stst, char* name) {

	for (size_t i = 0; i < stst->count; i++) {

		struct STSTLine* line = stst->lines[i];

		if (strcmp(line->name, name) == 0) { return line; }
	}

	fprintf(stderr, ERR_NOT_FOUND);
	fprintf(stderr, "\t%s\n", name);
	return NULL;
}

struct StructMember* stst_get_member(struct STST* stst, char* struct_name, char* member_name) {

	struct STSTLine* line = stst_get(stst, struct_name);

	assert(line->decl);
	assert(struct_name);
	assert(member_name);

	for (int j = 0; j < line->decl->count_members; j++) {

		struct StructMember* member = line->decl->members[j];

		if (strcmp(member->name, member_name) == 0) { return member; }
	}

	char* msg = "[STST] could not find member '%s' of '%s'\n";
	fprintf(stderr, msg, member_name, struct_name);

	stst_print(stst);
	return NULL;
}

struct Type* stst_member_type(struct STST* stst, const char* struct_name, const char* member_name) {

	struct StructMember* member = stst_get_member(stst, (char*)struct_name, (char*)member_name);

	if (!member) {
		return NULL;
	}

	return member->type;
}

bool stst_has_member(struct STST* stst, const char* struct_name, const char* member_name) {

	assert(struct_name);
	assert(member_name);

	struct STSTLine* line = stst_get(stst, (char*)struct_name);

	if (!line) {
		return false;
	}

	assert(line->decl);

	for (int j = 0; j < line->decl->count_members; j++) {

		struct StructMember* member = line->decl->members[j];

		if (strcmp(member->name, member_name) == 0) { return true; }
	}

	return false;
}

int32_t stst_member_offset(struct STST* stst, char* struct_name, char* member_name, bool x86) {

	//calculates the offset in bytes, from the start of the struct

	struct STSTLine* line = stst_get(stst, struct_name);

	uint32_t offset = 0;

	assert(line->decl);

	for (int j = 0; j < line->decl->count_members; j++) {

		struct StructMember* member = line->decl->members[j];

		if (strcmp(member->name, member_name) == 0) return offset;

		offset += lvst_sizeof_type(member->type, x86);
	}

	fprintf(stderr, "[STST] could not find member offset, struct '%s', member '%s'\n", struct_name, member_name);

	return -1;
}

uint32_t stst_size(struct STST* stst) {
	return stst->count;
}

struct STSTLine* stst_at(struct STST* stst, uint32_t index) {
	return stst->lines[index];
}

void stst_free(struct STST* stst) {

	struct STSTLine* prev = NULL;

	for (size_t i = 0; i < stst->count; i++) {
		struct STSTLine* line = stst->lines[i];

		if (line != prev) {
			prev = line;

			free(line->name);
			free(line->_namespace);
			free(line);
		}
	}

	free(stst->lines);

	free(stst);
}

struct STSTLine* stst_line_ctor(struct ST* st, struct StructDecl* s, char* _namespace) {

	struct STSTLine* line = make(STSTLine);

	line->decl = s;
	line->is_private = has_annotation(s->super.annotations, ANNOT_PRIVATE);

	struct SimpleType* copy = copy_simple_type(s->type);

	assert(copy);

	line->type = makeType_4(copy);

	assert(line->type);

	st_register_inferred_type(st, line->type);

	asprintf(&(line->_namespace), "%s", _namespace);
	asprintf(&(line->name), "%s", s->type->struct_type->type_name);

	return line;
}

void stst_add(struct STST* stst, struct STSTLine* line) {

	if (stst->capacity <= stst->count) {

		stst->capacity *= 2;
		stst->lines = realloc(stst->lines, sizeof(struct STSTLine*) * stst->capacity);
	}

	stst->lines[stst->count] = line;
	stst->count += 1;
}
