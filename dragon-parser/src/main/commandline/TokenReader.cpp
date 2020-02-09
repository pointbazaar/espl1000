//standard includes
#include <vector> 
#include <stdio.h>
#include <map>
#include <optional>
#include <iostream>
#include <sstream>

#include "TokenReader.hpp"
#include "TokenKeys.hpp"
#include "Token.hpp"

optional<Token> recognizeToken(string tkn, bool debug) {
	//parses the token, as encoded by the lexer
	//everything being seperated by a space, and the whole line not
	//having any spaces in front or at the back is important to keep parsing simple.
	if (debug) {
		cout << "recognize: " << tkn << endl;
	}

	vector<string> parts;

	stringstream ss(tkn);
    string token;
    while (getline(ss, token, ' ')) {
        parts.push_back(token);
    }
	int tkn_id = stoi(parts[0]);
	int line_no = 1;

	if (tkn_id == LINE_NO) {
		int line_no_change = stoi(parts[1]);
		line_no += line_no_change;
		return nullopt;
		//break;
	}
	Token r = Token(NAMESPACE,"namespace");

	switch (tkn_id) {

		case STRINGCONST : 
			r = Token(STRINGCONST, tkn.substr(3,tkn.size()));
			break;
		case CHARCONST : 
			r = Token(CHARCONST,tkn.substr(2,tkn.size()));
			break;
		case ANYTYPE : 
			r = Token(ANYTYPE,"#");
			break;

		//CONSTANTS
		case BOOLCONST : 
			r = Token(BOOLCONST, parts.at(1) );
			break;
		case FLOATING : 
			;
			r = Token(FLOATING, parts.at(1));
			break;
		case INTEGER : 
			r = Token(INTEGER, parts.at(1));
			break;

		//IDENTIFIERS
		case IDENTIFIER : 
			r = Token(IDENTIFIER,parts.at(1));
			break;
		case TYPEIDENTIFIER : 
			r = Token(TYPEIDENTIFIER,parts.at(1));
			break;

		//SECTION: OPERATORNS
		case OPERATOR : 
			r = Token(OPERATOR,parts.at(1));
			break;

		case EQ : 
			r = Token(EQ,"=");
			break;

		case STRUCTMEMBERACCESS : 
			r = Token(STRUCTMEMBERACCESS,".");
			break;

		case TYPEPARAM : 
			r = Token(TYPEPARAM, parts.at(1));
			break;

		case NAMESPACE : 
			r = Token(NAMESPACE,"namespace");
			break;

		//BRACKETS, BRACES, PARENTHESES
		case LBRACKET : 
			r = Token(LBRACKET,"[");
			break;
		case RBRACKET : 
			r = Token(RBRACKET,"]");
			break;

		case LPARENS : 
			r = Token(LPARENS,"(");
			break;
		case RPARENS : 
			r = Token(RPARENS,")");
			break;

		case LCURLY : 
			r = Token(LCURLY,"{");
			break;
		case RCURLY : 
			r = Token(RCURLY,"}");
			break;

		case GREATER: 
			r = Token(GREATER,">");
			break;

		case LESSER : 
			r = Token(LESSER,"<");
			break;

		case WAVE : 
			r = Token(WAVE,"~");
			break;

		case SEMICOLON : 
			r = Token(SEMICOLON,";");
			break;

		case COMMA : 
			r = Token(COMMA,",");
			break;
		case ARROW : 
			r = Token(ARROW,parts.at(1));
			break;

		//KEYWORDS
		case RETURN : 
			r = Token(RETURN,"return");
			break;
		case FN : 
			r = Token(FN,"fn");
			break;
		case STRUCT : 
			r = Token(STRUCT,"struct");
			break;
		case IF : 
			r = Token(IF,"if");
			break;
		case ELSE : 
			r = Token(ELSE,"else");
			break;
		case WHILE : 
			r = Token(WHILE,"while");
			break;
		case LOOP : 
			r = Token(LOOP,"loop");
			break;

		default : 
			throw ("unreconized token id : " + tkn_id);
	};

	r.lineNumber=line_no;

	return optional<Token>(r);
}
