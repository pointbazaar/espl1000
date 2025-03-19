#pragma once

#include <stdbool.h>

struct CC;

#include "ast/ast_declare.h"

#include "compiler/main/analyzer/halts/halts.h"
#include "compiler/main/analyzer/dead/dead.h"

struct SSTLine {
	//Subroutine Symbol Table Line

	//the name of the subroutine
	char* name;

	//where the subroutine was defined
	char* _namespace;

	//a reference to the type being returned
	struct Type* return_type;

	//type of the method (SubrType underneath)
	struct Type* type; //may be NULL (depends on method)
	struct Method* method; //may be NULL

	//if it allocates, frees,
	//writes to memory, opens file, ...
	bool has_side_effect;

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

	bool is_syscall;
};

//-------------
// @returns NULL on error
struct SST* sst_ctor();
void sst_free(struct SST* sst);
//-------------

void sst_clear(struct SST* sst);

// @returns false on error
bool sst_add(struct SST* sst, struct SSTLine* line);

// @returns NULL on error
struct SSTLine* sst_get(struct SST* sst, const char* name);

bool sst_contains(struct SST* sst, const char* name);

uint32_t sst_size(struct SST* sst);

// @returns NULL on error
struct SSTLine* sst_at(struct SST* sst, uint32_t index);

// @returns < 0 on error
int32_t sst_index_of(struct SST* sst, const char* name);

// @returns size of arguments in bytes
uint32_t sst_args_size_avr(struct SST* sst, char* name);
// @returns size of arguments in bytes
uint32_t sst_args_size_x86(struct SST* sst, char* name);

// @returns the count of arguments to 'name' function
uint32_t sst_args_count(struct SST* sst, char* name);

//-----------
struct SSTLine* sst_line_ctor(
    char* name,
    char* _namespace,
    struct Type* return_type,
    bool isLibC,
    enum HALTS halts,
    bool has_side_effect);

struct SSTLine* sst_line_ctor2(
    struct Method* m,
    struct Type* type,
    char* _namespace);

void sst_line_free(struct SSTLine* l);
