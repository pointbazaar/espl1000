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
	//{ "fgets",  "int" },
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
	//{ "rewind", "void" },
};

struct Proto protos_stdlib[] = {
	{ "atof", "float" },
	{ "atoi", "int" },
	{ "rand", "int" },
	//{ "srand", "void" },
	//calloc, malloc, free
	{ "atexit", "int" },
	//exit
	//getenv
	{ "system", "int" },
	//bsearch
	//qsort
	{ "abs", "int" },
	//div
};

struct Proto protos_string[] = {
	{ "strcmp", "int" },
	{ "strlen", "int" },
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
	
	fill_protos(st, protos_math, l1);
	fill_protos(st, protos_stdio, l2);
	fill_protos(st, protos_stdlib, l3);
	fill_protos(st, protos_string, l4);
	fill_protos(st, protos_ctype, l5);
}

static void fill_protos(struct ST* st, struct Proto* protos, int n){

	struct Type* mf = typeFromStrPrimitive(st, "float");
	struct Type* mi = typeFromStrPrimitive(st, "int");
	

	for(int i=0;i < n; i++){
	
		char* name = protos[i].name;
		char* type = protos[i].type;
		
		struct Type* t = NULL;
		
		if(strcmp(type, "int")   == 0){ t = mi; }
		if(strcmp(type, "float") == 0){ t = mf; }
		
		if(t == NULL){
			printf("[Transpiler] Error in fill_proto\n");
			exit(1);
		}
		
		sst_add(st->sst, makeSSTLine(name, t, true));
	}
}
