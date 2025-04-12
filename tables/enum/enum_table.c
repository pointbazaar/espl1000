#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "enum_table.h"

struct EnumTable {

	struct EnumTableEntry** entries;
	size_t count_entries;
	size_t capacity;
};

struct EnumTable* enum_table_ctor() {

	struct EnumTable* res = calloc(1, sizeof(struct EnumTable));
	res->count_entries = 0;
	res->capacity = 10;

	res->entries = calloc(res->capacity, sizeof(struct EnumTableEntry*));

	return res;
}

void enum_table_dtor(struct EnumTable* data) {

	for (size_t i = 0; i < data->count_entries; i++) {
		struct EnumTableEntry* entry = data->entries[i];

		free(entry->name);
		free(entry);
	}
	free(data->entries);
	free(data);
}

static bool enum_table_resize(struct EnumTable* data) {

	assert(data);

	if ((data->count_entries + 1) >= data->capacity) {

		data->capacity *= 2;
		struct EnumTableEntry** new_entries = realloc(data->entries, data->capacity * sizeof(struct EnumTableEntry*));

		if (!new_entries) {
			return false;
		}

		data->entries = new_entries;

		assert(data->entries);
	}

	return true;
}

bool enum_table_insert(struct EnumTable* et, char* name, uint64_t value) {

	if (!name) {
		return false;
	}

	if (enum_table_lookup(et, name) >= 0) {
		fprintf(stderr, "Error: enum value %s was already registered\n", name);
		return false;
	}

	const size_t len = strlen(name);

	// enum members are all uppercase
	for (size_t i = 0; i < len; i++) {
		if (islower(name[i])) {
			return false;
		}
	}

	if (!enum_table_resize(et)) {
		return false;
	}

	struct EnumTableEntry* entry = malloc(sizeof(struct EnumTableEntry));

	if (entry == NULL) {
		return false;
	}

	entry->value = value;
	entry->name = strdup(name);

	if (entry->name == NULL) {
		free(entry);
		return false;
	}

	et->entries[et->count_entries++] = entry;

	return true;
}

int64_t enum_table_lookup(struct EnumTable* et, char* name) {

	assert(et);
	assert(name);

	for (size_t i = 0; i < et->count_entries; i++) {
		struct EnumTableEntry* entry = et->entries[i];

		assert(entry->name);

		if (strcmp(entry->name, name) == 0) {
			return entry->value;
		}
	}

	return -1;
}

int int_value_from_const(struct ConstValue* cv) {

	switch (cv->kind) {
		case 1: return (int)cv->ptr.m1_bool_const;
		case 2: return (int)cv->ptr.m2_int_const;
		case 3: return (int)cv->ptr.m3_char_const;
		case 5: return (int)cv->ptr.m5_hex_const;
		case 6: return (int)cv->ptr.m5_hex_const;
	}
	return -1;
}

static bool enum_table_fill_single(struct EnumTable* enum_table, struct EnumDecl* ed) {

	for (size_t j = 0; j < ed->count_members; j++) {

		struct EnumMember* em = ed->members[j];

		struct ConstValue* cv = em->value;

		const int value = int_value_from_const(cv);

		if (!enum_table_insert(enum_table, em->name, value)) {
			return false;
		}
	}

	return true;
}

bool enum_table_fill(struct EnumTable* enum_table, struct Namespace* ns, bool debug) {

	if (debug) {
		printf("[debug] filling enum tables\n");
	}

	for (size_t i = 0; i < ns->count_enums; i++) {

		struct EnumDecl* ed = ns->enums[i];

		if (debug) {
			printf("[debug] enum %s\n", ed->name);
		}

		if (!enum_table_fill_single(enum_table, ed)) {
			return false;
		}
	}

	if (debug) {
		enum_table_print(enum_table);
	}

	return true;
}

void enum_table_print(struct EnumTable* enum_table) {

	printf("Enum Table:\n");

	for (size_t i = 0; i < enum_table->count_entries; i++) {
		struct EnumTableEntry* entry = enum_table->entries[i];

		printf("%-20s = %10ld\n", entry->name, entry->value);
	}

	printf("------------------\n");
}
