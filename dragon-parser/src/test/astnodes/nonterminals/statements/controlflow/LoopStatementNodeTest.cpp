package org.vanautrui.languages.test.astnodes.nonterminals.statements.controlflow;

import org.vanautrui.languages.compiler.lexing.tokens.IntegerNonNegativeConstantToken;
import org.vanautrui.languages.compiler.lexing.tokens.no_members.LCurlyToken;
import org.vanautrui.languages.compiler.lexing.tokens.no_members.RCurlyToken;
import org.vanautrui.languages.compiler.lexing.tokens.no_members.keywords.LoopToken;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.LoopStatementNode;

import java.nio.file.Paths;

public final class LoopStatementNodeTest {

	public static int test1() throws Exception {
		try {
			TokenList list = new TokenList(Paths.get("/dev/null"));
			list.add(new LoopToken());
			list.add(new IntegerNonNegativeConstantToken(5));
			list.add(new LCurlyToken());

			list.add(new RCurlyToken());

			final LoopStatementNode loop = new LoopStatementNode(list);

			return 1;
		}catch (Exception e){
			return 0;
		}
	}
}
