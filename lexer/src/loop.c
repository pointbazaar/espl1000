//standard headers
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//user headers

#include "lexer.h"
#include "strutils.h"
#include "states.h"
#include "loop.h"
#include "init_dfa.h"
#include "loop_io.h"

#include "../../token/token.h"

//token types
#include "../../parser/main/commandline/TokenKeys.h"

//TODO: when following the approach of writing to the '.tokens' file every x tokens,
//we should instead write to a '.tokens.temp' file, and rename it to a '.tokens' file
//when we are finished, so that this and other tools are not confused
//when dragon-lexer crashes

//statically allocate a Token* array to store our Tokens
const uint64_t tokens_capacity = 5000;	//should be 5000
struct Token* tokens[5000];

const uint64_t n_states = 180;		//number of states in our state machine

//possible ascii chars, without the 
//Extended Ascii codes
//source: http://www.asciitable.com/
const uint64_t n_transitions = 128; 

short state;
int line_no;

uint64_t tokens_index;

uint64_t input_index;	//our index in the input file

//deterministic finite automaton
//int dfa[n_states][n_transitions];
short** dfa;
bool* final_state;
uint64_t i;
//------------
void lex_main_inner(
	int input_capacity, 
	char* input_filename, 
	char* tkn_filename,
	int input_file_length,
	bool free_tokens,
	bool debug
);
//------------
struct Token** lex(
	char* clean_source, 
	char* tkn_filename,
	bool debug
){
	
	if(debug){
		printf("lex(...)\n");
	}

	//we use a temporary File for the test to make them
	//compatible with the implementation
	//TODO
	FILE* temp_file = fopen("temp.dg","w");
	fwrite(clean_source,sizeof(char),strlen(clean_source),temp_file);
	fclose(temp_file);

	struct Token** res = lex_main(
		tkn_filename,
		"temp.dg",
		strlen(clean_source),
		false,
		debug
	);

	//delete the file afterwards
	remove("temp.dg");

	return res;
}


struct Token** lex_main(
	char* tkn_filename, 
	char* input_filename, 
	long input_file_length, 
	bool free_tokens,
	bool debug
){
	//https://www.youtube.com/watch?v=G4g-du1MIas
	//https://nothings.org/computer/lexing.html
	const uint64_t input_capacity = 5000;
	line_no = 1;
	tokens_index = 0;
	input_index = 0;

	if(debug){
		printf("lex_main(...)\n");
	}

	readFromFile(input_filename,0,input_file_length,&input_index);

	if(debug){
		printf("initializing deterministic finite automaton (a kind of state machine) \n");
		printf("allocating memory for state machine \n");
	}

	dfa = malloc(sizeof(short*)*n_states);
	
	for(uint64_t k = 0;k < n_states;k++){
		dfa[k] = malloc(sizeof(short)*n_transitions);
	}

	final_state = malloc(sizeof(bool)*n_states);

	init_dfa(dfa,final_state, n_states);
	
	if(debug){
		printf("starting lexer loop\n");
	}

	lex_main_inner(
		input_capacity, 
		input_filename, 
		tkn_filename,
		input_file_length,
		free_tokens,
		debug
	);

	writeToFile(tkn_filename, tokens,tokens_capacity,tokens_index,free_tokens);

	if(debug){ printf("free state machine\n"); }
	free_dfa(dfa,n_states);

	if(debug){ printf("free final_state array\n"); }
	free(final_state);

	return tokens;

}

