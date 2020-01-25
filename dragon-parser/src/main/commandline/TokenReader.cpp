//standard includes
#include <vector> 
#include <stdio.h>
#include <map>

//project includes
#include "../compiler/lexing/tokens.*.hpp"
#include "../compiler/lexing/tokens.no_members.*.hpp"
#include "../compiler/lexing/tokens.no_members.keywords.*.hpp"
#include "../compiler/lexing/utils.IToken.hpp"

#include "../commandline/TokenKeys/*.hpp"
#include "../commandline/TokenKeys/LOOP.hpp"


StringConstantToken recognizeStrConstToken(string strconst) {

	return new StringConstantToken(strconst.substring(1, strconst.length() - 1));
}

CharConstantToken recognizeCharConstToken(string charconst) {

	char v = charconst.charAt(1);
	if (charconst.equals("'\\n'")) {
		v = '\n';
	}
	return new CharConstantToken(v);
}

Optional<BaseToken> recognizeToken(string tkn, bool debug) throws Exception {
	//parses the token, as encoded by the lexer

	//everything being seperated by a space, and the whole line not
	//having any spaces in front or at the back is important to keep parsing simple.

	//DEBUG
	if (debug) {
		out.println("recognize: " + tkn);
	}

	String[] parts = tkn.split(" ");

	int tkn_id = Integer.parseInt(parts[0]);

	int line_no = 1;


	if (tkn_id == LINE_NO) {
		int line_no_change = parseInt(parts[1]);
		line_no += line_no_change;
		return Optional.empty();
		//break;
	}

	const BaseToken result = switch (tkn_id) {

		case STRINGCONST -> recognizeStrConstToken(tkn.substring((STRINGCONST + " ").length()));
		case CHARCONST -> recognizeCharConstToken(tkn.substring((CHARCONST + " ").length()));

		case ANYTYPE -> new AnyTypeToken();

		//CONSTANTS
		case BOOLCONST -> new BoolConstantToken(parseBoolean(parts[1]));
		case FLOATING -> new FloatNonNegativeConstantToken(parseFloat(parts[1]));
		case INTEGER -> new IntegerNonNegativeConstantToken(parseInt(parts[1]));

		//IDENTIFIERS
		case IDENTIFIER -> new IdentifierToken(parts[1]);
		case TYPEIDENTIFIER -> new TypeIdentifierToken(parts[1]);


		//SECTION: OPERATORNS
		case OPERATOR -> new OperatorToken(parts[1]);

		case EQ -> new OperatorToken("=");

		case STRUCTMEMBERACCESS -> new StructMemberAccessToken();

		case TYPEPARAM -> new TypeParameterIdentifierToken(parseInt(parts[1]));

		case NAMESPACE -> new NamespaceToken();

		//BRACKETS, BRACES, PARENTHESES
		case LBRACKET -> new LBracketToken();
		case RBRACKET -> new RBracketToken();

		case LPARENS -> new LParensToken();
		case RPARENS -> new RParensToken();

		case LCURLY -> new LCurlyToken();
		case RCURLY -> new RCurlyToken();

		case GREATER -> new OperatorToken(">");
		case LESSER -> new OperatorToken("<");

		case WAVE -> new WaveToken();

		case SEMICOLON -> new SemicolonToken();

		case COMMA -> new CommaToken();

		case ARROW -> (parts[1].equals("->")) ?
				new ArrowToken(true, true) :
				new ArrowToken(true, false);

		//KEYWORDS
		case RETURN -> new ReturnToken();
		case FN -> new FnToken();
		case STRUCT -> new StructToken();
		case IF -> new IfToken();
		case ELSE -> new ElseToken();
		case WHILE -> new WhileToken();
		case LOOP -> new LoopToken();

		default -> throw new Exception("unreconized token id : " + tkn_id);
	};

	result.setLineNumber(line_no);

	return Optional.of(result);
}
