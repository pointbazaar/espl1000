package org.vanautrui.languages.test.astnodes.nonterminals.statements.controlflow;

import org.vanautrui.languages.commandline.ParserPhases;
import org.vanautrui.languages.compiler.lexing.tokens.IdentifierToken;
import org.vanautrui.languages.compiler.lexing.tokens.IntegerNonNegativeConstantToken;
import org.vanautrui.languages.compiler.lexing.tokens.OperatorToken;
import org.vanautrui.languages.compiler.lexing.tokens.StringConstantToken;
import org.vanautrui.languages.compiler.lexing.tokens.no_members.*;
import org.vanautrui.languages.compiler.lexing.tokens.no_members.keywords.IfToken;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.IfStatementNode;

import java.nio.file.Paths;

public final class IfStatementNodeTest {

	public static int test1() throws Exception {
		final TokenList list = new TokenList(Paths.get("/dev/null"));
		list.add(new IfToken());

		list.add(new LParensToken());

		list.add(new IntegerNonNegativeConstantToken(5));
		list.add(new OperatorToken("<"));
		list.add(new IntegerNonNegativeConstantToken(3));

		list.add(new RParensToken());

		list.add(new LCurlyToken());

		list.add(new RCurlyToken());

		final IfStatementNode ifStatement = new IfStatementNode(list);
		boolean assert1 = (0 == list.size());

		return  (assert1)?1:0;
	}

	public static int test2() {

		try {
			final TokenList tokens = new TokenList(Paths.get("/dev/null"));

			tokens.add(new IfToken());

			tokens.add(new LParensToken());

			tokens.add(new IdentifierToken("x"));
			tokens.add(new OperatorToken("<"));
			tokens.add(new IntegerNonNegativeConstantToken(5));

			tokens.add(new RParensToken());

			tokens.add(new LCurlyToken());

			tokens.add(new IdentifierToken("println"));


			tokens.add(new LParensToken());
			tokens.add(new StringConstantToken("x<5"));
			tokens.add(new RParensToken());

			tokens.add(new SemicolonToken());

			tokens.add(new RCurlyToken());

			final IfStatementNode stmt = new IfStatementNode(tokens);

			return 1;
		}catch (Exception e){
			return 0;
		}
	}

	public static int test3() {

		try {
			final TokenList tokens = ParserPhases.makeTokenList("if( n <= 1 ){ return 1; }", false);

			final IfStatementNode stmt = new IfStatementNode(tokens);
			return 1;
		}catch (Exception e){
			return 0;
		}
	}
}
