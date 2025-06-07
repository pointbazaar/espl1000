#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

struct IBuffer;

struct DataTable;

struct DataEntry {

	// needs to be freed
	char* symbol;
	// needs to be freed
	char* value;
};

struct DataTable* data_ctor();
void data_dtor(struct DataTable* data);

// @returns   the count of entries in data table
size_t data_count(struct DataTable* data);

// @returns true on success
bool data_write_data_segment(struct DataTable* data, struct IBuffer* ibu);

// @brief inserts a new entry into data table
// @param str   the string to enter
bool data_insert(struct DataTable* data, char* str);

// @brief may return e.g. "str_hello"
// for string "hello" stored in table
// @returns NULL if entry is not found
char* data_symbol(struct DataTable* data, uint32_t offset);

// @brief returns offset of string in data table
// @param str     The string from 'struct StringConst'
// @returns < 0   on error
int32_t data_string_offset(struct DataTable* data, char* str);
