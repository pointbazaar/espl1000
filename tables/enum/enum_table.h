#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "ast/ast_declare.h"
#include "tables/symtable/symtable.h"

struct EnumTable;

struct EnumTableEntry {

	// needs to be freed
	char* name;

	int64_t value;
};

struct EnumTable* enum_table_ctor();
void enum_table_dtor(struct EnumTable* data);

// @brief inserts a new entry into enum value table
// @param str   the name to enter
// @param value the value for that enum value name
bool enum_table_insert(struct EnumTable* data, char* str, int64_t value);

// @returns false if entry is not found
// @sets value_out =  integer value for the enum value name
bool enum_table_lookup(struct EnumTable* data, char* name, int64_t* value_out);

bool enum_table_fill(struct EnumTable* enum_table, struct Namespace* ns, bool debug);

void enum_table_print(struct EnumTable* enum_table);

int int_value_from_const(struct ConstValue* cv);
