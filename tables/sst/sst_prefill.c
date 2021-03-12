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
};

static void fill_protos(struct ST* st, struct Proto* protos, int n);

struct Proto protos_math[] = {
	{ "sin", "float" },
	{ "cos", "float" },
	{ "tan", "float" },
	
	{ "asin", "float" },
	{ "acos", "float" },
	{ "atan", "float" },
	
	{ "sinh", "float" },
	{ "cosh", "float" },
	{ "tanh", "float" },
	
	{ "exp", "float" },
	{ "log", "float" },
	{ "log10", "float" },
	
	{ "pow", "float" },
	{ "sqrt", "float" },
	
	{ "ceil", "float" },
	{ "floor", "float" },
	
	{ "fabs", "float" },
	
	{ "fmin", "float" },
	{ "fmax", "float" },
};

struct Proto protos_stdio[] = {
	{ "fscanf", "int" },
	{ "scanf",  "int" },
	{ "sscanf", "int" },
	
	{ "fprintf", "int" },
	{ "printf",  "int" },
	{ "sprintf", "int" },
	
	{ "fgetc", "int" },
	{ "fgets",  "[char]" },
	{ "fputc", "int" },
	{ "fputs", "int" },
	{ "getc", "int" },
	{ "getchar", "int" },
	{ "putc", "int" },
	{ "putchar", "int" },
	{ "puts", "int" },
	{ "ungetc", "int" },
	
	{ "fread", "int" },
	{ "fwrite", "int" },
	
	{ "fgetpos", "int" },
	{ "fseek", "int" },
	{ "fsetpos", "int" },
	{ "ftell", "int" },
	{ "rewind", "int" }, //TODO: wrong return type
};

struct Proto protos_stdlib[] = {
	{ "atof", "float" },
	{ "atoi", "int" },
	{ "rand", "int" },
	{ "srand", "int" }, //TODO: wrong return type
	{ "calloc", "int" }, //TODO: wrong return type
	{ "malloc", "int" }, //TODO: wrong return type
	{ "free", "int" }, //TODO: wrong return type
	{ "atexit", "int" },
	{ "exit", "int" }, //TODO: wrong return type
	{ "getenv", "[char]" },
	{ "system", "int" },
	//bsearch
	//qsort
	{ "abs", "int" },
	//div
};

struct Proto protos_string[] = {
	{ "strcmp", "int" },
	{ "strncmp", "int" },
	
	{ "strlen", "int" },
	{ "strnlen", "int" },
	
	{ "strcpy", "[char]" },
	{ "strncpy", "[char]" },
	
	{ "strcat", "[char]" },
	{ "strncat", "[char]" },
	
	{ "strchr", "[char]" },
	{ "strrchr", "[char]" },
	
	{ "strspn", "int" },
	{ "strcspn", "int" },
	
	{ "strpbrk", "[char]" },
	{ "strstr", "[char]" },
	
	{ "basename", "[char]" },
	
	{ "strerror", "[char]" },
	
	{ "strtok", "[char]" },
};

struct Proto protos_ctype[] = {
	{ "isalnum", "int" },
	{ "isalpha", "int" },
	{ "isblank", "int" },
	{ "iscntrl", "int" },
	{ "isdigit", "int" },
	{ "isgraph", "int" },
	{ "islower", "int" },
	{ "isprint", "int" },
	{ "ispunct", "int" },
	{ "isspace", "int" },
	{ "isupper", "int" },
	{ "isxdigit", "int" },
	
	{ "tolower", "int" },
	{ "toupper", "int" },
};

struct Proto protos_assert[] = {
	{ "assert", "int" }, //TODO: wrong return type
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

static void fill_protos(struct ST* st, struct Proto* protos, int n){

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
	
		char* name = protos[i].name;
		char* type = protos[i].type;
		
		struct Type* t = NULL;
		
		if(strcmp(type, "int")   == 0){ t = mi; }
		if(strcmp(type, "float") == 0){ t = mf; }
		if(strcmp(type, "char") == 0) { t = mc; }
		if(strcmp(type, "[char]") == 0){ t = cp; }
		
		if(t == NULL){
			printf("[Transpiler] Error in fill_proto\n");
			exit(1);
		}
		
		sst_add(st->sst, makeSSTLine(name, t, true));
	}
}
