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
};

static void fill_protos(struct ST* st, struct Proto* protos, int n);

static struct Type* makeCharPtrTypeC();
static struct Type* makeFileTypeC();

//TODO: find trustworthy sources
//to see which of these functions 
//are guaranteed to terminate

struct Proto protos_math[] = {
	{ "sin", "float",   HALTS_ALWAYS },
	{ "cos", "float",   HALTS_ALWAYS },
	{ "tan", "float",   HALTS_ALWAYS },
	
	{ "asin", "float",  HALTS_ALWAYS },
	{ "acos", "float",  HALTS_ALWAYS },
	{ "atan", "float",  HALTS_ALWAYS },
	
	{ "sinh", "float",  HALTS_ALWAYS },
	{ "cosh", "float",  HALTS_ALWAYS },
	{ "tanh", "float",  HALTS_ALWAYS },
	
	{ "exp", "float",   HALTS_ALWAYS },
	{ "log", "float",   HALTS_ALWAYS },
	{ "log10", "float", HALTS_ALWAYS },
	
	{ "pow", "float",   HALTS_ALWAYS },
	{ "sqrt", "float",  HALTS_ALWAYS },
	
	{ "ceil", "float",  HALTS_ALWAYS },
	{ "floor", "float", HALTS_ALWAYS },
	
	{ "fabs", "float",  HALTS_ALWAYS },
	
	{ "fmin", "float",  HALTS_ALWAYS },
	{ "fmax", "float",  HALTS_ALWAYS },

	{ "round", "int",   HALTS_ALWAYS },
};

struct Proto protos_stdio[] = {
	{ "fscanf",  "int",    HALTS_UNKNOWN },
	{ "scanf",   "int",    HALTS_UNKNOWN },
	{ "sscanf",  "int",    HALTS_ALWAYS },
	
	{ "fprintf", "int",    HALTS_UNKNOWN },
	{ "printf",  "int",    HALTS_UNKNOWN },
	{ "sprintf", "int",    HALTS_ALWAYS },
	
	{ "fgetc",   "int",    HALTS_UNKNOWN },
	{ "fgets",   "[char]", HALTS_UNKNOWN },
	{ "fputc",   "int",    HALTS_UNKNOWN },
	{ "fputs",   "int",    HALTS_UNKNOWN },
	{ "getc",    "int",    HALTS_UNKNOWN },
	{ "getchar", "int",    HALTS_UNKNOWN },
	{ "putc",    "int",    HALTS_UNKNOWN },
	{ "putchar", "int",    HALTS_UNKNOWN },
	{ "puts",    "int",    HALTS_UNKNOWN },
	{ "ungetc",  "int",    HALTS_UNKNOWN },
	
	{ "fread",   "int",    HALTS_UNKNOWN },
	{ "fwrite",  "int",    HALTS_UNKNOWN },
	
	{ "fgetpos", "int",    HALTS_UNKNOWN },
	{ "fseek",   "int",    HALTS_UNKNOWN },
	{ "fsetpos", "int",    HALTS_UNKNOWN },
	{ "ftell",   "int",    HALTS_UNKNOWN },
	{ "rewind",  "int",    HALTS_UNKNOWN }, //TODO: wrong return type
	
	{ "fopen",   "FILE", HALTS_UNKNOWN },
	{ "freopen", "FILE", HALTS_UNKNOWN },
	{ "fflush",  "int",    HALTS_UNKNOWN },
	{ "fclose",  "int",    HALTS_UNKNOWN },
	
	{ "remove",  "int",    HALTS_UNKNOWN },
	{ "rename",  "int",    HALTS_UNKNOWN },
};

struct Proto protos_stdlib[] = {
	{ "atof",   "float",  HALTS_ALWAYS },
	{ "atoi",   "int",    HALTS_ALWAYS },
	{ "rand",   "int",    HALTS_ALWAYS },
	{ "srand",  "int",    HALTS_ALWAYS }, //TODO: wrong return type
	{ "calloc", "int",    HALTS_ALWAYS }, //TODO: wrong return type
	{ "malloc", "int",    HALTS_ALWAYS }, //TODO: wrong return type
	{ "free",   "int",    HALTS_ALWAYS }, //TODO: wrong return type
	{ "atexit", "int",    HALTS_ALWAYS },
	{ "exit",   "int",    HALTS_ALWAYS }, //TODO: wrong return type
	{ "getenv", "[char]", HALTS_UNKNOWN },
	{ "system", "int",    HALTS_UNKNOWN },
	//bsearch
	//qsort
	{ "abs",    "int",    HALTS_ALWAYS },
	//div
};

