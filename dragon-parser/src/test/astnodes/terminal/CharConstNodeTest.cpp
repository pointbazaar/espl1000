package org.vanautrui.languages.test.astnodes.terminal;

import org.vanautrui.languages.compiler.lexing.tokens.CharConstantToken;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.CharConstNode;

import java.nio.file.Paths;

public final class CharConstNodeTest {

	public static int test_parse_char_constant_node() throws Exception {
		final TokenList list = new TokenList(Paths.get("/dev/null"));
		list.add(new CharConstantToken('h'));

		final CharConstNode node = new CharConstNode(list);
		boolean assert1 = ('h'== node.content);

		return (assert1)?1:0;
	}

	public static int test_parse_char_constant_node_newline() throws Exception {

		final TokenList list = new TokenList(Paths.get("/dev/null"));
		list.add(new CharConstantToken('\n'));

		final CharConstNode node = new CharConstNode(list);
		boolean assert1 = ('\n' == node.content);

		return (assert1)?1:0;
	}
}
