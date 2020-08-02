//standard headers
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>

//user headers
#include "tokens.h"
#include "lexer.h"
#include "strutils.h"
#include "states.h"
#include "loop.h"
#include "init_dfa.h"

//TODO: when following the approach of writing to the '.tokens' file every x tokens,
//we should instead write to a '.tokens.temp' file, and rename it to a '.tokens' file
//when we are finished, so that this and other tools are not confused
//when dragon-lexer crashes

//statically allocate a char array to store the contents of
//our input file
const uint64_t input_capacity = 5000;	//should be 5000
char input[5000];

//statically allocate a Token* array to store our Tokens
const uint64_t tokens_capacity = 5000;	//should be 5000
struct Token* tokens[5000];

struct Token** lex(char* clean_source, char* tkn_filename){
	if(DEBUG){
		printf("lex(...)\n");
	}

	//we use a temporary File for the test to make them
	//compatible with the implementation
	//TODO
	FILE* temp_file = fopen("temp.dg","w");
	fwrite(clean_source,sizeof(char),strlen(clean_source),temp_file);
	fclose(temp_file);

	struct Token** res = lex_main(tkn_filename,"temp.dg",strlen(clean_source),false);

	//delete the file afterwards
	remove("temp.dg");

	return res;
}


struct Token** lex_main(char* tkn_filename, char* input_filename, long input_file_length, bool free_tokens){
	//https://www.youtube.com/watch?v=G4g-du1MIas
	//https://nothings.org/computer/lexing.html

	uint64_t input_index = 0;	//our index in the input file

	if(DEBUG){
		printf("lex_main(...)\n");
	}

	readFromFile(input_filename,input_capacity,0,input_file_length,&input_index);

	uint64_t tokens_index = 0;

	printf("initializing deterministic finite automaton (a kind of state machine) \n");

	const uint64_t n_states = 150;		//number of states in our state machine
	const uint64_t n_transitions = 256; //possible ascii chars

	printf("allocating memory for state machine \n");

	//deterministic finite automaton
	//int dfa[n_states][n_transitions];
	short** dfa = (short**)malloc(sizeof(short*)*n_states);
	//init the memory
	for(uint64_t i = 0;i < n_states;i++){
		dfa[i] = (short*)malloc(sizeof(short)*n_transitions);
	}

	//if a state is final
	bool* final_state = malloc(sizeof(bool)*n_states);

	init_dfa(dfa,final_state, n_states);
	
	printf("starting lexer loop\n");

	short state;

	uint64_t i = 0;
	int line_no = 1;
	