struct Proto protos_string[] = {
	{ "strcmp",   "int",    HALTS_UNKNOWN },
	{ "strncmp",  "int",    HALTS_ALWAYS },
	
	{ "strlen",   "int",    HALTS_UNKNOWN },
	{ "strnlen",  "int",    HALTS_ALWAYS },
	
	{ "strcpy",   "[char]", HALTS_UNKNOWN },
	{ "strncpy",  "[char]", HALTS_ALWAYS },
	
	{ "strcat",   "[char]", HALTS_UNKNOWN },
	{ "strncat",  "[char]", HALTS_ALWAYS },
	
	{ "strchr",   "[char]", HALTS_UNKNOWN },
	{ "strrchr",  "[char]", HALTS_UNKNOWN },
	
	{ "strspn",   "int",    HALTS_UNKNOWN },
	{ "strcspn",  "int",    HALTS_UNKNOWN },
	
	{ "strpbrk",  "[char]", HALTS_UNKNOWN },
	{ "strstr",   "[char]", HALTS_UNKNOWN },
	
	{ "basename", "[char]", HALTS_UNKNOWN },
	
	{ "strerror", "[char]", HALTS_UNKNOWN },
	
	{ "strtok",   "[char]", HALTS_UNKNOWN },
};

struct Proto protos_ctype[] = {
	{ "isalnum",  "int", HALTS_ALWAYS },
	{ "isalpha",  "int", HALTS_ALWAYS },
	{ "isblank",  "int", HALTS_ALWAYS },
	{ "iscntrl",  "int", HALTS_ALWAYS },
	{ "isdigit",  "int", HALTS_ALWAYS },
	{ "isgraph",  "int", HALTS_ALWAYS },
	{ "islower",  "int", HALTS_ALWAYS },
	{ "isprint",  "int", HALTS_ALWAYS },
	{ "ispunct",  "int", HALTS_ALWAYS },
	{ "isspace",  "int", HALTS_ALWAYS },
	{ "isupper",  "int", HALTS_ALWAYS },
	{ "isxdigit", "int", HALTS_ALWAYS },
	
	{ "tolower",  "int", HALTS_ALWAYS },
	{ "toupper",  "int", HALTS_ALWAYS },
};

struct Proto protos_assert[] = {
	{ "assert", "int", HALTS_ALWAYS }, //TODO: wrong return type
};

struct Proto protos_pthread[] = {
	{ "pthread_create",  "int", HALTS_UNKNOWN },
	{ "pthread_join",    "int", HALTS_UNKNOWN },
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
	
	fill_protos(st, protos_math, l1);
	fill_protos(st, protos_stdio, l2);
	fill_protos(st, protos_stdlib, l3);
	fill_protos(st, protos_string, l4);
	fill_protos(st, protos_ctype, l5);
	fill_protos(st, protos_assert, l6);
	fill_protos(st, protos_pthread, l7);
}

static void fill_protos(
	struct ST* st, 
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
			makeSSTLine(name, "_C_", t, true, proto.halts);
		
		sst_add(st->sst, line);
	}
}

static struct Type* makeCharPtrTypeC(){
	
	struct PrimitiveType* pt = make(PrimitiveType);
	
	pt->isIntType  = false;
	pt->isCharType = true;
	pt->isFloatType= false;
	pt->isBoolType = false;
	
	struct SimpleType* myst = make(SimpleType);
	myst->primitiveType = pt;
	myst->structType    = NULL;
	
	struct BasicType* btw = make(BasicType);
	btw->simpleType = myst;
	btw->subrType = NULL;
	
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
	
	strcpy(s->typeName, "_IO_FILE");
	s->typeParamCount = 0;
	s->typeParams     = NULL;
	
	struct SimpleType* myst = make(SimpleType);
	myst->primitiveType = NULL;
	myst->structType    = s;
	
	struct BasicType* btw = make(BasicType);
	btw->simpleType = myst;
	btw->subrType = NULL;
	
	struct Type* m = make(Type);
		m->m1 = btw;
		m->m2 = NULL;
		m->m3 = NULL;
		
	return m;
}
