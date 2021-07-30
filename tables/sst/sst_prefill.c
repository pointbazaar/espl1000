#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "tables/symtable/symtable.h"
#include "ast/ast.h"

//TODO: get rid of this include,
//as sst_prefill should not depend on type
//type_str
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

static struct Type* makeCharPtrTypeC();
static struct Type* makeFileTypeC();

struct Proto protos_stdio[] = {
	{ "fscanf",  "int",    HALTS_UNKNOWN, true },
	{ "scanf",   "int",    HALTS_UNKNOWN, true },
	{ "sscanf",  "int",    HALTS_ALWAYS, true },
	
	{ "fprintf", "int",    HALTS_UNKNOWN, true },
	{ "printf",  "int",    HALTS_UNKNOWN, true },
	{ "sprintf", "int",    HALTS_ALWAYS, true },
	
	{ "fgetc",   "int",    HALTS_UNKNOWN, true },
	{ "fgets",   "[char]", HALTS_UNKNOWN, true },
	{ "fputc",   "int",    HALTS_UNKNOWN, true },
	{ "fputs",   "int",    HALTS_UNKNOWN, true },
	{ "getc",    "int",    HALTS_UNKNOWN, true },
	{ "getchar", "int",    HALTS_UNKNOWN, true },
	{ "putc",    "int",    HALTS_UNKNOWN, true },
	{ "putchar", "int",    HALTS_UNKNOWN, true },
	{ "puts",    "int",    HALTS_UNKNOWN, true },
	{ "ungetc",  "int",    HALTS_UNKNOWN, true },
	
	{ "fread",   "int",    HALTS_UNKNOWN, true },
	{ "fwrite",  "int",    HALTS_UNKNOWN, true },

};


struct Proto protos_pthread[] = {
	{ "pthread_create",  "int", HALTS_UNKNOWN, true },
	{ "pthread_join",    "int", HALTS_UNKNOWN, true },
};


void sst_prefill(struct ST* st, struct SST* sst){
	
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

	struct Type* mf = typeFromStrPrimitive(st, "float");
	struct Type* mi = typeFromStrPrimitive(st, "int");
	struct Type* mc = typeFromStrPrimitive(st, "char");
	
	struct Type* cp = makeCharPtrTypeC();
	struct Type* fp = makeFileTypeC();
	
	st_register_inferred_type(st, fp);	
	st_register_inferred_type(st, cp);

	for(int i=0;i < n; i++){
		
		struct Proto proto = protos[i];
	
		char* name = proto.name;
		char* type = proto.type;
		
		struct Type* t = NULL;
		
		if(strcmp(type, "int")   == 0) { t = mi; }
		if(strcmp(type, "float") == 0) { t = mf; }
		if(strcmp(type, "char") == 0)  { t = mc; }
		if(strcmp(type, "[char]") == 0){ t = cp; }
		if(strcmp(type, "FILE") == 0)  { t = fp; }
		
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

static struct Type* makeCharPtrTypeC(){
	
	struct PrimitiveType* pt = make(PrimitiveType);
	
	pt->is_int_type  = false;
	pt->is_char_type = true;
	pt->is_float_type= false;
	pt->is_bool_type = false;
	
	struct SimpleType* myst = make(SimpleType);
	myst->primitive_type = pt;
	myst->struct_type    = NULL;
	
	struct BasicType* btw = make(BasicType);
	btw->simple_type = myst;
	btw->subr_type = NULL;
	
	struct Type* mychar = make(Type);
		mychar->m1 = btw;
		mychar->m2 = NULL;
		mychar->m3 = NULL;
		
	struct ArrayType* at = make(ArrayType);
		at->element_type = mychar;
		
	struct Type* cp = make(Type);
		cp->m1 = NULL;
		cp->m2 = NULL;
		cp->m3 = at;
		
	return cp;
}

static struct Type* makeFileTypeC(){
	
	struct StructType* s = make(StructType);
	
	strcpy(s->type_name, "_IO_FILE");
	s->count_type_params = 0;
	s->type_params     = NULL;
	
	struct SimpleType* myst = make(SimpleType);
	myst->primitive_type = NULL;
	myst->struct_type    = s;
	
	struct BasicType* btw = make(BasicType);
	btw->simple_type = myst;
	btw->subr_type = NULL;
	
	struct Type* m = make(Type);
		m->m1 = btw;
		m->m2 = NULL;
		m->m3 = NULL;
		
	return m;
}
