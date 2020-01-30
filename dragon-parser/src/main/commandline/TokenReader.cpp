//standard includes
#include <vector> 
#include <stdio.h>
#include <map>
#include <optional>

//project includes
#include "TokenReader.hpp"
#include "TokenKeys.hpp"

BaseToken recognizeStrConstToken(string strconst) {

	return new BaseToken(strconst.substring(1, strconst.length() - 1));
}

BaseToken recognizeCharConstToken(string charconst) {

	char v = charconst.charAt(1);
	if (charconst == '\n') {
		v = '\n';
	}
	return new BaseToken(v);
}

optional<BaseToken> recognizeToken(string tkn, bool debug) {
	//parses the token, as encoded by the lexer

	//everything being seperated by a space, and the whole line not
	//having any spaces in front or at the back is important to keep parsing simple.

	//DEBUG
	if (debug) {
		cout << "recognize: " << tkn << endl;
	}

	string[] parts = tkn.split(" ");

	int tkn_id = stoi(parts[0]);

	int line_no = 1;


	if (tkn_id == LINE_NO) {
		int line_no_change = stoi(parts[1]);
		line_no += line_no_change;
		return make_optional();
		//break;
	}

	BaseToken result;

	switch (tkn_id) {

		case STRINGCONST : 
			result= recognizeStrConstToken(tkn.substring((STRINGCONST + " ").length()));
			break;
		case CHARCONST : 
			result= recognizeCharConstToken(tkn.substring((CHARCONST + " ").length()));
			break;
		case ANYTYPE : 
			result= BaseToken(ANYTYPE);
			break;

		//CONSTANTS
		case BOOLCONST : 
			result= BaseToken(BOOLCONST,parseBoolean(parts[1]));
			break;
		case FLOATING : 
			result=BaseToken(FLOATING,stof(parts[1]));
			break;
		case INTEGER : 
			result= BaseToken(INTEGER,stoi(parts[1]));
			break;

		//IDENTIFIERS
		case IDENTIFIER : 
			result= BaseToken(IDENTIFIER,parts[1]);
			break;
		case TYPEIDENTIFIER : 
			result= BaseToken(TYPEIDENTIFIER,parts[1]);
			break;

		//SECTION: OPERATORNS
		case OPERATOR : 
			result= BaseToken(OPERATOR,parts[1]);
			break;

		case EQ : 
			result=BaseToken(EQ,"=");
			break;

		case STRUCTMEMBERACCESS : 
			result= BaseToken(STRUCTMEMBERACCESS);
			break;

		case TYPEPARAM : 
			result= BaseToken(TYPEPARAM,stoi(parts[1]));
			break;

		case NAMESPACE : 
			result= BaseToken(NAMESPACE);
			break;

		//BRACKETS, BRACES, PARENTHESES
		case LBRACKET : 
			result= BaseToken(LBRACKET);
			break;
		case RBRACKET : 
			result= BaseToken(RBRACKET);
			break;

		case LPARENS : 
			result= BaseToken(LPARENS);
			break;
		case RPARENS : 
			result= BaseToken(RPARENS);
			break;

		case LCURLY : 
			result=	BaseToken(LCURLY);
			break;
		case RCURLY : 
			result= BaseToken(RCURLY);
			break;

		case GREATER: 
			result= BaseToken(">");
			break;

		case LESSER : 
			result=BaseToken("<");
			break;

		case WAVE : 
			result= BaseToken(WAVE);
			break;

		case SEMICOLON : 
			result= BaseToken(SEMICOLON);
			break;

		case COMMA : 
			result= BaseToken(COMMA);
			break;
		case ARROW : 
			result= (parts[1].equals("->")) ?
				BaseToken(true, true) :
				BaseToken(true, false);
			break;

		//KEYWORDS
		case RETURN : 
			result= BaseToken(RETURN);
			break;
		case FN : 
			result= BaseToken(FN);
			break;
		case STRUCT : 
			result= BaseToken(STRUCT);
			break;
		case IF : 
			result= BaseToken(IF);
			break;
		case ELSE : 
			result= BaseToken(ELSE);
			break;
		case WHILE : 
			result= BaseToken(WHILE);
			break;
		case LOOP : 
			result= BaseToken(LOOP);
			break;

		default : 
			result= throw ("unreconized token id : " + tkn_id);
	};

	result.lineNumber=line_no;

	return make_optional(result);
}
