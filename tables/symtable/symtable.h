#pragma once

#include <stdbool.h>

#include "tables/lvst/lvst.h"
#include "tables/sst/sst.h"
#include "tables/stst/stst.h"
#include "tables/data/data.h"
#include "tables/enum/enum_table.h"

struct ST {
	//struct SymTable
	//this structure is an indirection and abstraction
	//for the 3 main symbol tables:
	// - LVST (local variable symbol table)
	// - SST (subroutine symbol table)
	// - STST (struct symbol table)

	struct LVST* lvst; //may be NULL
	struct SST* sst; //may be NULL
	struct STST* stst; //may be NULL

	// things which should be in .data segment
	struct DataTable* data;

	struct EnumTable* enum_table;

	// -----------------------------

	//all the type nodes that were additionally
	//constructed via type inference
	//and are not memory-managed as part of the
	//AST Tree.

	struct Type** inferred_types;
	uint32_t inferred_types_count;
	uint32_t inferred_types_capacity;
};

// @returns NULL on error
struct ST* st_ctor();
void st_free(struct ST* st);

//to memory-managed all these Types created during type inference
void st_register_inferred_type(struct ST* st, struct Type* type);