	while( input_index+i < input_file_length){

		char ch = input[i];

		if(DEBUG){
			printf("start inner loop\n");
		}

		uint64_t start = i;
		state = S_START;

		do{
			ch = input[i];
			i++;

			//TODO: make the dfa smaller by cutting
			//off unused sections of characters 
			//(from the about 256 ascii ones)
			line_no += (ch=='\n')?1:0;
			state = dfa[state][ch];

			if(DEBUG){
				//debug: which state we got into
				printf("looking at '%c' -> %i \n",ch,state);
				printf("line_no : %d\n",line_no);
			}

		}while(  !final_state[state] );

		if(DEBUG){
			printf("recognized a token or error\n\n");
		}

		unsigned int end=i;

		if(state==S_START){
			continue;
		}

		if(state==S_EOS){
			printf("reached end of file / string\n");
			break;
		}

		struct Token* tkn = (struct Token*)malloc(sizeof(struct Token));

		tkn->kind=-1;
		tkn->value="";
		tkn->statically_allocated=true;

		switch(state){

			//ERROR
			case S_ERROR:
				free_dfa(dfa,n_states);
				free(final_state);
				printf("encountered error state\n");
				exit(1);
				break;

			//NEWLINE
			case S_NEWLINE_FINAL:
				;
				char* str = (char*)malloc(sizeof(char)*20);
				sprintf(str, "%d", line_no);
				tkn->kind=LINE_NO;
				tkn->value=str;
				tkn->statically_allocated=false;
				break;

			//SECTION: KEYWORDS
			case S_loop_FINAL:
				tkn->kind=LOOP;
				tkn->value="LOOP";
				i--;
				break;

			case S_IF_FINAL:
				tkn->kind=IF;
				tkn->value="IF";
				i--;
				break;

			case S_WHILE_FINAL:
				tkn->kind=WHILE;
				tkn->value="WHILE";
				i--;
				break;

			case S_RETURN_FINAL:
				tkn->kind=RETURN;
				tkn->value="RETURN";
				break;

			case S_struct_FINAL:
				tkn->kind=STRUCT;
				tkn->value="STRUCT";
				break;

			case S_fn_FINAL:
				tkn->kind=FN;
				tkn->value="FN";
				break;

			case S_else_FINAL:
				tkn->kind=ELSE;
				tkn->value="ELSE";
				i--;
				break;

			//SECTION: IDENTIFIERS
			case S_IDENTIFIER_FINAL:
				tkn->kind=IDENTIFIER;
				tkn->value=substr(input+start,end-start-1);
				tkn->statically_allocated=false;
				i--;
				break;

			case S_TYPEIDENTIFIER_FINAL:
				tkn->kind=TYPEIDENTIFIER;
				tkn->value=substr(input+start,end-start-1);
				tkn->statically_allocated=false;
				i--;
				break;

			case S_TPARAM_FINAL:
				tkn->kind=TYPEPARAM;
				tkn->value=substr(input+start+2,1);
				tkn->statically_allocated=false;
				i--;
				break;

			//SECTION: BRACKETS,BRACES,PARENS
			case S_LCURLY_FINAL:
				tkn->kind=LCURLY;
				tkn->value="LCURLY";

				break;

			case S_RCURLY_FINAL:
				tkn->kind=RCURLY;
				tkn->value="RCURLY";
				break;

			case S_LPARENS_FINAL:
				tkn->kind=LPARENS;
				tkn->value="LPARENS";
				break;

			case S_RPARENS_FINAL:
				tkn->kind=RPARENS;
				tkn->value="RPARENS";
				break;

			case S_LBRACKET_FINAL:
				tkn->kind=LBRACKET;
				tkn->value="LBRACKET";

				break;

			case S_RBRACKET_FINAL:
				tkn->kind=RBRACKET;
				tkn->value="RBRACKET";
				break;

			//SECTION: OTHER SYMBOLS
			case S_LESSER_FINAL:
				tkn->kind=LESSER;
				tkn->value="LESSER";
				break;

			case S_GREATER_FINAL:
				tkn->kind=GREATER;
				tkn->value="GREATER";
				break;

			case S_WAVE_FINAL:
				tkn->kind=WAVE;
				tkn->value="WAVE";
				break;

			case S_SEMICOLON_FINAL:
				tkn->kind=SEMICOLON;
				tkn->value="SEMICOLON";
				break;

			case S_EQ_FINAL:
				tkn->kind=EQ;
				tkn->value="=";
				i--;
				break;

			case S_BOOLCONST_FINAL:
				tkn->kind=BOOLCONST;
				tkn->value=substr(input+start,end-start-1);
				tkn->statically_allocated=false;
				i--;
				break;

			case S_ANYTYPE_FINAL:
				tkn->kind=ANYTYPE;
				tkn->value="#";
				break;

			case S_STRUCTMEMBERACCESS_FINAL:
				tkn->kind=STRUCTMEMBERACCESS;
				tkn->value=".";
				break;

			case S_COMMA_FINAL:
				tkn->kind=COMMA;
				tkn->value=",";
				break;

			case S_ARROW_FINAL:
				tkn->kind=ARROW;
				tkn->value=substr(input+start,end-start);
				tkn->statically_allocated=false;
				break;

			//SECTION: COMMENTS
			case S_SINGLE_LINE_COMMENT_FINAL:
				//as it is a comment,
				//it should be skipped
				//free the allocated memory
				free(tkn);
				continue;
				break;

			case S_MULTI_LINE_COMMENT_FINAL:
				//as it is a comment,
				//it should be skipped

				//free the allocated memory
				free(tkn);
				continue;
				break;
				
			//SECTION: NUMBERS
			case S_INTEGER_FINAL:
				tkn->kind=INTEGER;
				tkn->value=substr(input+start,end-start-1);;
				tkn->statically_allocated=false;
				i--;
				break;

			case S_FLOAT_FINAL:
				tkn->kind=FLOATING;
				tkn->value=substr(input+start,end-start-1);
				tkn->statically_allocated=false;
				i--;
				break;

			//SECTION: OPERATORS
			case S_OPERATOR_FINAL:
				tkn->kind=OPERATOR;
				tkn->value=substr(input+start,end-start);
				tkn->statically_allocated=false;
				//TODO: recognize /2 and do i--
				break;

			case S_MINUS_FINAL:
				tkn->kind=OPERATOR;
				tkn->value=substr(input+start,end-start-1);
				tkn->statically_allocated=false;
				i--;
				break;

			//SECTION: CHARCONST, STRINGCONST
			case S_CHARCONST_FINAL:
				tkn->kind=CHARCONST;
				tkn->value=substr(input+start,end-start);
				tkn->statically_allocated=false;
				break;

			case S_STRING_FINAL:
				tkn->kind=STRINGCONST;
				tkn->value=substr(input+start,end-start);
				tkn->statically_allocated=false;
				break;

			default:
				printf("unconsidered in lexer.c\n");
				free_dfa(dfa,n_states);
				free(final_state);
				exit(1);
				break;
		}

		if(DEBUG){
			printf("i = %ld \n",i);
		}

		tokens[tokens_index++] = tkn;

		if(tokens_index >= tokens_capacity){
			writeToFile(tkn_filename, tokens,tokens_capacity,tokens_index,free_tokens);
			tokens_index=0;
		}

		//maybe we should read again from the file
		//starting from the position we stopped reading last time
		if(i > (input_capacity * 0.5)){
			readFromFile(input_filename,input_capacity,i,input_file_length,&input_index);
			i = 0;	//reset index into input array
		}
	}

