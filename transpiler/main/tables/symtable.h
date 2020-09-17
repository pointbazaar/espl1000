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
	
	// -----------------------------
	
	//all the type nodes that were additionally
	//constructed via type inference
	//and are not memory-managed as part of the 
	//AST Tree.
	
	//TODO: [ ] allocate , [ ] free
	struct Type** inferredTypes;
	unsigned int inferredTypesCount;
	unsigned int inferredTypesCapacity;
};

#endif
