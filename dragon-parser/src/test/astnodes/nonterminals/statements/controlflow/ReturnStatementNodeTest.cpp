package org.vanautrui.languages.test.astnodes.nonterminals.statements.controlflow;

import org.vanautrui.languages.compiler.lexing.tokens.IdentifierToken;
import org.vanautrui.languages.compiler.lexing.tokens.IntegerNonNegativeConstantToken;
import org.vanautrui.languages.compiler.lexing.tokens.OperatorToken;
import org.vanautrui.languages.compiler.lexing.tokens.no_members.*;
import org.vanautrui.languages.compiler.lexing.tokens.no_members.keywords.ReturnToken;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.ReturnStatementNode;

import java.nio.file.Paths;

public final class ReturnStatementNodeTest {

	public static int test1() {
		try {
			final TokenList list = new TokenList(Paths.get("/dev/null"));
			list.add(new ReturnToken());

			list.add(new LParensToken());

			list.add(new OperatorToken("-"));
			list.add(new IntegerNonNegativeConstantToken(5));

			list.add(new RParensToken());

			list.add(new SemicolonToken());

			final ReturnStatementNode returnStatementNode = new ReturnStatementNode(list);

			return 1;
		}catch (Exception e){
			return 0;
		}
	}

	public static int test2() throws Exception {
		try {
			final TokenList list = new TokenList(Paths.get("/dev/null"));
			list.add(new ReturnToken());

			list.add(new LParensToken());

			list.add(new OperatorToken("-"));

			list.add(new IntegerNonNegativeConstantToken(5));

			list.add(new RParensToken());

			list.add(new OperatorToken("*"));

			list.add(new IdentifierToken("n"));

			list.add(new SemicolonToken());

			final ReturnStatementNode returnStatementNode = new ReturnStatementNode(list);

			return 1;
		}catch (Exception e){
			return 0;
		}
	}

	public static int test3() throws Exception {

		try {
			final TokenList list = new TokenList(Paths.get("/dev/null"));

			list.add(new ReturnToken());

			list.add(new IdentifierToken("arr"));

			list.add(new LBracketToken());
			list.add(new IntegerNonNegativeConstantToken(0));

			list.add(new RBracketToken());

			list.add(new SemicolonToken());

			final ReturnStatementNode returnStatementNode = new ReturnStatementNode(list);
			return 1;
		}catch (Exception e){
			return 0;
		}
	}


}
