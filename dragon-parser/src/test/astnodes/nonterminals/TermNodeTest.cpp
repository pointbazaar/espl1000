package org.vanautrui.languages.test.astnodes.nonterminals;

import org.vanautrui.languages.compiler.lexing.tokens.IdentifierToken;
import org.vanautrui.languages.compiler.lexing.tokens.IntegerNonNegativeConstantToken;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.TermNode;

import java.nio.file.Paths;

public final class TermNodeTest {

	public static int test_simple_term() {

		try {
			final TokenList list = new TokenList(Paths.get("/dev/null"));
			list.add(new IntegerNonNegativeConstantToken(4));
			final TermNode expr = new TermNode(list);
			return 1;
		}catch (Exception e){
			return 0;
		}
	}

	public static int test_variable_term() {

		try {
			final TokenList list = new TokenList(Paths.get("/dev/null"));
			list.add(new IdentifierToken("x"));
			final TermNode expr = new TermNode(list);
			return 1;
		}catch (Exception e){
			return 0;
		}
	}


}
