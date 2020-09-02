#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "TokenReader.h"
#include "TokenKeys.h"
#include "Token.h"

struct Token* recognizeTokenInner(int tkn_id, char* tkn, char* part2);

struct Token* recognizeToken(char* tkn, bool* isLineNo, bool debug) {
	//parses the token, as encoded by the lexer
	//everything being seperated by a space, and the whole line not
	//having any spaces in front or at the back is important to keep parsing simple.
	if (debug) {
		printf("recognizeToken('%s', %d)\n", tkn, debug);
	}

	
	char part1[10];
	char part2[30];

	//find first space
	int space_index = -1;
	for(int i=0;i<strlen(tkn);i++){
		if(tkn[i] == ' '){
			space_index = i;
			break;
		}
	}

	if(space_index == -1){
		if(debug){
			printf("could not read 2 parts of token\n");
		}
		return NULL;
	}

	tkn[space_index] = '\0';
	
	strcpy(part1, tkn);
	strcpy(part2, tkn+space_index+1);
	
	if(debug){
			printf("part 1: %s\n", part1);
			printf("part 2: %s\n", part2);
	}

	int tkn_id = atoi(part1);
	int line_no = 1;

	if (tkn_id == LINE_NO) {
		int line_no_change = atoi(part2);
		line_no += line_no_change;
		*isLineNo = true;
		return NULL;
	}
	

	struct Token* r = recognizeTokenInner(tkn_id, tkn, part2);

	r->lineNumber = line_no;

	return r;
}

struct Token* recognizeTokenInner(int tkn_id, char* tkn, char* part2){
	
	struct Token* r = NULL;
	
	switch (tkn_id) {
		case STRINGCONST : 
			r = makeToken2(STRINGCONST, tkn+3);
			break;
		case CCONST : 
			r = makeToken2(CCONST, tkn+2);
			break;
		case ANYTYPE : 
		//CONSTANTS
		case BCONST : 
		case FLOATING : 
		case INTEGER : 
		//BRACKETS, BRACES, PARENTHESES
		case LBRACKET : 
		case RBRACKET : 
		case LPARENS : 
		case RPARENS : 
		case LCURLY : 
		case RCURLY : 
		//IDENTIFIERS
		case ID : 
		case TYPEIDENTIFIER : 
		//SECTION: OPERATORNS
		case OPKEY : 
		case EQ : 
		//SECTION: OTHER
		case TPARAM : 
		case SEMICOLON : 
		case COMMA : 
		case ARROW : 
		case STRUCTMEMBERACCESS : 
		//KEYWORDS
		case RETURN : 
		case FN : 
		case STRUCT : 
		case IF : 
		case ELSE : 
		case WHILE : 
		case LOOP : 
		case BREAK:
		case WAVE : 
			r = makeToken2(tkn_id, part2);
			break;
		default : 
			//("unreconized token id : " + tkn_id);
			return NULL;
	};
	//DEBUG
	printf("read: tkn_id: %d part2: '%s'\n", tkn_id, part2);
	return r;
}
