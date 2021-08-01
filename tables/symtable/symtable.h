#ifndef SYMTABLE
#define SYMTABLE

#include <stdbool.h>

#include "tables/lvst/lvst.h"
#include "tables/sst/sst.h"
#include "tables/stst/stst.h"

struct ST {
	//struct SymTable
	//this structure is an indirection and abstraction
	//for the 3 main symbol tables:
	// - LVST (local variable symbol table)
	// - SST (subroutine symbol table)
	// - STST (struct symbol table)
	
	struct LVST* lvst; //may be NULL
	struct SST*   sst; //may be NULL
	struct STST* stst; //may be NULL
	
	// -----------------------------
	
	//all the type nodes that were additionally
	//constructed via type inference
	//and are not memory-managed as part of the 
	//AST Tree.

	struct Type** inferred_types;
	uint32_t      inferred_types_count;
	uint32_t      inferred_types_capacity;
};

struct ST* makeST();
void freeST(struct ST* st);

//to memory-managed all these Types created during type inference
void st_register_inferred_type(struct ST* st, struct Type* type);

#endif
