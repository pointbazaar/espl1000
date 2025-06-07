#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"
#include "ibuffer/ibuffer_x86.h"

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

	assert(data);

	if ((data->count_entries + 1) >= data->capacity) {
		data->capacity *= 2;
		data->entries = realloc(data->entries, data->capacity * sizeof(struct DataEntry*));

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

void data_print_escaped_str(char* dest, const char* str) {

	while (*str) {
		switch (*str) {
			case '\n': sprintf(dest, "\\n"); break;
			case '\t': sprintf(dest, "\\t"); break;
			case '\r': sprintf(dest, "\\r"); break;
			case '\b': sprintf(dest, "\\b"); break;
			case '\f': sprintf(dest, "\\f"); break;
			case '\"': sprintf(dest, "\\\""); break;
			case '\\': printf(dest, "\\\\"); break;
			default:
				if (isprint((unsigned char)*str)) {
					// Printable character -> write as-is
					*dest = *str;
				} else {
					// Non-printable character -> use hex notation
					sprintf(dest, "\\x%02X", (unsigned char)*str);
				}
				break;
		}
		str++;
		dest++;
	}
}

static bool data_write_data_segment_entry(struct DataEntry* e, struct IBuffer* ibu) {

	// The ',0' at the end NULL-terminates the string.

	char* buf = calloc(1, strlen(e->value) * 2 + strlen(e->symbol) + 40);
	if (!buf) {
		return false;
	}

	char* buf2 = calloc(1, strlen(e->value) * 2 + 40);
	if (!buf2) {
		return false;
	}

	data_print_escaped_str(buf2, e->value);

	sprintf(buf, "%s: db `%s`,0\n", e->symbol, buf2);

	nasm_db(strdup(buf));

	free(buf);
	free(buf2);

	return true;
}

bool data_write_data_segment(struct DataTable* data, struct IBuffer* ibu) {

	section(".data");

	for (size_t i = 0; i < data->count_entries; i++) {
		if (!data_write_data_segment_entry(data->entries[i], ibu)) {
			return false;
		}
	}
	return true;
}

bool data_insert(struct DataTable* data, char* str) {

	if (!str) {
		return false;
	}

	if (data_string_offset(data, str) >= 0) {
		// already have that string
		return true;
	}

	data_resize(data);

	struct DataEntry* entry = malloc(sizeof(struct DataEntry));

	if (entry == NULL) {
		return false;
	}

	entry->value = strdup(str);

	if (entry->value == NULL) {
		free(entry);
		return false;
	}

	entry->symbol = data_make_symbol(str);

	if (entry->symbol == NULL) {
		free(entry->value);
		free(entry);
		return false;
	}

	data->entries[data->count_entries++] = entry;

	return true;
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

	if (!str) {
		return -1;
	}

	for (size_t i = 0; i < data->count_entries; i++) {
		struct DataEntry* e = data->entries[i];

		if (strcmp(str, e->value) == 0) {
			return i;
		}
	}

	return -1;
}
