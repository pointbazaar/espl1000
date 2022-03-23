#ifndef SST_H
#define SST_H

#include <stdbool.h>

struct CC;

#include "ast/ast_declare.h"

#include "transpiler/main/analyzer/halts/halts.h"
#include "transpiler/main/analyzer/dead/dead.h"

struct SSTLine {
	//Subroutine Symbol Table Line
	
	//the name of the subroutine
	char name[DEFAULT_STR_SIZE]; 
	
	//where the subroutine was defined
	char _namespace[DEFAULT_STR_SIZE];
	
	//a reference to the type being returned
	struct Type* return_type;
	
	//type of the method (SubrType underneath)
	struct Type* type;     //may be NULL (depends on method)
	struct Method* method; //may be NULL 
	
	//if it allocates, frees,
	//writes to memory, opens file, ...
	bool has_side_effect;
	
	//if it is from libC
	bool is_libc;

	bool is_extern_c;
	
	//callees and callers
	struct CC* cc; //may be NULL
	
	//--- dead code analysis ---
	//bool is_dead;
	//bool dead_known;
	enum DEAD dead;
	//--------------------------
	
	//-- termination analysis --
	enum HALTS halts;
	//--------------------------
	
	//-- encapsulation
	bool is_private;

    //maybe NULL, this is used if the function has a different
	//name in c source code, possibly as a result of 'externc' usage
	char* name_in_c;
};


//-------------
struct SST* makeSST();
void freeSST(struct SST* sst);
//-------------

void            sst_clear(struct SST* sst);

void            sst_add(struct SST* sst, struct SSTLine* line);

struct SSTLine* sst_get(struct SST* sst, char* name);

bool            sst_contains(struct SST* sst, char* name);

uint32_t        sst_size(struct SST* sst);

struct SSTLine* sst_at(struct SST* sst, uint32_t index);

//-----------
struct SSTLine* makeSSTLine(
	char* name, 
	char* _namespace,
	struct Type* return_type,
	bool isLibC,
	enum HALTS halts,
	bool has_side_effect
);

struct SSTLine* makeSSTLine2(
	struct Method* m, 
	struct Type* type,
	char* _namespace
);

void freeSSTLine(struct SSTLine* l);

#endif
