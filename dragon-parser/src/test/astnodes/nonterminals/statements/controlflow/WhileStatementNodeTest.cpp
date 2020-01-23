package org.vanautrui.languages.test.astnodes.nonterminals.statements.controlflow;

import org.vanautrui.languages.compiler.lexing.tokens.IntegerNonNegativeConstantToken;
import org.vanautrui.languages.compiler.lexing.tokens.OperatorToken;
import org.vanautrui.languages.compiler.lexing.tokens.no_members.LCurlyToken;
import org.vanautrui.languages.compiler.lexing.tokens.no_members.LParensToken;
import org.vanautrui.languages.compiler.lexing.tokens.no_members.RCurlyToken;
import org.vanautrui.languages.compiler.lexing.tokens.no_members.RParensToken;
import org.vanautrui.languages.compiler.lexing.tokens.no_members.keywords.WhileToken;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.WhileStatementNode;

import java.nio.file.Paths;

public final class WhileStatementNodeTest {

	public static int test1() throws Exception {
		try {
			final TokenList list = new TokenList(Paths.get("/dev/null"));
			list.add(new WhileToken());

			list.add(new LParensToken());

			list.add(new IntegerNonNegativeConstantToken(5));
			list.add(new OperatorToken("<"));
			list.add(new IntegerNonNegativeConstantToken(3));

			list.add(new RParensToken());

			list.add(new LCurlyToken());

			list.add(new RCurlyToken());

			final WhileStatementNode whileStatement = new WhileStatementNode(list);

			return 1;
		}catch (Exception e){
			return 0;
		}
	}
}
