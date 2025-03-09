#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"

struct DataTable {

	struct DataEntry** entries;
	size_t count_entries;
	size_t capacity;
};

struct DataTable* data_ctor() {

	struct DataTable* res = calloc(1, sizeof(struct DataTable));
	res->count_entries = 0;
	res->capacity = 10;

	res->entries = calloc(res->capacity, sizeof(struct DataEntry*));

	return res;
}

static void data_resize(struct DataTable* data) {

	if ((data->count_entries + 1) >= data->capacity) {
		data->capacity *= 2;
		data->entries = realloc(data->entries, data->capacity);

		assert(data->entries);
	}
}

// @brief Turns any string into something
//        that can be used as a label in the assembly.
//        The result needs to be freed.
// @param str   any string
static char* data_make_symbol(char* str) {

	const size_t len = strlen(str);
	char* res = calloc(len + 1, sizeof(char));

	sprintf(res, "str_");
	int j = strlen(res);

	for (int i = 0; i < len; i++) {
		char c = str[i];
		if (isalnum(c)) {
			res[j++] = c;
		} else {
			//TODO: make this bijective to the actual string.
			//Currently, this would collapse "ab?" and "ab*"
			//to the same symbol
			res[j++] = '_';
		}
	}
	return res;
}

size_t data_count(struct DataTable* data) {
	return data->count_entries;
}

void data_write_data_segment(struct DataTable* data, FILE* fout) {

	fprintf(fout, "section .data\n");

	for (size_t i = 0; i < data->count_entries; i++) {
		struct DataEntry* e = data->entries[i];
		size_t len = strlen(e->value);

		fprintf(fout, "%s: db \"%s\", %ld\n", e->symbol, e->value, len);
	}
}

void data_insert(struct DataTable* data, char* str) {

	data_resize(data);

	struct DataEntry* entry = malloc(sizeof(struct DataEntry));

	entry->value = strdup(str);
	entry->symbol = data_make_symbol(str);

	data->entries[data->count_entries++] = entry;
}

char* data_symbol(struct DataTable* data, uint32_t offset) {

	assert(data);

	if (offset >= data->count_entries) {
		fprintf(stderr, "%s:%s: could not find symbol for offset %d\n", __FILE__, __func__, offset);
		return NULL;
	}

	return data->entries[offset]->symbol;
}