	writeToFile(tkn_filename, tokens,tokens_capacity,tokens_index,free_tokens);

	//TODO: handle the deletion of the .tokens file
	//if any errors were encountered anywhere or
	//malloc could not allocate or anything

	printf("free state machine\n");
	free_dfa(dfa,n_states);

	printf("free final_state array\n");
	free(final_state);

	return tokens;

}

void readFromFile(
	char* input_filename, 
	uint64_t input_capacity, 
	uint64_t amount_read, 
	uint64_t input_file_length,
	uint64_t* input_index
){
	
	if(DEBUG){
		printf("readFromFile(...)\n");
	}


	//add how many items we already read 
	//to our index in the file
	*input_index	+=	amount_read;

	if(DEBUG){
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

	if(length_read < read_length){
		printf("error with fread(...)\n");
		printf("tried to read %ld bytes but only read %ld bytes\n",read_length,length_read);
		exit(1);
	}

	fclose(file);
}

void writeToFile(
	char* tkn_filename, 
	struct Token** tokens, 
	int tokens_capacity, 
	int len,
	bool free_tokens
){

	if(DEBUG){
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
		char* value = tkn->value;

		sprintf(buffer,"%i %s\n",id,value);

		//debug
		if(DEBUG){
			printf("writing   %s",buffer);
		}

		//free our token
		if(free_tokens){
			if( !tkn->statically_allocated ){
				free(tkn->value);
			}
			free(tkn);
		}
		
		fputs(buffer,file2);
	}

	if(DEBUG){
		printf("close  %s\n",tkn_filename);
	}
	
	fclose(file2);

}
