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

void data_dtor(struct DataTable* data) {

	for (size_t i = 0; i < data->count_entries; i++) {
		struct DataEntry* entry = data->entries[i];

		free(entry->symbol);
		free(entry->value);
		free(entry);
	}
	free(data->entries);
	free(data);
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
static char* data_make_symbol(const char* str) {

	const size_t len = strlen(str);

	char* res = calloc(len * 3 + 10, sizeof(char));
	if (!res) return NULL;

	sprintf(res, "str_");
	size_t j = strlen(res);

	for (size_t i = 0; i < len; i++) {
		char c = str[i];
		if (isalnum(c)) {
			res[j++] = c;
		} else if (c == '_') {
			res[j++] = '_';
		} else {
			// Add '$' to signal that an escape code follows
			sprintf(res + j, "$%02X", (unsigned char)c);
			j += 3;
		}
	}

	return res;
}

size_t data_count(struct DataTable* data) {
	return data->count_entries;
}

void data_print_escaped_str(FILE* fout, const char* str) {
	while (*str) {
		switch (*str) {
			case '\n': fprintf(fout, "\\n"); break;
			case '\t': fprintf(fout, "\\t"); break;
			case '\r': fprintf(fout, "\\r"); break;
			case '\b': fprintf(fout, "\\b"); break;
			case '\f': fprintf(fout, "\\f"); break;
			case '\"': fprintf(fout, "\\\""); break;
			case '\\': fprintf(fout, "\\\\"); break;
			default:
				if (isprint((unsigned char)*str)) {
					// Printable character -> write as-is
					fputc(*str, fout);
				} else {
					// Non-printable character -> use hex notation
					fprintf(fout, "\\x%02X", (unsigned char)*str);
				}
				break;
		}
		str++;
	}
}

static void data_write_data_segment_entry(struct DataEntry* e, FILE* fout) {

	// The ',0' at the end NULL-terminates the string.

	fprintf(fout, "%s: db ", e->symbol);
	fprintf(fout, "`");
	data_print_escaped_str(fout, e->value);
	fprintf(fout, "`");
	fprintf(fout, ",0\n");
}

void data_write_data_segment(struct DataTable* data, FILE* fout) {

	fprintf(fout, "section .data\n");

	for (size_t i = 0; i < data->count_entries; i++) {
		data_write_data_segment_entry(data->entries[i], fout);
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

int32_t data_string_offset(struct DataTable* data, char* str) {

	for (size_t i = 0; i < data->count_entries; i++) {
		struct DataEntry* e = data->entries[i];

		if (strcmp(str, e->value) == 0) {
			return i;
		}
	}

	return -1;
}
