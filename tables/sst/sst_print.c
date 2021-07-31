#include <stdio.h>
#include <stdlib.h>

#include "ast/util/str_ast.h"

#include "sst.h"
#include "sst_print.h"

static void sst_print_line(struct SSTLine* line, char* fmt);

void sst_print(struct SST* sst){
	
	char* fmt = "%-20s|%-20s|%30s|%6s|%8s\n";
	
	printf("[SST] Subroutine Symbol Table\n");
	printf("----------------------------------------------------------------------------------------\n");
	printf(fmt, "namespace",             "name",            "type / return type",         "is_libc","halts?");
	printf("--------------------+--------------------+------------------------------+------+--------\n");
	
	for(int i = 0; i < sst_size(sst); i++){
		
		struct SSTLine* line = sst_at(sst, i);	
		sst_print_line(line, fmt);
	}
	
	printf("----------------------------------------------------------------------------------------\n");
}

static void sst_print_line(struct SSTLine* line, char* fmt){
	
	char* isLibC = (line->is_libc) ? "yes" : "no";
		
	char* halt_info = "-";
	
	if(line->halts != HALTS_UNKNOWN){
			
		halt_info = (line->halts == HALTS_ALWAYS)?"halts":"inf-loop";
	}
	
	char* typeStr = "";
	
	if(line->type != NULL){ 
		typeStr = str_type(line->type);
	}else if(line->return_type != NULL){
		typeStr = str_type(line->return_type);
	}
	
	printf(fmt, line->_namespace, line->name, typeStr, isLibC, halt_info);
	
	if(line->type != NULL){ 
		free(typeStr); 
	}
}