void lex_main_inner(
	int input_capacity, 
	char* input_filename, 
	char* tkn_filename,
	int input_file_length,
	bool free_tokens,
	bool debug
){
	
	i = 0;	
	
	while( input_index+i < input_file_length){

		char ch = input[i];

		if(debug){
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
			state = dfa[state][(short)ch];

			if(debug){
				//debug: which state we got into
				printf("looking at '%c' -> %i \n",ch,state);
				printf("line_no : %d\n",line_no);
			}

		}while(  !final_state[state] );

		if(debug){
			printf("recognized a token\n");
		}

		unsigned int end=i;

		if(state==S_START){
			continue;
		}

		if(state==S_EOS){
			printf("reached end of file / string\n");
			break;
		}

		struct Token* tkn = malloc(sizeof(struct Token));

		tkn->kind=-1;
		tkn->value_ptr="";
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
				char* str = malloc(sizeof(char)*20);
				sprintf(str, "%d", line_no);
				tkn->kind=LINE_NO;
				tkn->value_ptr=str;
				tkn->statically_allocated=false;
				break;

			//SECTION: KEYWORDS
			case S_loop_FINAL:
				tkn->kind=LOOP;
				tkn->value_ptr="loop";
				i--;
				break;

			case S_IF_FINAL:
				tkn->kind=IF;
				tkn->value_ptr="if";
				i--;
				break;

			case S_WHILE_FINAL:
				tkn->kind=WHILE;
				tkn->value_ptr="while";
				i--;
				break;
				
			case S_break_FINAL:
				tkn->kind=BREAK;
				tkn->value_ptr="break";
				i--;
				break;
				
			case S_for_FINAL:
				tkn->kind=FOR;
				tkn->value_ptr="for";
				i--;
				break;
				
			case S_in_FINAL:
				tkn->kind=IN;
				tkn->value_ptr="in";
				i--;
				break;

			case S_RETURN_FINAL:
				tkn->kind=RETURN;
				tkn->value_ptr="return";
				break;

			case S_struct_FINAL:
				tkn->kind=STRUCT;
				tkn->value_ptr="struct";
				break;

			case S_fn_FINAL:
				tkn->kind=FN;
				tkn->value_ptr="fn";
				break;

			case S_else_FINAL:
				tkn->kind=ELSE;
				tkn->value_ptr="else";
				i--;
				break;
				
			case S_switch_FINAL:
				tkn->kind=SWITCH;
				tkn->value_ptr="switch";
				i--;
				break;
			
			case S_case_FINAL:
				tkn->kind=CASE;
				tkn->value_ptr="case";
				i--;
				break;

			//SECTION: IDENTIFIERS
			case S_IDENTIFIER_FINAL:
				tkn->kind=ID;
				tkn->value_ptr=substr(input+start,end-start-1);
				tkn->statically_allocated=false;
				i--;
				break;

			case S_TYPEIDENTIFIER_FINAL:
				tkn->kind=TYPEIDENTIFIER;
				tkn->value_ptr=substr(input+start,end-start-1);
				tkn->statically_allocated=false;
				i--;
				break;

			case S_TPARAM_FINAL:
				tkn->kind=TPARAM;
				tkn->value_ptr=substr(input+start+2,1);
				tkn->statically_allocated=false;
				i--;
				break;

			//SECTION: BRACKETS,BRACES,PARENS
			case S_LCURLY_FINAL:
				tkn->kind=LCURLY;
				tkn->value_ptr="{";
				break;

			case S_RCURLY_FINAL:
				tkn->kind=RCURLY;
				tkn->value_ptr="}";
				break;

			case S_LPARENS_FINAL:
				tkn->kind=LPARENS;
				tkn->value_ptr="(";
				break;

			case S_RPARENS_FINAL:
				tkn->kind=RPARENS;
				tkn->value_ptr=")";
				break;

			case S_LBRACKET_FINAL:
				tkn->kind=LBRACKET;
				tkn->value_ptr="[";
				break;

			case S_RBRACKET_FINAL:
				tkn->kind=RBRACKET;
				tkn->value_ptr="]";
				break;

			//SECTION: OTHER SYMBOLS
			case S_SEMICOLON_FINAL:
				tkn->kind=SEMICOLON;
				tkn->value_ptr=";";
				break;

			//SECTION: ASSIGNMENT ------
			//sometimes reading single operators
			//into their own state for 
			//performance reasons (atleast i think so)
			case S_ASSIGN_PLUS_EQ_FINAL:
				tkn->kind = ASSIGNOP;
				tkn->value_ptr="+=";
				break;
			case S_ASSIGN_MINUS_EQ_FINAL:
				tkn->kind = ASSIGNOP;
				tkn->value_ptr="-=";
				break;
			case S_ASSIGN_TIMES_EQ_FINAL:
				tkn->kind = ASSIGNOP;
				tkn->value_ptr="*=";
				break;
			case S_ASSIGN_DIV_EQ_FINAL:
				tkn->kind = ASSIGNOP;
				tkn->value_ptr="/=";
				break;
				
			case S_ASSIGN_EQ_FINAL:
				tkn->kind=ASSIGNOP;
				tkn->value_ptr="=";
				i--;
				break;
			//--------------------------
			case S_BOOLCONST_FINAL:
				tkn->kind=BCONST;
				tkn->value_ptr=substr(input+start,end-start-1);
				tkn->statically_allocated=false;
				i--;
				break;

			case S_ANYTYPE_FINAL:
				tkn->kind=ANYTYPE;
				tkn->value_ptr="#";
				break;

			case S_STRUCTMEMBERACCESS_FINAL:
				tkn->kind=STRUCTMEMBERACCESS;
				tkn->value_ptr=".";
				i--;
				break;
				
			case S_RANGE_OP_FINAL:
				tkn->kind=RANGEOP;
				tkn->value_ptr="..";
				break;

			case S_COMMA_FINAL:
				tkn->kind=COMMA;
				tkn->value_ptr=",";
				break;

			case S_ARROW_FINAL:
				tkn->kind=ARROW;
				tkn->value_ptr=substr(input+start,end-start);
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
				tkn->value_ptr=substr(input+start,end-start-1);;
				tkn->statically_allocated=false;
				i--;
				break;

			case S_FLOAT_FINAL:
				tkn->kind=FLOATING;
				tkn->value_ptr=substr(input+start,end-start-1);
				tkn->statically_allocated=false;
				i--;
				break;

			//SECTION: OPERATORS
			case S_OPERATOR_FINAL_2:
				end--;
				i--;
			case S_OPERATOR_FINAL:
				tkn->kind=OPKEY;
				tkn->value_ptr=substr(input+start,end-start);
				tkn->statically_allocated=false;
				break;

			//SECTION: CHARCONST, STRINGCONST
			case S_CHARCONST_FINAL:
				tkn->kind=CCONST;
				tkn->value_ptr=substr(input+start,end-start);
				tkn->statically_allocated=false;
				break;

			case S_STRING_FINAL:
				tkn->kind=STRINGCONST;
				tkn->value_ptr=substr(input+start,end-start);
				tkn->statically_allocated=false;
				break;

			default:
				printf("unconsidered in lexer.c\n");
				free_dfa(dfa,n_states);
				free(final_state);
				exit(1);
				break;
		}
		
		if(debug){
			printf("recognized Token: (%d, '%s')\n", tkn->kind, tkn->value_ptr);
		}

		tokens[tokens_index++] = tkn;

		if(tokens_index >= tokens_capacity){
			writeToFile(tkn_filename, tokens,tokens_capacity,tokens_index,free_tokens);
			tokens_index=0;
		}

		//maybe we should read again from the file
		//starting from the position we stopped reading last time
		if(i > (input_capacity * 0.5)){
			readFromFile(input_filename,i,input_file_length,&input_index);
			i = 0;	//reset index into input array
		}
	}
}
