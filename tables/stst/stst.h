#ifndef STRUCTSYMTABLE
#define STRUCTSYMTABLE

#include "ast/ast.h"

struct STST;

struct STSTLine {
	
	char name[DEFAULT_STR_SIZE];

	struct StructDecl* decl;
};


struct STST* makeSTST();
void freeSTST(struct STST* stst);

void stst_fill(struct STST* stst, struct Namespace* ns);

void stst_clear(struct STST* stst);

void stst_add(struct STST* stst, struct StructDecl* s);

void stst_print(struct STST* stst);

struct Type* stst_get_member_type(struct STST* stst, char* struct_name, char* member_name);

#endif
