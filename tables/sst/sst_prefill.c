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

//TODO: find trustworthy sources
//to see which of these functions 
//are guaranteed to terminate

struct Proto protos_math[] = {
	{ "sin", "float",   HALTS_ALWAYS, false },
	{ "cos", "float",   HALTS_ALWAYS, false },
	{ "tan", "float",   HALTS_ALWAYS, false },
	
	{ "asin", "float",  HALTS_ALWAYS, false },
	{ "acos", "float",  HALTS_ALWAYS, false },
	{ "atan", "float",  HALTS_ALWAYS, false },
	
	{ "sinh", "float",  HALTS_ALWAYS, false },
	{ "cosh", "float",  HALTS_ALWAYS, false },
	{ "tanh", "float",  HALTS_ALWAYS, false },
	
	{ "exp", "float",   HALTS_ALWAYS, false },
	{ "log", "float",   HALTS_ALWAYS, false },
	{ "log10", "float", HALTS_ALWAYS, false },
	
	{ "pow", "float",   HALTS_ALWAYS, false },
	{ "sqrt", "float",  HALTS_ALWAYS, false },
	
	{ "ceil", "float",  HALTS_ALWAYS, false },
	{ "floor", "float", HALTS_ALWAYS, false },
	
	{ "fabs", "float",  HALTS_ALWAYS, false },
	
	{ "fmin", "float",  HALTS_ALWAYS, false },
	{ "fmax", "float",  HALTS_ALWAYS, false },

	{ "round", "int",   HALTS_ALWAYS, false },
};

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
	
	{ "fgetpos", "int",    HALTS_UNKNOWN, true },
	{ "fseek",   "int",    HALTS_UNKNOWN, true },
	{ "fsetpos", "int",    HALTS_UNKNOWN, true },
	{ "ftell",   "int",    HALTS_UNKNOWN, true },
	{ "rewind",  "int",    HALTS_UNKNOWN, true }, //TODO: wrong return type
	
	{ "fopen",   "FILE", HALTS_UNKNOWN, true },
	{ "freopen", "FILE", HALTS_UNKNOWN, true },
	{ "fflush",  "int",    HALTS_UNKNOWN, true },
	{ "fclose",  "int",    HALTS_UNKNOWN, true },
	
	{ "remove",  "int",    HALTS_UNKNOWN, true },
	{ "rename",  "int",    HALTS_UNKNOWN, true },
};

struct Proto protos_stdlib[] = {
	{ "atof",   "float",  HALTS_ALWAYS, true },
	{ "atoi",   "int",    HALTS_ALWAYS, true },
	{ "rand",   "int",    HALTS_ALWAYS, true },
	{ "srand",  "int",    HALTS_ALWAYS, true }, //TODO: wrong return type
	{ "calloc", "int",    HALTS_ALWAYS, true }, //TODO: wrong return type
	{ "malloc", "int",    HALTS_ALWAYS, true }, //TODO: wrong return type
	{ "free",   "int",    HALTS_ALWAYS, true }, //TODO: wrong return type
	{ "atexit", "int",    HALTS_ALWAYS, true },
	{ "exit",   "int",    HALTS_ALWAYS, true }, //TODO: wrong return type
	{ "getenv", "[char]", HALTS_UNKNOWN, true },
	{ "system", "int",    HALTS_UNKNOWN, true },
	//bsearch
	//qsort
	{ "abs",    "int",    HALTS_ALWAYS, true },
	//div
};

struct Proto protos_string[] = {
	{ "strcmp",   "int",    HALTS_UNKNOWN, false },
	{ "strncmp",  "int",    HALTS_ALWAYS, false },
	
	{ "strlen",   "int",    HALTS_UNKNOWN, false },
	{ "strnlen",  "int",    HALTS_ALWAYS, false },
	
	{ "strcpy",   "[char]", HALTS_UNKNOWN, true },
	{ "strncpy",  "[char]", HALTS_ALWAYS, true },
	
	{ "strcat",   "[char]", HALTS_UNKNOWN, true },
	{ "strncat",  "[char]", HALTS_ALWAYS, true },
	
	{ "strchr",   "[char]", HALTS_UNKNOWN, false },
	{ "strrchr",  "[char]", HALTS_UNKNOWN, false },
	
	{ "strspn",   "int",    HALTS_UNKNOWN, true },
	{ "strcspn",  "int",    HALTS_UNKNOWN, true },
	
	{ "strpbrk",  "[char]", HALTS_UNKNOWN, true },
	{ "strstr",   "[char]", HALTS_UNKNOWN, true },
	
	{ "basename", "[char]", HALTS_UNKNOWN, true },
	
	{ "strerror", "[char]", HALTS_UNKNOWN, true },
	
	{ "strtok",   "[char]", HALTS_UNKNOWN, true },
	
	{ "memcpy",   "int", HALTS_UNKNOWN, true },
};

struct Proto protos_ctype[] = {
	{ "isalnum",  "int", HALTS_ALWAYS, false },
	{ "isalpha",  "int", HALTS_ALWAYS, false },
	{ "isblank",  "int", HALTS_ALWAYS, false },
	{ "iscntrl",  "int", HALTS_ALWAYS, false },
	{ "isdigit",  "int", HALTS_ALWAYS, false },
	{ "isgraph",  "int", HALTS_ALWAYS, false },
	{ "islower",  "int", HALTS_ALWAYS, false },
	{ "isprint",  "int", HALTS_ALWAYS, false },
	{ "ispunct",  "int", HALTS_ALWAYS, false },
	{ "isspace",  "int", HALTS_ALWAYS, false },
	{ "isupper",  "int", HALTS_ALWAYS, false },
	{ "isxdigit", "int", HALTS_ALWAYS, false },
	
	{ "tolower",  "int", HALTS_ALWAYS, false },
	{ "toupper",  "int", HALTS_ALWAYS, false },
};

struct Proto protos_assert[] = {
	{ "assert", "int", HALTS_ALWAYS, true }, //TODO: wrong return type
};

struct Proto protos_pthread[] = {
	{ "pthread_create",  "int", HALTS_UNKNOWN, true },
	{ "pthread_join",    "int", HALTS_UNKNOWN, true },
};


struct Proto protos_time[] = {
	//TODO: it is probably not int !
	//fix that!
	{ "time",  "int", HALTS_ALWAYS, true }, 
};

void sst_prefill(struct ST* st, struct SST* sst){
	
	//fills the SST with some basic
	//function signatures from LibC,
	//such that those functions can be used
	//with type inference
	
	const int s = sizeof(struct Proto);
	
	int l1 = sizeof(protos_math)/s;
	int l2 = sizeof(protos_stdio)/s;
	int l3 = sizeof(protos_stdlib)/s;
	int l4 = sizeof(protos_string)/s;
	int l5 = sizeof(protos_ctype)/s;
	int l6 = sizeof(protos_assert)/s;
	int l7 = sizeof(protos_pthread)/s;
	int l8 = sizeof(protos_time)/s;
	
	fill_protos(st, "math.h", protos_math, l1);
	fill_protos(st, "stdio.h", protos_stdio, l2);
	fill_protos(st, "stdlib.h", protos_stdlib, l3);
	fill_protos(st, "string.h", protos_string, l4);
	fill_protos(st, "ctype.h", protos_ctype, l5);
	fill_protos(st, "assert.h", protos_assert, l6);
	fill_protos(st, "pthread.h", protos_pthread, l7);
	fill_protos(st, "time.h", protos_time, l8);
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
