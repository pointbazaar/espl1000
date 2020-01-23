package org.vanautrui.languages.test.astnodes.terminal;

import org.vanautrui.languages.compiler.lexing.tokens.FloatNonNegativeConstantToken;
import org.vanautrui.languages.compiler.lexing.tokens.OperatorToken;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.FloatConstNode;

import java.nio.file.Paths;

public final class FloatConstNodeTest {

	public static int test1() {
		try {
			final TokenList list = new TokenList(Paths.get("/dev/null"));
			list.add(new OperatorToken("-"));
			list.add(new FloatNonNegativeConstantToken(4.0f));

			final FloatConstNode node = new FloatConstNode(list);
			return 1;
		}catch (Exception e){
			return 0;
		}
	}

	public static int test2() throws Exception {
		try {
			final TokenList list = new TokenList(Paths.get("/dev/null"));
			list.add(new FloatNonNegativeConstantToken(4.0f));

			final FloatConstNode node = new FloatConstNode(list);
			return 1;
		}catch (Exception e){
			return 0;
		}
	}
}
