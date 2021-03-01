#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "serialize.h"

void magic_num_serialize(uint32_t num, FILE* file){
	//paired with magic_num_require in 
	//ast_reader.h
	//this routine writes a given magic number to 'file'
	//in order to early-detect a corrupted .ast file
	serialize_int(num, file);
}

void magic_num_require(uint32_t expected, FILE* file){
	//paired with magic_num_serialize in ASTWriter.h
	//this routine requires to read a magic number from 'file'
	//in order to early-detect a corrupted .ast file
	const int actual = deserialize_int(file);
	if(actual != expected){
		printf("magic_num_require failed.\n");
		printf("required was: %d\n", expected);
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

