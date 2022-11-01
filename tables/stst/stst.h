#ifndef STRUCTSYMTABLE
#define STRUCTSYMTABLE

#include "ast/ast.h"

struct STST;

struct STSTLine {
	
	char name[DEFAULT_STR_SIZE];
	
	//where the struct was defined
	char _namespace[DEFAULT_STR_SIZE];

	struct StructDecl* decl;
	
	bool is_private;
};


struct STST* makeSTST();
void freeSTST(struct STST* stst);

struct STSTLine* makeSTSTLine(struct StructDecl* s, char* _namespace);

void                 stst_fill(struct STST* stst, struct Namespace* ns);

void                 stst_add(struct STST* stst, struct STSTLine* line);

struct STSTLine*     stst_get(struct STST* stst, char* name);

struct StructMember* stst_get_member(struct STST* stst, char* struct_name, char* member_name);

//calculates the offset in bytes, from the start of the struct
uint32_t 			 stst_member_offset(struct STST* stst, char* struct_name, char* member_name);

uint32_t             stst_size(struct STST* stst);

struct STSTLine*     stst_at(struct STST* stst, uint32_t index);

#endif
