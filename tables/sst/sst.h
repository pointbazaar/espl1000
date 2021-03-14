#ifndef SST_H
#define SST_H

#include <stdbool.h>

#include "ast/ast.h"

#include "tables/cc/cc.h"
#include "tables/symtable/symtable.h"

struct SSTLine {
	//Subroutine Symbol Table Line
	
	//the name of the subroutine
	char name[DEFAULT_STR_SIZE]; 
	
	//a reference to the type being returned
	struct Type* returnType; 
	
	//if it is from libC
	bool isLibC;
	
	//callees and callers
	struct CC* cc; //may be NULL
	
	//--- dead code analysis ---
	bool is_dead;
	bool dead_visited;
	//--------------------------
};

struct SST {
	//Subroutine Symbol Table (SST)
	
	unsigned int count;
	size_t capacity;
	
	struct SSTLine** lines;
};

//-------------
struct SST* makeSST();
void freeSST(struct SST* sst);
//-------------
void sst_clear(struct SST* sst);

void sst_fill(struct SST* sst, struct Namespace* ns, bool debug);

void sst_add(struct SST* sst, struct SSTLine* line);

void sst_print(struct SST* sst);

struct SSTLine* sst_get(struct SST* sst, char* name);

bool sst_contains(struct SST* sst, char* name);

//-----------
struct SSTLine* makeSSTLine(char* name, struct Type* type, bool isLibC);
void freeSSTLine(struct SSTLine* l);



#endif
