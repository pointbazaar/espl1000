package org.vanautrui.languages.test.astnodes.terminal;

import org.vanautrui.languages.compiler.lexing.tokens.IdentifierToken;
import org.vanautrui.languages.compiler.lexing.tokens.IntegerNonNegativeConstantToken;
import org.vanautrui.languages.compiler.lexing.tokens.StringConstantToken;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;

import java.nio.file.Paths;

public final class ExpressionNodeTest {

	public static int test_simple_expression() {

		try {
			final TokenList list = new TokenList(Paths.get("/dev/null"));
			list.add(new IntegerNonNegativeConstantToken(4));
			final ExpressionNode expr = new ExpressionNode(list);

			return 1;
		}catch (Exception e){
			return 0;
		}
	}

	public static int test_variable_name_expression() {

		try {
			final TokenList list = new TokenList(Paths.get("/dev/null"));

			list.add(new IdentifierToken("x"));

			final ExpressionNode expr = new ExpressionNode(list);

			return 1;
		}catch (Exception e){
			return 0;
		}
	}

	public static int recognize_string_constant_expression() {

		try {
			final TokenList tokens = new TokenList(Paths.get("/dev/null"));

			tokens.add(new StringConstantToken("hello"));

			final ExpressionNode expr = new ExpressionNode(tokens);

			return 1;
		}catch (Exception e){
			return 0;
		}
	}
}
