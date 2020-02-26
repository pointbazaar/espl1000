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
		printf("recognizeToken(...)\n");
		printf("recognize: %s\n", tkn);
	}

	char* parts[10];
	int partsc = 0;

	char* ss = tkn;
    char* token;
    while (getline(ss, token, ' ')) {
        parts[partsc++] = token;
    }
	int tkn_id = atoi(parts[0]);
	int line_no = 1;

	if (tkn_id == LINE_NO) {
		int line_no_change = atoi(parts[1]);
		line_no += line_no_change;
		return NULL;
		//break;
	}
	struct Token* r = makeToken(CCONST,"c");

	switch (tkn_id) {

		case STRINGCONST : 
			r = makeToken(STRINGCONST, tkn.substr(3,strlen(tkn)));
			break;
		
		case CCONST : 
			r = makeToken(CCONST,tkn.substr(2,strlen(tkn)));
			break;
		case ANYTYPE : 
			r = makeToken(ANYTYPE,"#");
			break;

		//CONSTANTS
		case BCONST : 
			r = makeToken(BCONST, parts[1] );
			break;
		case FLOATING : 
			;
			r = makeToken(FLOATING, parts[1]);
			break;
		case INTEGER : 
			r = makeToken(INTEGER, parts[1]);
			break;

		//IDENTIFIERS
		case ID : 
			r = makeToken(ID,parts[1]);
			break;
		case TYPEIDENTIFIER : 
			r = makeToken(TYPEIDENTIFIER,parts[1]);
			break;

		//SECTION: OPERATORNS
		case OPKEY : 
			r = makeToken(OPKEY,parts[1]);
			break;

		case EQ : 
			r = makeToken(EQ,"=");
			break;

		case STRUCTMEMBERACCESS : 
			r = makeToken(STRUCTMEMBERACCESS,".");
			break;

		case TPARAM : 
			r = makeToken(TPARAM, parts[1]);
			break;

		//BRACKETS, BRACES, PARENTHESES
		case LBRACKET : 
			r = makeToken(LBRACKET,"[");
			break;
		case RBRACKET : 
			r = makeToken(RBRACKET,"]");
			break;

		case LPARENS : 
			r = makeToken(LPARENS,"(");
			break;
		case RPARENS : 
			r = makeToken(RPARENS,")");
			break;

		case LCURLY : 
			r = makeToken(LCURLY,"{");
			break;
		case RCURLY : 
			r = makeToken(RCURLY,"}");
			break;

		case GREATER: 
			r = makeToken(GREATER,">");
			break;

		case LESSER : 
			r = makeToken(LESSER,"<");
			break;

		case WAVE : 
			r = makeToken(WAVE,"~");
			break;

		case SEMICOLON : 
			r = makeToken(SEMICOLON,";");
			break;

		case COMMA : 
			r = makeToken(COMMA,",");
			break;
		case ARROW : 
			r = makeToken(ARROW,parts[1]);
			break;

		//KEYWORDS
		case RETURN : 
			r = makeToken(RETURN,"return");
			break;
		case FN : 
			r = makeToken(FN,"fn");
			break;
		case STRUCT : 
			r = makeToken(STRUCT,"struct");
			break;
		case IF : 
			r = makeToken(IF,"if");
			break;
		case ELSE : 
			r = makeToken(ELSE,"else");
			break;
		case WHILE : 
			r = makeToken(WHILE,"while");
			break;
		case LOOP : 
			r = makeToken(LOOP,"loop");
			break;

		default : 
			//("unreconized token id : " + tkn_id);
			return NULL;
	};

	r->lineNumber=line_no;

	return r;
}
