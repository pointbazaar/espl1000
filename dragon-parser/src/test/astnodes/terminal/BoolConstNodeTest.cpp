package org.vanautrui.languages.test.astnodes.terminal;

import org.vanautrui.languages.compiler.lexing.tokens.BoolConstantToken;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.BoolConstNode;

import java.nio.file.Paths;

public final class BoolConstNodeTest {

	public static int test_parse_bool_constant_node() throws Exception {
		final TokenList list = new TokenList(Paths.get("/dev/null"));
		list.add(new BoolConstantToken(true));

		final BoolConstNode node = new BoolConstNode(list);
		boolean assert1 = ( node.boolValue);

		return (assert1)?1:0;
	}
}
