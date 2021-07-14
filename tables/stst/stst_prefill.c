#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "tables/symtable/symtable.h"
#include "ast/ast.h"

#include "tables/stst/stst.h"
#include "stst_prefill.h"

struct ProtoStruct {
	char name[DEFAULT_STR_SIZE];
};


struct ProtoStruct protos[] = {
	{ "_IO_FILE" },
};

void stst_prefill(struct ST* st, struct STST* stst){
	
	int n = sizeof(protos)/sizeof(struct ProtoStruct);
	
	for(int i=0;i < n; i++){
		
		struct ProtoStruct proto = protos[i];
	
		struct StructType* s = make(StructType);
		
		strncpy(s->type_name, proto.name, DEFAULT_STR_SIZE);
		s->count_type_params = 0;
		
		struct SimpleType* stype = make(SimpleType);
		
		stype->struct_type    = s;
		stype->primitive_type = NULL;
		
		struct StructDecl* sd = make(StructDecl);
		
		sd->super.line_num    = 0;
		sd->super.annotations = 0;
		
		sd->type          = stype;
		sd->members       = NULL;
		sd->count_members = 0;
		
		struct STSTLine* line = makeSTSTLine(sd, "_C_");
		
		stst_add(st->stst, line);
	}
}
