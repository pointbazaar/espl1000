#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "TokenReader.h"
#include "TokenKeys.h"
#include "Token.h"

struct Token* recognizeToken(char* tkn, bool debug) {
	//parses the token, as encoded by the lexer
	//everything being seperated by a space, and the whole line not
	//having any spaces in front or at the back is important to keep parsing simple.
	if (debug) {
		printf("recognizeToken(%s, %d)\n", tkn, debug);
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
		printf("could not read 2 parts of token\n");
		return NULL;
	}

	tkn[space_index] = '\0';
	
	strcpy(part1, tkn);
	strcpy(part2, tkn+space_index+1);

	int tkn_id = atoi(part1);
	int line_no = 1;

	if (tkn_id == LINE_NO) {
		int line_no_change = atoi(part2);
		line_no += line_no_change;
		return NULL;
		//break;
	}
	struct Token* r = makeToken2(CCONST,"c");

	switch (tkn_id) {

		case STRINGCONST : 
			r = makeToken2(STRINGCONST, tkn+3);
			break;
		
		case CCONST : 
			r = makeToken2(CCONST, tkn+2);
			break;
		case ANYTYPE : 
			r = makeToken2(ANYTYPE,"#");
			break;

		//CONSTANTS
		case BCONST : 
			r = makeToken2(BCONST, part2);
			break;
		case FLOATING : 
			;
			r = makeToken2(FLOATING, part2);
			break;
		case INTEGER : 
			r = makeToken2(INTEGER, part2);
			break;

		//IDENTIFIERS
		case ID : 
			r = makeToken2(ID,part2);
			break;
		case TYPEIDENTIFIER : 
			r = makeToken2(TYPEIDENTIFIER,part2);
			break;

		//SECTION: OPERATORNS
		case OPKEY : 
			r = makeToken2(OPKEY,part2);
			break;

		case EQ : 
			r = makeToken2(EQ,"=");
			break;

		case STRUCTMEMBERACCESS : 
			r = makeToken2(STRUCTMEMBERACCESS,".");
			break;

		case TPARAM : 
			r = makeToken2(TPARAM, part2);
			break;

		//BRACKETS, BRACES, PARENTHESES
		case LBRACKET : 
			r = makeToken2(LBRACKET,"[");
			break;
		case RBRACKET : 
			r = makeToken2(RBRACKET,"]");
			break;

		case LPARENS : 
			r = makeToken2(LPARENS,"(");
			break;
		case RPARENS : 
			r = makeToken2(RPARENS,")");
			break;

		case LCURLY : 
			r = makeToken2(LCURLY,"{");
			break;
		case RCURLY : 
			r = makeToken2(RCURLY,"}");
			break;

		case GREATER: 
			r = makeToken2(GREATER,">");
			break;

		case LESSER : 
			r = makeToken2(LESSER,"<");
			break;

		case WAVE : 
			r = makeToken2(WAVE,"~");
			break;

		case SEMICOLON : 
			r = makeToken2(SEMICOLON,";");
			break;

		case COMMA : 
			r = makeToken2(COMMA,",");
			break;
		case ARROW : 
			r = makeToken2(ARROW,part2);
			break;

		//KEYWORDS
		case RETURN : 
			r = makeToken2(RETURN,"return");
			break;
		case FN : 
			r = makeToken2(FN,"fn");
			break;
		case STRUCT : 
			r = makeToken2(STRUCT,"struct");
			break;
		case IF : 
			r = makeToken2(IF,"if");
			break;
		case ELSE : 
			r = makeToken2(ELSE,"else");
			break;
		case WHILE : 
			r = makeToken2(WHILE,"while");
			break;
		case LOOP : 
			r = makeToken2(LOOP,"loop");
			break;

		default : 
			//("unreconized token id : " + tkn_id);
			return NULL;
	};

	r->lineNumber = line_no;

	return r;
}
