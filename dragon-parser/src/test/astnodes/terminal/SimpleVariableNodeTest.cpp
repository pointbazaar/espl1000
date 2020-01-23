package org.vanautrui.languages.test.astnodes.terminal;

import org.vanautrui.languages.compiler.lexing.tokens.IdentifierToken;
import org.vanautrui.languages.compiler.lexing.tokens.IntegerNonNegativeConstantToken;
import org.vanautrui.languages.compiler.lexing.tokens.no_members.LBracketToken;
import org.vanautrui.languages.compiler.lexing.tokens.no_members.RBracketToken;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.IntConstNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.SimpleVariableNode;

import java.nio.file.Paths;

public final class SimpleVariableNodeTest {

	public static int test_parse_simple_variable() throws Exception {

		final TokenList list = new TokenList(Paths.get("/dev/null"));
		list.add(new IdentifierToken("x"));

		SimpleVariableNode node = new SimpleVariableNode(list);
		boolean assert1 = "x".equals(node.name);

		return (assert1)?1:0;
	}

	public static int test_parse_simple_indexed_variable() throws Exception {
		final TokenList list = new TokenList(Paths.get("/dev/null"));
		list.add(new IdentifierToken("x"));
		list.add(new LBracketToken());
		list.add(new IntegerNonNegativeConstantToken(0));
		list.add(new RBracketToken());


		final SimpleVariableNode node = new SimpleVariableNode(list);
		boolean assert1 = "x".equals( node.name);
		boolean assert2 = (node.indexOptional.isPresent());

		boolean assert3 = (0 == ((IntConstNode) node.indexOptional.get().term1.termNode).number);

		return (assert1&&assert2&&assert3)?1:0;
	}
}
