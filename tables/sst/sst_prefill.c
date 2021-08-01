#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "tables/symtable/symtable.h"
#include "ast/ast.h"

#include "transpiler/main/typeinference/util/type_str.h"

#include "transpiler/main/analyzer/halts/halts.h"

#include "tables/sst/sst.h"
#include "sst_prefill.h"

struct Proto {
	char name[DEFAULT_STR_SIZE];
	char type[DEFAULT_STR_SIZE];
	
	enum HALTS halts;
	
	bool has_side_effect;
};

static void fill_protos(
	struct ST* st, 
	char* namespace,
	struct Proto* protos, 
	int n
);

struct Proto protos_stdio[] = {
	{ "fscanf",  "int",    HALTS_UNKNOWN, true },
	{ "scanf",   "int",    HALTS_UNKNOWN, true },
	{ "sscanf",  "int",    HALTS_ALWAYS, true },
	
	{ "fprintf", "int",    HALTS_UNKNOWN, true },
	{ "printf",  "int",    HALTS_UNKNOWN, true },
	{ "sprintf", "int",    HALTS_ALWAYS, true },

	//{ "getchar", "int",    HALTS_UNKNOWN, true },
	//{ "fgetc",   "int",    HALTS_UNKNOWN, true },
};


struct Proto protos_pthread[] = {
	{ "pthread_create",  "int", HALTS_UNKNOWN, true },
	{ "pthread_join",    "int", HALTS_UNKNOWN, true },
};


void sst_prefill(struct ST *st) {
	
	//fills the SST with some basic
	//function signatures from LibC,
	//such that those functions can be used
	//with type inference
	
	const int s = sizeof(struct Proto);

	int l2 = sizeof(protos_stdio)/s;
	int l7 = sizeof(protos_pthread)/s;

	fill_protos(st, "stdio.h", protos_stdio, l2);
	fill_protos(st, "pthread.h", protos_pthread, l7);
}

static void fill_protos(
	struct ST* st, 
	char* namespace,
	struct Proto* protos, 
	int n
){

	struct Type* mi = typeFromStrPrimitive(st, "int");

	for(int i=0;i < n; i++){
		
		struct Proto proto = protos[i];
	
		char* name = proto.name;
		char* type = proto.type;
		
		struct Type* t = NULL;
		
		if(strcmp(type, "int")   == 0) { t = mi; }
		
		if(t == NULL){
			printf("[SST][Error] fill_proto\n");
			exit(1);
		}
		
		struct SSTLine* line = 
			makeSSTLine(
				name, 
				namespace, 
				t, 
				true, 
				proto.halts, 
				proto.has_side_effect,
				false
			);
		
		sst_add(st->sst, line);
	}
}
