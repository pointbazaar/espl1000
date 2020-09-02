#ifndef SYMTABLE
#define SYMTABLE

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
};

#endif
