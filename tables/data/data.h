#pragma once

#include <stdio.h>
#include <stdint.h>

struct DataTable;

struct DataEntry {

	// needs to be freed
	char* symbol;
	// needs to be freed
	char* value;
};

struct DataTable* data_ctor();

// @returns   the count of entries in data table
size_t data_count(struct DataTable* data);

void data_write_data_segment(struct DataTable* data, FILE* fout);

// @brief inserts a new entry into data table
// @param str   the string to enter
void data_insert(struct DataTable* data, char* str);

// @brief may return e.g. "str_hello"
// for string "hello" stored in table
// @returns NULL if entry is not found
char* data_symbol(struct DataTable* data, uint32_t offset);
