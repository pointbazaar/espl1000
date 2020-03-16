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
	struct Token* r = makeToken2(CCONST,"c");

	switch (tkn_id) {

		case STRINGCONST : 
			r = makeToken2(STRINGCONST, tkn.substr(3,strlen(tkn)));
			break;
		
		case CCONST : 
			r = makeToken2(CCONST,tkn.substr(2,strlen(tkn)));
			break;
		case ANYTYPE : 
			r = makeToken2(ANYTYPE,"#");
			break;

		//CONSTANTS
		case BCONST : 
			r = makeToken2(BCONST, parts[1] );
			break;
		case FLOATING : 
			;
			r = makeToken2(FLOATING, parts[1]);
			break;
		case INTEGER : 
			r = makeToken2(INTEGER, parts[1]);
			break;

		//IDENTIFIERS
		case ID : 
			r = makeToken2(ID,parts[1]);
			break;
		case TYPEIDENTIFIER : 
			r = makeToken2(TYPEIDENTIFIER,parts[1]);
			break;

		//SECTION: OPERATORNS
		case OPKEY : 
			r = makeToken2(OPKEY,parts[1]);
			break;

		case EQ : 
			r = makeToken2(EQ,"=");
			break;

		case STRUCTMEMBERACCESS : 
			r = makeToken2(STRUCTMEMBERACCESS,".");
			break;

		case TPARAM : 
			r = makeToken2(TPARAM, parts[1]);
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
			r = makeToken2(ARROW,parts[1]);
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
