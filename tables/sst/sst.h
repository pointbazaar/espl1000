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
	struct Type* returnType;
	
	struct Method* method; //may be NULL 
	
	//if it is from libC
	bool isLibC;
	
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
};


//-------------
struct SST* makeSST();
void freeSST(struct SST* sst);
//-------------

void            sst_clear(struct SST* sst);

void            sst_fill(struct SST* sst, struct Namespace* ns);

void            sst_add(struct SST* sst, struct SSTLine* line);

void            sst_print(struct SST* sst);

struct SSTLine* sst_get(struct SST* sst, char* name);

bool            sst_contains(struct SST* sst, char* name);

uint32_t        sst_size(struct SST* sst);

struct SSTLine* sst_at(struct SST* sst, uint32_t index);

//-----------
struct SSTLine* makeSSTLine(
	char* name, 
	char* _namespace,
	struct Type* type, 
	bool isLibC,
	enum HALTS halts
);
struct SSTLine* makeSSTLine2(struct Method* m, char* _namespace);
void freeSSTLine(struct SSTLine* l);



#endif
