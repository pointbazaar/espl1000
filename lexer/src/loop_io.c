#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../../token/token.h"
#include "loop_io.h"

//statically allocate a char array to store the contents of
//our input file
char input[INPUT_CAPACITY];

void readFromFile(
	char* input_filename, 
	uint64_t amount_read, 
	uint64_t input_file_length,
	uint64_t* input_index
){
	const uint64_t input_capacity = INPUT_CAPACITY;
	
	bool debug = false;
	
	if(debug){
		printf("readFromFile(...)\n");
	}


	//add how many items we already read 
	//to our index in the file
	*input_index	+=	amount_read;

	if(debug){
		printf("amount_read: %ld , new input_index is : %ld \n",amount_read,*input_index);
	}

	if(*input_index >= input_file_length){
		printf("new input index would be after the file. meaning the file is probably fully read.\n");
		//we set input_index to be higher than input_file_length to break out
		*input_index = input_file_length + 1;
		return;
	}

	//read a part from our file
	FILE* file = fopen(input_filename,"r");
	if(file == NULL){
		printf("could not open input file \n");
		exit(1);
	}

	//skip to our position of last read
	fseek ( file , *input_index , SEEK_SET );

	//how many could we possibly read?
	uint64_t max_read = input_file_length - *input_index;

	//minimum of our capacity and how many we can read
	uint64_t read_length = (max_read < input_capacity)?max_read:input_capacity;

	size_t length_read = fread(input,sizeof(char),read_length,file);

	fclose(file);

	if(length_read < read_length){
		printf("error with fread(...)\n");
		printf("tried to read %ld bytes but only read %ld bytes\n",read_length,length_read);
		exit(1);
	}
}

void writeToFile(
	char* tkn_filename, 
	struct Token** tokens, 
	int tokens_capacity, 
	int len,
	bool free_tokens
){
	bool debug = false;
	
	if(debug){
		printf("writeToFile(...) : write to %s\n",tkn_filename);
	}

	FILE* file2 = fopen(tkn_filename,"w");
	

	if(file2 == NULL){
		printf("could not open output file \n");
		exit(1);
	}

	//should be big enough
	char buffer[500];

	for(uint64_t j=0;j<len;j++){

		struct Token* tkn = tokens[j];
		int id = tkn->kind;
		char* value = tkn->value_ptr;

		sprintf(buffer,"%i %s\n",id,value);

		//debug
		if(debug){
			//printf("writing   %s",buffer);
		}

		//free our token
		if(free_tokens){
			if( !tkn->statically_allocated ){
				free(tkn->value_ptr);
			}
			free(tkn);
		}
		
		fputs(buffer,file2);
	}

	if(debug){
		printf("close  %s\n",tkn_filename);
	}
	
	fclose(file2);

}
