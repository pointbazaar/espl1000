#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "tables/symtable/symtable.h"
#include "ast/ast.h"

//TODO: get rid of this include,
//as sst_prefill should not depend on type
//type_str
#include "transpiler/main/typeinference/util/type_str.h"

#include "tables/sst/sst.h"
#include "sst_prefill.h"

struct Proto {
	char name[DEFAULT_STR_SIZE];
	char type[DEFAULT_STR_SIZE];
	bool halts_known;
	bool halts;
};

static void fill_protos(struct ST* st, struct Proto* protos, int n);

//TODO: find trustworthy sources
//to see which of these functions 
//are guaranteed to terminate

struct Proto protos_math[] = {
	{ "sin", "float", true, true },
	{ "cos", "float", true, true },
	{ "tan", "float", true, true },
	
	{ "asin", "float", true, true },
	{ "acos", "float", true, true },
	{ "atan", "float", true, true },
	
	{ "sinh", "float", true, true },
	{ "cosh", "float", true, true },
	{ "tanh", "float", true, true },
	
	{ "exp", "float", true, true },
	{ "log", "float", true, true },
	{ "log10", "float", true, true },
	
	{ "pow", "float", true, true },
	{ "sqrt", "float", true, true },
	
	{ "ceil", "float", true, true },
	{ "floor", "float", true, true },
	
	{ "fabs", "float", true, true },
	
	{ "fmin", "float", true, true },
	{ "fmax", "float", true, true },

	{ "round", "int", true, true },
};

struct Proto protos_stdio[] = {
	{ "fscanf",  "int", false, false },
	{ "scanf",   "int", false, false },
	{ "sscanf",  "int", true, true },
	
	{ "fprintf", "int", false, false },
	{ "printf",  "int", false, false },
	{ "sprintf", "int", true, true },
	
	{ "fgetc",   "int", false, false },
	{ "fgets",   "[char]", false, false },
	{ "fputc",   "int", false, false },
	{ "fputs",   "int", false, false },
	{ "getc",    "int", false, false },
	{ "getchar", "int", false, false },
	{ "putc",    "int", false, false },
	{ "putchar", "int", false, false },
	{ "puts",    "int", false, false },
	{ "ungetc",  "int", false, false },
	
	{ "fread",   "int", false, false },
	{ "fwrite",  "int", false, false },
	
	{ "fgetpos", "int", false, false },
	{ "fseek",   "int", false, false },
	{ "fsetpos", "int", false, false },
	{ "ftell",   "int", false, false },
	{ "rewind",  "int", false, false }, //TODO: wrong return type
};

struct Proto protos_stdlib[] = {
	{ "atof",   "float",  true, true },
	{ "atoi",   "int",    true, true },
	{ "rand",   "int",    true, true },
	{ "srand",  "int",    true, true }, //TODO: wrong return type
	{ "calloc", "int",    true, true }, //TODO: wrong return type
	{ "malloc", "int",    true, true }, //TODO: wrong return type
	{ "free",   "int",    true, true }, //TODO: wrong return type
	{ "atexit", "int",    true, true },
	{ "exit",   "int",    true, true }, //TODO: wrong return type
	{ "getenv", "[char]", false, false },
	{ "system", "int",    false, false },
	//bsearch
	//qsort
	{ "abs", "int" },
	//div
};

struct Proto protos_string[] = {
	{ "strcmp", "int", false, false },
	{ "strncmp", "int", true, true },
	
	{ "strlen", "int", false, false },
	{ "strnlen", "int", true, true },
	
	{ "strcpy", "[char]", false, false },
	{ "strncpy", "[char]", true, true },
	
	{ "strcat", "[char]", false, false },
	{ "strncat", "[char]", true, true },
	
	{ "strchr", "[char]", false, false },
	{ "strrchr", "[char]", false, false },
	
	{ "strspn", "int", false, false },
	{ "strcspn", "int", false, false },
	
	{ "strpbrk", "[char]", false, false },
	{ "strstr", "[char]", false, false },
	
	{ "basename", "[char]", false, false },
	
	{ "strerror", "[char]", false, false },
	
	{ "strtok", "[char]", false, false },
};

struct Proto protos_ctype[] = {
	{ "isalnum",  "int", true, true },
	{ "isalpha",  "int", true, true },
	{ "isblank",  "int", true, true },
	{ "iscntrl",  "int", true, true },
	{ "isdigit",  "int", true, true },
	{ "isgraph",  "int", true, true },
	{ "islower",  "int", true, true },
	{ "isprint",  "int", true, true },
	{ "ispunct",  "int", true, true },
	{ "isspace",  "int", true, true },
	{ "isupper",  "int", true, true },
	{ "isxdigit", "int", true, true },
	
	{ "tolower",  "int", true, true },
	{ "toupper",  "int", true, true },
};

struct Proto protos_assert[] = {
	{ "assert", "int", true, true }, //TODO: wrong return type
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
	
	fill_protos(st, protos_math, l1);
	fill_protos(st, protos_stdio, l2);
	fill_protos(st, protos_stdlib, l3);
	fill_protos(st, protos_string, l4);
	fill_protos(st, protos_ctype, l5);
	fill_protos(st, protos_assert, l6);
}

static void fill_protos(
	struct ST* st, 
	struct Proto* protos, 
	int n
){

	struct Type* mf = typeFromStrPrimitive(st, "float");
	struct Type* mi = typeFromStrPrimitive(st, "int");
	struct Type* mc = typeFromStrPrimitive(st, "char");
	
	struct PrimitiveType* pt = make(PrimitiveType);
	
	pt->isIntType  = false;
	pt->isCharType = true;
	pt->isFloatType= false;
	pt->isBoolType = false;
	
	struct SimpleType* myst = make(SimpleType);
	myst->primitiveType = pt;
	myst->structType    = NULL;
	
	struct BasicTypeWrapped* btw = make(BasicTypeWrapped);
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
		
	registerInferredType(st, cp);

	for(int i=0;i < n; i++){
		
		struct Proto proto = protos[i];
	
		char* name = proto.name;
		char* type = proto.type;
		
		bool halts_known = proto.halts_known;
		bool halts       = proto.halts;
		
		struct Type* t = NULL;
		
		if(strcmp(type, "int")   == 0){ t = mi; }
		if(strcmp(type, "float") == 0){ t = mf; }
		if(strcmp(type, "char") == 0) { t = mc; }
		if(strcmp(type, "[char]") == 0){ t = cp; }
		
		if(t == NULL){
			printf("[SST][Error] fill_proto\n");
			exit(1);
		}
		
		struct SSTLine* line = 
			makeSSTLine(name, t, true, halts_known, halts);
		
		sst_add(st->sst, line);
	}
}
