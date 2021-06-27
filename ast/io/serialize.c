#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "serialize.h"

void magic_num_serialize(uint32_t num, FILE* file){
	
	//this routine writes a given magic number to 'file'
	//in order to early-detect a corrupted .ast file
	
	serialize_int(num, file);
}
void magic_num_require(uint32_t expected, FILE* file){
	
	//this routine requires to read a magic number from 'file'
	//in order to early-detect a corrupted .ast file
	
	const int actual = deserialize_int(file);
	if(actual != expected){
		
		printf("[AST] magic_num_require failed.\n");
		printf("[AST] required was: %d\n", expected);
		printf("[AST] actual: %d\n", actual);
		
		fclose(file);
		exit(1);
	}
}
//---------------------------------------------------
void serialize_int(uint32_t x, FILE* file){
	fwrite(&x, sizeof(uint32_t), 1, file);
}
uint32_t deserialize_int(FILE* file){
	uint32_t res;
	fread(&res, sizeof(uint32_t), 1, file);
	return res;
}
//-------------------------------------------
void serialize_string(char* str, FILE* file){
	
	const int len = strlen(str);
	serialize_int(len, file);
	
	fwrite(str, sizeof(char), len, file);
}
char* deserialize_string(FILE* file){
	
	const int len = deserialize_int(file);
	
	char* res = malloc(sizeof(char)*(len+1));
	memset(res, 0, len+1);
	
	fread(res, sizeof(char), len, file);
	
	return res;
}
//-----------------------------------------
void serialize_astnode(struct ASTNode* node, FILE* file){
	serialize_int(node->line_num, file);
	serialize_int(node->annotations, file);
}
void deserialize_astnode(struct ASTNode* node, FILE* file){
	node->line_num    = deserialize_int(file);
	node->annotations = deserialize_int(file);
}
