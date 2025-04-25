#pragma once

#include "ast/ast.h"
#include "tables/symtable/symtable.h"

struct STST;

struct STSTLine {

	// name of the struct
	char* name;

	//where the struct was defined
	char* _namespace;

	struct StructDecl* decl;

	// Type of the struct, for convenience
	// and interoperability with functions working on types.
	// This should always be struct type
	struct Type* type;

	bool is_private;
};

// @returns NULL on error
struct STST* stst_ctor();
void stst_free(struct STST* stst);

struct STSTLine* stst_line_ctor(struct ST* st, struct StructDecl* s, char* _namespace);

void stst_fill(struct ST* st, struct Namespace* ns);

void stst_add(struct STST* stst, struct STSTLine* line);

// @returns NULL on error
struct STSTLine* stst_get(struct STST* stst, char* name);

// @returns NULL on error
struct StructMember* stst_get_member(struct STST* stst, char* struct_name, char* member_name);

// @returns type of member on success
// @returns NULL on error
struct Type* stst_member_type(struct STST* stst, const char* struct_name, const char* member_name);

// @returns true if that member exists
bool stst_has_member(struct STST* stst, const char* struct_name, const char* member_name);

//calculates the offset in bytes, from the start of the struct
// @returns < 0 on error
int32_t stst_member_offset(struct STST* stst, char* struct_name, char* member_name, bool x86);

uint32_t stst_size(struct STST* stst);

struct STSTLine* stst_at(struct STST* stst, uint32_t index);
