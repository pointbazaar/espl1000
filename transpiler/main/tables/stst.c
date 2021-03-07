#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../../../ast/ast.h"

#include "tables/stst.h"


struct STST* makeStructSymTable2(bool debug){
	
	if(debug){ printf("makeStructSymTable2(...)\n"); }
	
	struct STST* stst = malloc(sizeof(struct STST));
	
	stst->capacity = 10;
	stst->lines = malloc(sizeof(struct STSTLine*) * stst->capacity);
	stst->count = 0;
	
	return stst;
}

struct STST* makeStructSymTable(struct Namespace* ns, bool debug){
	
	if(debug){ printf("makeStructSymTable(...)\n"); }
	
	struct STST* stst = makeStructSymTable2(debug);
	
	for(int i=0;i < ns->count_structs; i++){

		struct StructDecl* mystruct = ns->structs[i];

		stst_add(stst, mystruct);
	}

	if(debug){
		
		printf("Struct Symbol Table (STST)\n");
		printf("%8s|\n", "name");
		printf("--------|--------\n");
		for(int i = 0; i < stst->count; i++){
			struct STSTLine* line = stst->lines[i];
			
			printf("%8s|\n", line->name);
		}
		
	}

	
	return stst;
}

void freeStructSymTable(struct STST* stst){
	
	for(int i=0;i < stst->count; i++){

		free(stst->lines[i]);
	}
	free(stst->lines);
	
	free(stst);
}

void stst_add(struct STST* stst, struct StructDecl* s){

	struct STSTLine* line = malloc(sizeof(struct STSTLine));

	line->decl = s;
	line->type = s->type;
	strcpy(line->name, s->type->typeName);
	
	if(stst->capacity <= stst->count){
		
		stst->capacity *= 2;
		stst->lines = realloc(stst->lines, sizeof(struct STSTLine*) * stst->capacity);
	}

	stst->lines[stst->count] = line;
	stst->count += 1;
}

struct Type* stst_get_member_type(struct STST* stst, char* struct_name, char* member_name){

	for(int i=0;i < stst->count; i++){
		
		struct STSTLine* line = stst->lines[i];

		if(strcmp(line->name, struct_name) != 0){
			continue;
		}

		for(int j=0; j < line->decl->count_members; j++){

			struct StructMember* member = line->decl->members[i];

			if(strcmp(member->name, member_name) == 0){

				return member->type;
			}
		}
	}
	
	char* msg = "Error: stst_get_member_type : could not find type of member %s of %s\n";
	printf(msg, struct_name, member_name);

	exit(1);
}
