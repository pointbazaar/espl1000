#ifndef SYMTABLE
#define SYMTABLE

struct ST {
	
	//struct SymTable
	//this structure is an indirection and abstraction
	//for the 3 main symbol tables:
	// - LVST (local variable symbol table)
	// - SST (subroutine symbol table)
	// - STST (struct symbol table)
	
	struct LVST* lvst;
	struct SST* sst;
	struct STST* stst;
};

#endif
